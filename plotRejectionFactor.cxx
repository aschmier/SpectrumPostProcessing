// Macro for comparing rejection factors in pp 2012 jet data

#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TH2D.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D_rf.C"
#include "/home/austin/alice/SpectrumPostProcessing/makeRejectionFactor.cpp"

void plotRejectionFactor(TString datafile, TString mcfile, TString output, int radius, double lowval = 4., double highval = 12.25, TString suffix = "", TString fileType = "pdf", TString system = "pp")
{
    // global variables
    double textSize = 0.04;
    TString jetType   = "Full";

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    // open files
    TFile *fdata   = TFile::Open(datafile);
    if(!fdata || fdata->IsZombie()) return;

    TFile *fmc  = TFile::Open(mcfile);
    if(!fmc || fmc->IsZombie()) return;

    // create output directory
    gSystem->Exec(Form("mkdir -p %s/RejectionFactors",output.Data()));
    gSystem->Exec(Form("mkdir -p %s/RejectionFactors/ClustersData",output.Data()));
    gSystem->Exec(Form("mkdir -p %s/RejectionFactors/ClustersMC",output.Data()));
    gSystem->Exec(Form("mkdir -p %s/RejectionFactors/ClusterTE",output.Data()));    

    // initialize vectors. EMC7 trigger is used twice because we need one with course binning and one with fine binning
    vector<double> binningFine   = getJetPtBinningRejectionFactorsFine();
    vector<double> binningCourse = getJetPtBinningRejectionFactorsCourse();
    //vector<double> binningFine   = getJetPtBinningRejectionFactorsFineVar("option4");
    //vector<double> binningCourse = getJetPtBinningRejectionFactorsCourseVar("option4");
    vector<TString> triggers{"INT7","EMC7","EMC7","EJE"};
    vector<TString> triggersMC{"INT7","INT7","EMC7","EMC7","EJE"};
    //vector<TString> triggers{"INT7","EJ2","EJ2","EJ1"};
    //vector<TString> triggersMC{"INT7","INT7","EJ2","EJ2","EJ1"};
    //vector<TString> triggers{"INT7","EG2","EG2","EG1"};
    //vector<TString> triggersMC{"INT7","INT7","EJ2","EJ2","EJ1"};
    vector<vector<double>> binnings{binningCourse,binningCourse,binningFine,binningFine};
    vector<vector<double>> binningsMC{binningCourse,binningFine,binningCourse,binningFine,binningFine};
    vector<TH1D*> vecClusters;
    vector<TH1D*> vecClustersMC;

    for(int trigger=0; trigger<triggersMC.size(); trigger++){
        TString name    = Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,triggersMC.at(trigger).Data());
        TDirectory *dir = (TDirectory*)fmc->Get(name.Data());
        TList *list     = (TList*)dir->Get(name.Data());
        TH1D *clusters  = (TH1D*)list->FindObject("hClusterEnergy1D");
        TH1D *clustersRebinned = (TH1D*)clusters->Rebin(binningsMC.at(trigger).size()-1, Form("clusterRebinnedMC_%i",trigger), binningsMC.at(trigger).data());
        clustersRebinned->Scale(1.,"width");
        vecClustersMC.push_back(clustersRebinned);
    }

    for(int trigger=0; trigger<triggers.size(); trigger++){
        TString name    = Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,triggers.at(trigger).Data());
        TDirectory *dir = (TDirectory*)fdata->Get(name.Data());
        TList *list     = (TList*)dir->Get(name.Data());
        TH1D *events    = (TH1D*)list->FindObject("hClusterCounterAbs");
        TH1D *clusters  = (TH1D*)list->FindObject("hClusterEnergy1D");
        TH1D *normhist  = (TH1D*)list->FindObject("fNormalisationHist"); // take the one from the AliAnalysisTaskEmcal directly
        double vtxfeff  = normhist->GetBinContent(normhist->GetXaxis()->FindBin("Vertex reconstruction and quality")) / normhist->GetBinContent(normhist->GetXaxis()->FindBin("Event selection"));
        clusters->Scale(1./(events->GetBinContent(1)/vtxfeff));
        TH1D *clustersRebinned = (TH1D*)clusters->Rebin(binnings.at(trigger).size()-1, Form("raw_rebin_pp_%i",trigger), binnings.at(trigger).data());
        clustersRebinned->Scale(1.,"width");
        vecClusters.push_back(clustersRebinned);
    }

    // Make unscaled rejection factors
    std::tuple<double, double, TH1 *> TupleLowUnscaled;
    std::tuple<double, double, TH1 *> TupleHighUnscaled;

    if(system=="pp"){
        TupleLowUnscaled  = makeRejectionFactor(vecClusters.at(1), vecClusters.at(0), radius, "INT7", "EMC7", "default", lowval, 40., 1.5, 40.);
        TupleHighUnscaled = makeRejectionFactor(vecClusters.at(3), vecClusters.at(2), radius, "EMC7", "EJE", "default", highval, 60., 6., 200.);
    } else if(system=="pPb"){
        TupleLowUnscaled  = makeRejectionFactor(vecClusters.at(1), vecClusters.at(0), radius, "INT7", "EG2", "default", lowval, 40., 6.1, 40., false);
        TupleHighUnscaled = makeRejectionFactor(vecClusters.at(3), vecClusters.at(2), radius, "EG2", "EG1", "default", highval, 50., 10.2, 50., false);
    }else cout << "Wrong system" << endl;
    
    double valLowUnscaled = (double)(std::get<0>(TupleLowUnscaled));
    double errLowUnscaled = (double)(std::get<1>(TupleLowUnscaled));
    TH1D *RFacLowUnscaled = (TH1D*)(std::get<2>(TupleLowUnscaled));

    double valHighUnscaled = (double)(std::get<0>(TupleHighUnscaled));
    double errHighUnscaled = (double)(std::get<1>(TupleHighUnscaled));
    TH1D *RFacHighUnscaled = (TH1D*)(std::get<2>(TupleHighUnscaled));

    // Make efficiency-scaled rejection factors
    TH1D *effScaleCourseEMC7 = (TH1D*)vecClustersMC.at(2)->Clone("effScaleCourseLow");
    effScaleCourseEMC7->Divide(vecClustersMC.at(2),vecClustersMC.at(0));

    TH1D *effScaleFineEMC7 = (TH1D*)vecClustersMC.at(3)->Clone("effScaleFineLow");
    effScaleFineEMC7->Divide(vecClustersMC.at(3),vecClustersMC.at(1));

    TH1D *effScaleFineEJE = (TH1D*)vecClustersMC.at(4)->Clone("effScaleFineHigh");
    effScaleFineEJE->Divide(vecClustersMC.at(4),vecClustersMC.at(1));

    TH1D *clustersRebinnedScaledCourseEMC7 = (TH1D*)vecClusters.at(1)->Clone("clustersRebinnedScaledCourseEMC7");
    clustersRebinnedScaledCourseEMC7->Divide(vecClusters.at(1),effScaleCourseEMC7);

    TH1D *clustersRebinnedScaledFineEMC7 = (TH1D*)vecClusters.at(2)->Clone("clustersRebinnedScaledFineEMC7");
    clustersRebinnedScaledFineEMC7->Divide(vecClusters.at(2),effScaleFineEMC7);

    TH1D *clustersRebinnedScaledEJE = (TH1D*)vecClusters.at(3)->Clone("clustersRebinnedScaledEJE");
    clustersRebinnedScaledEJE->Divide(vecClusters.at(3),effScaleFineEJE);

    std::tuple<double, double, TH1 *> TupleLow  = makeRejectionFactor(clustersRebinnedScaledCourseEMC7, vecClusters.at(0), radius, "INT7", "EMC7", "default", lowval, 23., 1.5, 40.,false);
    std::tuple<double, double, TH1 *> TupleHigh = makeRejectionFactor(clustersRebinnedScaledEJE, clustersRebinnedScaledFineEMC7, radius, "EMC7", "EJE", "default", highval, 90., 6., 200.,false);

    double valLow = (double)(std::get<0>(TupleLow));
    double errLow = (double)(std::get<1>(TupleLow));
    TH1D *RFacLow = (TH1D*)(std::get<2>(TupleLow));

    double valHigh = (double)(std::get<0>(TupleHigh));
    double errHigh = (double)(std::get<1>(TupleHigh));
    TH1D *RFacHigh = (TH1D*)(std::get<2>(TupleHigh));


    // Set up canvas for plotting
    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogy(1);
    c->SetLogx(0);

    TH1D *dummyRF = new TH1D("dummyRF","",260,2,261);
    dummyRF->GetYaxis()->SetRangeUser(40,120);
    SetStyleHistoTH1ForGraphs(dummyRF,"","#it{p}_{T} (GeV/c)","#it{RF}",0.03,0.04,0.03,0.04,1,1);

    TH1D *dummyCluster = new TH1D("dummyCluster","",210,0,210);
    dummyCluster->GetYaxis()->SetRangeUser(1e-9,1);
    SetStyleHistoTH1ForGraphs(dummyCluster,"","p_{T} (GeV/#it{c})","#frac{1}{#it{N}^{trig}}#frac{d#it{N}^{cl}}{d#it{p}_{T}^{cl}}",0.03,0.04,0.03,0.04,1,1);

    TH1D *dummyEff = new TH1D("dummyEff","",258,2,260);
    dummyEff->GetYaxis()->SetRangeUser(0.87,1.05);
    SetStyleHistoTH1ForGraphs(dummyEff,"","p_{T} (GeV/#it{c})","Trigger Efficiency",0.03,0.04,0.03,0.04,1,1);

    TLegend *legendClus =  GetAndSetLegend2(0.34,(0.94-(4)*textSize),0.54,0.94,textSize,1);
    TLegend *legendEff =  GetAndSetLegend2(0.13,(0.93-(3)*textSize),0.33,0.93,textSize,1);
    TLegend *legendRF =  GetAndSetLegend2(0.12,(0.93-(2)*textSize),0.35,0.93,textSize,1);

    dummyCluster->Draw("axis");
    for(int i=0; i<vecClusters.size(); i++){
        vecClusters.at(i)->GetXaxis()->SetRangeUser(5,200);
        vecClusters.at(i)->SetMarkerStyle(styles[i]);
        vecClusters.at(i)->SetMarkerColor(colors[i]);
        vecClusters.at(i)->SetLineColor(colors[i]);
        vecClusters.at(i)->SetMarkerSize(2);
        if(i==1) legendClus->AddEntry(vecClusters.at(i), "EMC7 Course Binning", "p");
        else if(i==2) legendClus->AddEntry(vecClusters.at(i), "EMC7 Fine Binning", "p");
        else legendClus->AddEntry(vecClusters.at(i), triggers.at(i).Data(), "p");
        vecClusters.at(i)->Draw("p,e,same");
    }
    legendClus->Draw("same");
    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.94,0.9, textSize,false, false, true);
    drawLatexAdd("EMCal Clusters",0.94,0.86, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{E}^{cl} > 0.3 GeV; |#it{#eta}^{cl}| < 0.7",0.94,0.82, textSize,kFALSE, kFALSE, true);
    c->SaveAs(Form("%s/RejectionFactors/ClustersData/clusters_R0%i_%s.%s",output.Data(),radius,suffix.Data(),fileType.Data()));

    dummyCluster->Draw("axis");
    for(int i=0; i<vecClustersMC.size(); i++){
        vecClustersMC.at(i)->GetXaxis()->SetRangeUser(5,200);
        vecClustersMC.at(i)->SetMarkerStyle(styles[i]);
        vecClustersMC.at(i)->SetMarkerColor(colors[i]);
        vecClustersMC.at(i)->SetLineColor(colors[i]);
        vecClustersMC.at(i)->SetMarkerSize(2);
        vecClustersMC.at(i)->Draw("p,e,same");
    }
    legendClus->Draw("same");
    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.94,0.9, textSize,false, false, true);
    drawLatexAdd("EMCal Clusters",0.94,0.86, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{E}^{cl} > 0.3 GeV; |#it{#eta}^{cl}| < 0.7",0.94,0.82, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Simulated PYTHIA8+GEANT4",0.94,0.78, textSize,kFALSE, kFALSE, true);
    c->SaveAs(Form("%s/RejectionFactors/ClustersMC/clustersMC_R0%i_%s.%s",output.Data(),radius,suffix.Data(),fileType.Data()));

    c->SetLogy(0);
    c->SetLogx(1);

    TLine * l = new TLine (2,1,261,1);
    l->SetLineColor(14);
    l->SetLineWidth(3);
    l->SetLineStyle(7);

    effScaleCourseEMC7->GetXaxis()->SetRangeUser(2,200);
    effScaleCourseEMC7->SetMarkerStyle(styles[0]);
    effScaleCourseEMC7->SetMarkerColor(colors[0]);
    effScaleCourseEMC7->SetLineColor(colors[0]);
    legendEff->AddEntry(effScaleCourseEMC7, "EMC7 Course Binning", "p");
    effScaleFineEMC7->GetXaxis()->SetRangeUser(2,200);
    effScaleFineEMC7->SetMarkerStyle(styles[1]);
    effScaleFineEMC7->SetMarkerColor(colors[1]);
    effScaleFineEMC7->SetLineColor(colors[1]);
    legendEff->AddEntry(effScaleFineEMC7, "EMC7 Fine Binning", "p");
    effScaleFineEJE->GetXaxis()->SetRangeUser(10,200);
    effScaleFineEJE->SetMarkerStyle(styles[2]);
    effScaleFineEJE->SetMarkerColor(colors[2]);
    effScaleFineEJE->SetLineColor(colors[2]);
    legendEff->AddEntry(effScaleFineEJE, "EJE", "p");
    dummyEff->Draw("axis");
    effScaleCourseEMC7->Draw("p,e,same");
    effScaleFineEMC7->Draw("p,e,same");
    effScaleFineEJE->Draw("p,e,same");

    l->Draw("same");
    legendEff->Draw("same");
    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.94,0.9, textSize,false, false, true);
    drawLatexAdd("EMCal Clusters",0.94,0.86, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{E}^{cl} > 0.3 GeV; |#it{#eta}^{cl}| < 0.7",0.94,0.82, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Simulated PYTHIA8+GEANT4",0.94,0.78, textSize,kFALSE, kFALSE, true);
    c->SaveAs(Form("%s/RejectionFactors/ClusterTE/ClusterTriggerEfficiencies_R0%i_%s.%s",output.Data(),radius,suffix.Data(),fileType.Data()));

    c->SetLogy(0);
    c->SetLogx(1);

    RFacLow->GetXaxis()->SetRangeUser(2.5,40);
    RFacLow->SetMarkerStyle(styles[0]);
    RFacLow->SetMarkerColor(colors[0]);
    RFacLow->SetLineColor(colors[0]);
    RFacLow->SetMarkerSize(2);
    RFacLow->Sumw2();

    RFacHigh->GetXaxis()->SetRangeUser(7,200);
    RFacHigh->SetMarkerStyle(styles[7]);
    RFacHigh->SetMarkerColor(colors[2]);
    RFacHigh->SetLineColor(colors[2]);
    RFacHigh->SetMarkerSize(2);
    RFacHigh->Sumw2();

    dummyRF->Draw("axis");
    RFacLow->Draw("p,e,same");
    RFacHigh->Draw("p,e,same");

    legendRF->AddEntry(RFacLow, Form("EMC7/INT7 RF = %1.2f #pm %1.2f",valLow,errLow), "p");
    legendRF->AddEntry(RFacHigh, Form("EJE/EMC7 RF = %1.2f #pm %1.2f",valHigh,errHigh), "p");
    legendRF->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.14,0.8, textSize,false, false, false);
    drawLatexAdd("EMCal Clusters",0.14,0.76, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("#it{E}^{cl} > 0.3 GeV; |#it{#eta}^{cl}| < 0.7",0.14,0.72, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Efficiency Scaled",0.14,0.68, textSize,kFALSE, kFALSE, false);

    c->SaveAs(Form("%s/RejectionFactors/RF_R0%i_%s.%s",output.Data(),radius,suffix.Data(),fileType.Data()));

    legendRF->Clear();
    c->SetLogx(1);
    c->SetLogy(0);

    dummyRF->GetXaxis()->SetRangeUser(1,261);
    dummyRF->GetYaxis()->SetRangeUser(20,120);

    RFacLowUnscaled->GetXaxis()->SetRangeUser(1.5,40);
    RFacLowUnscaled->SetMarkerStyle(styles[0]);
    RFacLowUnscaled->SetMarkerColor(colors[0]);
    RFacLowUnscaled->SetLineColor(colors[0]);
    RFacLowUnscaled->SetMarkerSize(2);
    RFacLowUnscaled->Sumw2();

    RFacHighUnscaled->GetXaxis()->SetRangeUser(6,200);
    RFacHighUnscaled->SetMarkerStyle(styles[7]);
    RFacHighUnscaled->SetMarkerColor(colors[2]);
    RFacHighUnscaled->SetLineColor(colors[2]);
    RFacHighUnscaled->SetMarkerSize(2);
    RFacHighUnscaled->Sumw2();

    dummyRF->Draw("axis");
    RFacLowUnscaled->Draw("p,e,same");
    RFacHighUnscaled->Draw("p,e,same");

    legendRF->AddEntry(RFacLowUnscaled, Form("EMC7/INT7 RF = %1.2f #pm %1.2f",valLowUnscaled,errLowUnscaled), "p");
    legendRF->AddEntry(RFacHighUnscaled, Form("EJE/EMC7 RF = %1.2f #pm %1.2f",valHighUnscaled,errHighUnscaled), "p");
    legendRF->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.14,0.8, textSize,false, false, false);
    drawLatexAdd("EMCal Clusters",0.14,0.76, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("#it{E}^{cl} > 0.3 GeV; |#it{#eta}^{cl}| < 0.7",0.14,0.72, textSize,kFALSE, kFALSE, false);
    c->SaveAs(Form("%s/RejectionFactors/RF_R0%i_Unscaled_%s.%s",output.Data(),radius,suffix.Data(),fileType.Data()));

}
