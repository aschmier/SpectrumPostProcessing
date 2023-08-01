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

void plotRejectionFactor_pPb(TString mbfile, TString ej2file, TString ej1file, TString mcfile, TString output, int radius, TString fileType)
{
    // global variables
    Double_t textSize = 0.04;
    TString jetType   = "Full";

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    // open files
    TFile *fmb   = TFile::Open(mbfile);
    if(!fmb || fmb->IsZombie()) return;

    TFile *fej2 = TFile::Open(ej2file);
    if(!fej2 || fej2->IsZombie()) return;

    TFile *fej1  = TFile::Open(ej1file);
    if(!fej1 || fej1->IsZombie()) return;

    TFile *fmc  = TFile::Open(mcfile);
    if(!fmc || fmc->IsZombie()) return;

    // initialize vectors. EMC7 trigger is used twice because we need one with course binning and one with fine binning
    vector<TFile*> files{fmb,fej2,fej2,fej1};
    vector<double> binningFine   = getJetPtBinningRejectionFactorsHigh_pPb();
    vector<double> binningCourse = getJetPtBinningRejectionFactorsLow_pPb();
    
    vector<TString> triggersEJ{"INT7","EJ2","EJ2","EJ1"};
    vector<TString> triggersEG{"INT7","EG2","EG2","EG1"};
    vector<TString> triggersMC{"INT7","INT7","EJ2","EJ2","EJ1"};
    vector<vector<double>> binnings{binningCourse,binningCourse,binningFine,binningFine};
    vector<vector<double>> binningsMC{binningCourse,binningFine,binningCourse,binningFine,binningFine};
    //vector<vector<double>> binnings{binningCourse,binningCourse,binningCourse,binningCourse};
    //vector<vector<double>> binningsMC{binningCourse,binningCourse,binningCourse,binningCourse,binningCourse};
    vector<TH1D*> vecClustersEJ;
    vector<TH1D*> vecClustersEG;

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

    for(int trigger=0; trigger<triggersEJ.size(); trigger++){
        TString name    = Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,triggersEJ.at(trigger).Data());
        TDirectory *dir = (TDirectory*)files.at(trigger)->Get(name.Data());
        TList *list     = (TList*)dir->Get(name.Data());
        TH1D *events    = (TH1D*)list->FindObject("hClusterCounterAbs");
        TH1D *clusters  = (TH1D*)list->FindObject("hClusterEnergy1D");
        TH1D *normhist  = (TH1D*)list->FindObject("fNormalisationHist"); // take the one from the AliAnalysisTaskEmcal directly
        double vtxfeff  = normhist->GetBinContent(normhist->GetXaxis()->FindBin("Vertex reconstruction and quality")) / normhist->GetBinContent(normhist->GetXaxis()->FindBin("Event selection"));
        clusters->Scale(1./(events->GetBinContent(1)/vtxfeff));
        TH1D *clustersRebinned = (TH1D*)clusters->Rebin(binnings.at(trigger).size()-1, Form("raw_rebin_pp_%i",trigger), binnings.at(trigger).data());
        clustersRebinned->Scale(1.,"width");
        vecClustersEJ.push_back(clustersRebinned);
    }

    for(int trigger=0; trigger<triggersEG.size(); trigger++){
        TString name    = Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,triggersEG.at(trigger).Data());
        TDirectory *dir = (TDirectory*)files.at(trigger)->Get(name.Data());
        TList *list     = (TList*)dir->Get(name.Data());
        TH1D *events    = (TH1D*)list->FindObject("hClusterCounterAbs");
        TH1D *clusters  = (TH1D*)list->FindObject("hClusterEnergy1D");
        TH1D *normhist  = (TH1D*)list->FindObject("fNormalisationHist"); // take the one from the AliAnalysisTaskEmcal directly
        double vtxfeff  = normhist->GetBinContent(normhist->GetXaxis()->FindBin("Vertex reconstruction and quality")) / normhist->GetBinContent(normhist->GetXaxis()->FindBin("Event selection"));
        clusters->Scale(1./(events->GetBinContent(1)/vtxfeff));
        TH1D *clustersRebinned = (TH1D*)clusters->Rebin(binnings.at(trigger).size()-1, Form("raw_rebin_pp_%i",trigger), binnings.at(trigger).data());
        clustersRebinned->Scale(1.,"width");
        vecClustersEG.push_back(clustersRebinned);
    }

    // Make unscaled rejection factors
    std::tuple<double, double, TH1 *> TupleLowEJ;
    std::tuple<double, double, TH1 *> TupleHighEJ;
    std::tuple<double, double, TH1 *> TupleLowEG;
    std::tuple<double, double, TH1 *> TupleHighEG;

    
    TupleLowEJ  = makeRejectionFactor(vecClustersEJ.at(1), vecClustersEJ.at(0), radius, "INT7", "EJ2", "default", 18., 50., 5., 50.);
    TupleHighEJ = makeRejectionFactor(vecClustersEJ.at(3), vecClustersEJ.at(2), radius, "EJ2", "EJ1", "default", 20., 50., 7., 50.);
    TupleLowEG  = makeRejectionFactor(vecClustersEG.at(1), vecClustersEG.at(0), radius, "INT7", "EG2", "default", 6.1, 40., 7., 50.,false);
    TupleHighEG = makeRejectionFactor(vecClustersEG.at(3), vecClustersEG.at(2), radius, "EG2", "EG1", "default", 10.2, 50., 6., 50.,false);

    double valLowEJ = (double)(std::get<0>(TupleLowEJ));
    double errLowEJ = (double)(std::get<1>(TupleLowEJ));
    TH1D *RFacLowEJ = (TH1D*)(std::get<2>(TupleLowEJ));

    double valHighEJ = (double)(std::get<0>(TupleHighEJ));
    double errHighEJ = (double)(std::get<1>(TupleHighEJ));
    TH1D *RFacHighEJ = (TH1D*)(std::get<2>(TupleHighEJ));

    double valLowEG = (double)(std::get<0>(TupleLowEG));
    double errLowEG = (double)(std::get<1>(TupleLowEG));
    TH1D *RFacLowEG = (TH1D*)(std::get<2>(TupleLowEG));

    double valHighEG = (double)(std::get<0>(TupleHighEG));
    double errHighEG = (double)(std::get<1>(TupleHighEG));
    TH1D *RFacHighEG = (TH1D*)(std::get<2>(TupleHighEG));

    // Make efficiency-scaled rejection factors
    TH1D *effScaleCourseEJ2 = (TH1D*)vecClustersMC.at(2)->Clone("effScaleCourseLow");
    effScaleCourseEJ2->Divide(vecClustersMC.at(2),vecClustersMC.at(0));

    TH1D *effScaleFineEJ2 = (TH1D*)vecClustersMC.at(3)->Clone("effScaleFineLow");
    effScaleFineEJ2->Divide(vecClustersMC.at(3),vecClustersMC.at(1));

    TH1D *effScaleFineEJ1 = (TH1D*)vecClustersMC.at(4)->Clone("effScaleFineHigh");
    effScaleFineEJ1->Divide(vecClustersMC.at(4),vecClustersMC.at(1));

    TH1D *clustersRebinnedScaledCourseEJ2 = (TH1D*)vecClustersEJ.at(1)->Clone("clustersRebinnedScaledCourseEJ2");
    clustersRebinnedScaledCourseEJ2->Divide(vecClustersEJ.at(1),effScaleCourseEJ2);

    TH1D *clustersRebinnedScaledFineEJ2 = (TH1D*)vecClustersEJ.at(2)->Clone("clustersRebinnedScaledFineEJ2");
    clustersRebinnedScaledFineEJ2->Divide(vecClustersEJ.at(2),effScaleFineEJ2);

    TH1D *clustersRebinnedScaledEJ1 = (TH1D*)vecClustersEJ.at(3)->Clone("clustersRebinnedScaledEJ1");
    clustersRebinnedScaledEJ1->Divide(vecClustersEJ.at(3),effScaleFineEJ1);

    std::tuple<double, double, TH1 *> TupleLow  = makeRejectionFactor(clustersRebinnedScaledCourseEJ2, vecClustersEJ.at(0), radius, "INT7", "EJ2", "default", 10., 100., 1.5, 40.,false);
    std::tuple<double, double, TH1 *> TupleHigh = makeRejectionFactor(clustersRebinnedScaledEJ1, clustersRebinnedScaledFineEJ2, radius, "EJ2", "EJ1", "default", 5, 40., 6., 200.,false);

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
    c->SetLogy(0);
    c->SetLogx(1);

    // Make dummy histogram for axes
    TH1D *h = new TH1D("h","",240,0,240);
    SetStyleHistoTH1ForGraphs(h,"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);
    //h->GetXaxis()->SetMoreLogLabels();
    h->GetYaxis()->SetRangeUser(1e3,5e3);

    h->Draw("axis");

    // Make legend
    TLegend *legend =  GetAndSetLegend2(0.5,(0.95-(2)*textSize),0.7,0.95,textSize,1);

    RFacLowEJ->SetMarkerStyle(styles[0]);
    RFacLowEJ->SetMarkerColor(colors[0]);
    RFacLowEJ->SetLineColor(colors[0]);
    RFacLowEJ->SetMarkerSize(2);
    //RFacLowEJ->Sumw2();

    RFacHighEJ->SetMarkerStyle(styles[1]);
    RFacHighEJ->SetMarkerColor(colors[1]);
    RFacHighEJ->SetLineColor(colors[1]);
    RFacHighEJ->SetMarkerSize(2);
    //RFacHighEJ->Sumw2();

    RFacLowEG->SetMarkerStyle(styles[2]);
    RFacLowEG->SetMarkerColor(colors[2]);
    RFacLowEG->SetLineColor(colors[2]);
    RFacLowEG->SetMarkerSize(2);
    //RFacLowEG->Sumw2();

    RFacHighEG->SetMarkerStyle(styles[3]);
    RFacHighEG->SetMarkerColor(colors[3]);
    RFacHighEG->SetLineColor(colors[3]);
    RFacHighEG->SetMarkerSize(2);
    //RFacHighEG->Sumw2();

    RFacLow->SetMarkerStyle(styles[4]);
    RFacLow->SetMarkerColor(colors[4]);
    RFacLow->SetLineColor(colors[4]);
    RFacLow->SetMarkerSize(2);
    //RFacLow->Sumw2();

    RFacHigh->SetMarkerStyle(styles[5]);
    RFacHigh->SetMarkerColor(colors[5]);
    RFacHigh->SetLineColor(colors[5]);
    RFacHigh->SetMarkerSize(2);
    //RFacHigh->Sumw2();

    ////////////////////////////////////////////////////

    RFacLow->Draw("p,e,same");

    //RFacLowEJ->Draw("p,e,same");
    //RFacHighEJ->Draw("p,e,same");
    //RFacLowEG->Draw("p,e,same");
    //RFacHighEG->Draw("p,e,same");

    legend->AddEntry(RFacLow, Form("EJ2/INT7 RF = %1.2f #pm %1.2f",valLow,errLow), "p");

    //legend->AddEntry(RFacLowEJ, Form("EJ2/INT7 RF = %1.2f #pm %1.2f",valLowEJ,errLowEJ), "p");
    //legend->AddEntry(RFacHighEJ, Form("EJ1/EJ2 RF = %1.2f #pm %1.2f",valHighEJ,errHighEJ), "p");
    //legend->AddEntry(RFacLowEG, Form("EG2/INT7 RF = %1.2f #pm %1.2f",valLowEG,errLowEG), "p");
    //legend->AddEntry(RFacHighEG, Form("EG1/EG2 RF = %1.2f #pm %1.2f",valHighEG,errHighEG), "p");

    legend->Draw("same");

    drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV; Full Jets",0.25,0.2, textSize,false, false, false);
    drawLatexAdd("Corrected for vertex finding efficiency",0.25,0.16, textSize,false, false, false);


    c->SaveAs(Form("%s/RejectionFactors/pPbCustom/RF_R0%i_Low.%s",output.Data(),radius,fileType.Data()));

    ///////////////////////////////////////////////////////
    legend->Clear();
    h->GetYaxis()->SetRangeUser(2,4);
    h->GetXaxis()->SetRangeUser(2,70);

    h->Draw("axis");


    RFacHigh->Draw("p,e,same");

    //RFacLowEJ->Draw("p,e,same");
    //RFacHighEJ->Draw("p,e,same");
    //RFacLowEG->Draw("p,e,same");
    //RFacHighEG->Draw("p,e,same");

    legend->AddEntry(RFacHigh, Form("EJ1/EJ2 RF = %1.2f #pm %1.2f",valHigh,errHigh), "p");

    //legend->AddEntry(RFacLowEJ, Form("EJ2/INT7 RF = %1.2f #pm %1.2f",valLowEJ,errLowEJ), "p");
    //legend->AddEntry(RFacHighEJ, Form("EJ1/EJ2 RF = %1.2f #pm %1.2f",valHighEJ,errHighEJ), "p");
    //legend->AddEntry(RFacLowEG, Form("EG2/INT7 RF = %1.2f #pm %1.2f",valLowEG,errLowEG), "p");
    //legend->AddEntry(RFacHighEG, Form("EG1/EG2 RF = %1.2f #pm %1.2f",valHighEG,errHighEG), "p");

    legend->Draw("same");

    drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV; Full Jets",0.25,0.2, textSize,false, false, false);
    drawLatexAdd("Corrected for vertex finding efficiency",0.25,0.16, textSize,false, false, false);


    c->SaveAs(Form("%s/RejectionFactors/pPbCustom/RF_R0%i_High.%s",output.Data(),radius,fileType.Data()));

}
