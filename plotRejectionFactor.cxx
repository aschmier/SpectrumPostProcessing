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

void plotRejectionFactor(TString mbfile, TString emc7file, TString ejefile, TString mcfile, TString output, Int_t rad, TString fileType, TString system)
{
    // global variables
    Double_t textSize = 0.04;
    Int_t radius      = rad;
    TString jetType   = "Full";

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    // open files
    TFile *fmb   = TFile::Open(mbfile);
    if(!fmb || fmb->IsZombie()) return;

    TFile *femc7 = TFile::Open(emc7file);
    if(!femc7 || femc7->IsZombie()) return;

    TFile *feje  = TFile::Open(ejefile);
    if(!feje || feje->IsZombie()) return;

    TFile *fmc  = TFile::Open(mcfile);
    if(!fmc || fmc->IsZombie()) return;

    // initialize vectors. EMC7 trigger is used twice because we need one with course binning and one with fine binning
    vector<TFile*> files{fmb,femc7,femc7,feje};
    vector<double> binningFine   = getJetPtBinningRejectionFactorsFine();
    vector<double> binningCourse = getJetPtBinningRejectionFactorsCourse();
    //vector<double> binningFine   = getJetPtBinningRejectionFactorsFineVar("option4");
    //vector<double> binningCourse = getJetPtBinningRejectionFactorsCourseVar("option4");
    //vector<TString> triggers{"INT7","EMC7","EMC7","EJE"};
    //vector<TString> triggersMC{"INT7","INT7","EMC7","EMC7","EJE"};
    //vector<TString> triggers{"INT7","EJ2","EJ2","EJ1"};
    //vector<TString> triggersMC{"INT7","INT7","EJ2","EJ2","EJ1"};
    vector<TString> triggers{"INT7","EG2","EG2","EG1"};
    vector<TString> triggersMC{"INT7","INT7","EJ2","EJ2","EJ1"};
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
        TDirectory *dir = (TDirectory*)files.at(trigger)->Get(name.Data());
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
        TupleLowUnscaled  = makeRejectionFactor(vecClusters.at(1), vecClusters.at(0), radius, "INT7", "EMC7", "default", 4., 40., 1.5, 40.);
        TupleHighUnscaled = makeRejectionFactor(vecClusters.at(3), vecClusters.at(2), radius, "EMC7", "EJE", "default", 12.25, 60., 6., 200.);
    } else if(system=="pPb"){
        TupleLowUnscaled  = makeRejectionFactor(vecClusters.at(1), vecClusters.at(0), radius, "INT7", "EG2", "default", 6.1, 40., 6.1, 40., false);
        TupleHighUnscaled = makeRejectionFactor(vecClusters.at(3), vecClusters.at(2), radius, "EG2", "EG1", "default", 10.2, 50., 10.2, 50., false);
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

    std::tuple<double, double, TH1 *> TupleLow  = makeRejectionFactor(clustersRebinnedScaledCourseEMC7, vecClusters.at(0), radius, "INT7", "EMC7", "default", 4., 30., 1.5, 40.,false);
    std::tuple<double, double, TH1 *> TupleHigh = makeRejectionFactor(clustersRebinnedScaledEJE, clustersRebinnedScaledFineEMC7, radius, "EMC7", "EJE", "default", 12.25, 200., 6., 200.,false);

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
    //c->SetLogx(0);

    TLegend *legendClus =  GetAndSetLegend2(0.12,(0.86-(2)*textSize),0.31,0.86,textSize,1);

    for(int i=0; i<vecClusters.size(); i++){
        vecClusters.at(i)->GetXaxis()->SetRangeUser(10,200);
        vecClusters.at(i)->GetYaxis()->SetRangeUser(1e-9,10);
        //vecClusters.at(i)->GetYaxis()->SetRangeUser(5,100);
        SetStyleHistoTH1ForGraphs(vecClusters.at(i),"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);

        vecClusters.at(i)->SetMarkerStyle(styles[i]);
        vecClusters.at(i)->SetMarkerColor(colors[i]);
        vecClusters.at(i)->SetLineColor(colors[i]);
        vecClusters.at(i)->SetMarkerSize(2);
        legendClus->AddEntry(vecClusters.at(i), triggers.at(i).Data(), "p");

        TString save = ((i==0)? "p,e" : "p,e,same");
        vecClusters.at(i)->Draw(save.Data());
    }
    c->SaveAs(Form("%s/RejectionFactors/ClustersData/clusters_R0%i.%s",output.Data(),radius,fileType.Data()));

    for(int i=0; i<vecClustersMC.size(); i++){
        vecClustersMC.at(i)->GetXaxis()->SetRangeUser(10,200);
        vecClustersMC.at(i)->GetYaxis()->SetRangeUser(1e-9,10);
        //vecClustersMC.at(i)->GetYaxis()->SetRangeUser(5,100);
        SetStyleHistoTH1ForGraphs(vecClustersMC.at(i),"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);

        vecClustersMC.at(i)->SetMarkerStyle(styles[i]);
        vecClustersMC.at(i)->SetMarkerColor(colors[i]);
        vecClustersMC.at(i)->SetLineColor(colors[i]);
        vecClustersMC.at(i)->SetMarkerSize(2);
        TString save = ((i==0)? "p,e" : "p,e,same");
        vecClustersMC.at(i)->Draw(save.Data());
    }
    c->SaveAs(Form("%s/RejectionFactors/ClustersMC/clustersMC_R0%i.%s",output.Data(),radius,fileType.Data()));

    for(int i=0; i<vecClustersMC.size(); i++){
        vecClustersMC.at(i)->GetXaxis()->SetRangeUser(10,200);
        vecClustersMC.at(i)->GetYaxis()->SetRangeUser(1e-9,10);
        //vecClustersMC.at(i)->GetYaxis()->SetRangeUser(5,100);
        SetStyleHistoTH1ForGraphs(vecClustersMC.at(i),"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);

        vecClustersMC.at(i)->SetMarkerStyle(styles[i]);
        vecClustersMC.at(i)->SetMarkerColor(colors[i]);
        vecClustersMC.at(i)->SetLineColor(colors[i]);
        vecClustersMC.at(i)->SetMarkerSize(2);
        TString save = ((i==0)? "p,e" : "p,e,same");
        vecClustersMC.at(i)->Draw(save.Data());
    }
    c->SaveAs(Form("%s/RejectionFactors/clustersMC_R0%i.%s",output.Data(),radius,fileType.Data()));

    c->SetLogy(0);
    //c->SetLogx(0);

    TLine * l = new TLine (0,1,200,1);
    l->SetLineColor(14);
    l->SetLineWidth(3);
    l->SetLineStyle(7);

    effScaleCourseEMC7->GetXaxis()->SetRangeUser(0,200);
    effScaleCourseEMC7->GetYaxis()->SetRangeUser(0,1.4);
    SetStyleHistoTH1ForGraphs(effScaleCourseEMC7,"","p_{T} (GeV/c)","EMC7 Trig. Eff. (Clusters)",0.03,0.04,0.03,0.04,1,1.2);
    effScaleCourseEMC7->Draw("p,e");
    l->Draw("same");
    c->SaveAs(Form("%s/RejectionFactors/ClusterTE/EMC7course_R0%i.%s",output.Data(),radius,fileType.Data()));

    effScaleFineEMC7->GetXaxis()->SetRangeUser(0,200);
    effScaleFineEMC7->GetYaxis()->SetRangeUser(0,1.4);
    SetStyleHistoTH1ForGraphs(effScaleFineEMC7,"","p_{T} (GeV/c)","EMC7 Trig. Eff. (Clusters)",0.03,0.04,0.03,0.04,1,1.2);
    effScaleFineEMC7->Draw("p,e");
    l->Draw("same");
    c->SaveAs(Form("%s/RejectionFactors/ClusterTE/EMC7fine_R0%i.%s",output.Data(),radius,fileType.Data()));

    effScaleFineEJE->GetXaxis()->SetRangeUser(0,200);
    effScaleFineEJE->GetYaxis()->SetRangeUser(0,1.4);
    SetStyleHistoTH1ForGraphs(effScaleFineEJE,"","p_{T} (GeV/c)","EJE Trig. Eff. (Clusters)",0.03,0.04,0.03,0.04,1,1.2);
    effScaleFineEJE->Draw("p,e");
    l->Draw("same");
    c->SaveAs(Form("%s/RejectionFactors/ClusterTE/EJEfine_R0%i.%s",output.Data(),radius,fileType.Data()));

    c->SetLogy(0);
    c->SetLogx(1);

    TLegend *legend =  GetAndSetLegend2(0.52,(0.86-(2)*textSize),0.82,0.86,textSize,1);

    TH1D *dummy = (TH1D*)RFacLow->Clone("dummy");
    dummy->GetXaxis()->SetRangeUser(1,200);
    dummy->GetYaxis()->SetRangeUser(0,160);

    RFacLow->GetXaxis()->SetMoreLogLabels();
    RFacLow->GetXaxis()->SetRangeUser(0,40);
    RFacLow->GetYaxis()->SetRangeUser(5,210);
    RFacLow->SetMarkerStyle(styles[0]);
    RFacLow->SetMarkerColor(colors[0]);
    RFacLow->SetLineColor(colors[0]);
    RFacLow->SetMarkerSize(2);
    RFacLow->Sumw2();

    RFacLow->GetXaxis()->SetRangeUser(0,90);
    RFacHigh->SetMarkerStyle(styles[7]);
    RFacHigh->SetMarkerColor(colors[2]);
    RFacHigh->SetLineColor(colors[2]);
    RFacHigh->SetMarkerSize(2);
    RFacHigh->Sumw2();

    SetStyleHistoTH1ForGraphs(RFacLow,"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);
    SetStyleHistoTH1ForGraphs(RFacHigh,"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);

    TGraphErrors *RFacLowGraph = new TGraphErrors(RFacLow);
    TGraphErrors *RFacHighGraph = new TGraphErrors(RFacHigh);

    dummy->Draw("axis");
    RFacLow->Draw("p,e,same");
    RFacHigh->Draw("p,e,same");

    legend->AddEntry(RFacLow, Form("EMC7/INT7 RF = %1.2f #pm %1.2f",valLow,errLow), "p");
    legend->AddEntry(RFacHigh, Form("EJE/EMC7 RF = %1.2f #pm %1.2f",valHigh,errHigh), "p");
    legend->Draw("same");

    drawLatexAdd(Form("pp #it{#sqrt{s_{NN}}} = 8 TeV; Full Jets, R = 0.%i",radius),0.91,0.9, textSize,false, false, true);

    c->SaveAs(Form("%s/RejectionFactors/RF_R0%i.%s",output.Data(),radius,fileType.Data()));

    legend->Clear();
    c->SetLogx(1);
    //c->SetLogy();
    TLegend *legend2 =  GetAndSetLegend2(0.12,(0.86-(2)*textSize),0.32,0.86,textSize,1);


    RFacLowUnscaled->GetXaxis()->SetMoreLogLabels();
    //RFacLowUnscaled->GetXaxis()->SetRangeUser(1,200);
    if(system=="pp") RFacLowUnscaled->GetYaxis()->SetRangeUser(5,110);
    if(system=="pPb") RFacLowUnscaled->GetYaxis()->SetRangeUser(0,600);
    RFacLowUnscaled->SetMarkerStyle(styles[0]);
    RFacLowUnscaled->SetMarkerColor(colors[0]);
    RFacLowUnscaled->SetLineColor(colors[0]);
    RFacLowUnscaled->SetMarkerSize(2);
    RFacLowUnscaled->Sumw2();

    RFacHighUnscaled->SetMarkerStyle(styles[7]);
    RFacHighUnscaled->SetMarkerColor(colors[2]);
    RFacHighUnscaled->SetLineColor(colors[2]);
    RFacHighUnscaled->SetMarkerSize(2);
    RFacHighUnscaled->Sumw2();

    SetStyleHistoTH1ForGraphs(RFacLowUnscaled,"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);
    SetStyleHistoTH1ForGraphs(RFacHighUnscaled,"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);

    RFacLowUnscaled->Draw("p,e");
    RFacHighUnscaled->Draw("p,e,same");

    if(system=="pp"){
        legend2->AddEntry(RFacLowUnscaled, Form("EMC7/INT7 RF = %1.2f #pm %1.2f",valLowUnscaled,errLowUnscaled), "p");
        legend2->AddEntry(RFacHighUnscaled, Form("EJE/EMC7 RF = %1.2f #pm %1.2f",valHighUnscaled,errHighUnscaled), "p");
    }
    if(system=="pPb"){
        legend2->AddEntry(RFacLowUnscaled, Form("EG2/INT7 RF = %1.2f #pm %1.2f",valLowUnscaled,errLowUnscaled), "p");
        legend2->AddEntry(RFacHighUnscaled, Form("EG1/EG2 RF = %1.2f #pm %1.2f",valHighUnscaled,errHighUnscaled), "p");
    }
    legend2->Draw("same");

    if(system=="pp") drawLatexAdd(Form("pp #it{#sqrt{s_{NN}}} = 8 TeV; Full Jets, R = 0.%i",radius),0.14,0.9, textSize,false, false, false);
    if(system=="pPb") drawLatexAdd(Form("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV; Full Jets, R = 0.%i",radius),0.14,0.9, textSize,false, false, false);

    c->SaveAs(Form("%s/RejectionFactors/RF_R0%i_Unscaled.%s",output.Data(),radius,fileType.Data()));

}
