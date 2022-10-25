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
#include "/home/austin/alice/unfoldingResults/makeRejectionFactor.cpp"

void plotRejectionFactor(TString mbfile, TString emc7file, TString ejefile, TString output, Int_t rad, TString fileType)
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

    // initialize vectors. EMC7 trigger is used twice because we need one with course binning and one with fine binning
    vector<TFile*> files{fmb,femc7,femc7,feje};
    vector<double> binningFine   = getJetPtBinningRejectionFactorsFine();
    vector<double> binningCourse = getJetPtBinningRejectionFactorsCourse();
    //vector<double> binningFine   = getJetPtBinningRejectionFactorsFineVar("option4");
    //vector<double> binningCourse = getJetPtBinningRejectionFactorsCourseVar("option4");
    vector<TString> triggers{"INT7","EMC7","EMC7","EJE"};
    vector<vector<double>> binnings{binningCourse,binningCourse,binningFine,binningFine};
    vector<TH1D*> vecClusters;

    for(int trigger=0; trigger<triggers.size(); trigger++){
        TString name    = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr",jetType.Data(),radius,triggers.at(trigger).Data());
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

    std::tuple<double, double, TH1 *> TupleLow  = makeRejectionFactor(vecClusters.at(1), vecClusters.at(0), radius, "INT7", "EMC7", "nodownscalecorr", 4., 40., 1.5, 40.);
    std::tuple<double, double, TH1 *> TupleHigh = makeRejectionFactor(vecClusters.at(3), vecClusters.at(2), radius, "EMC7", "EJE", "nodownscalecorr", 12., 60., 6., 200.);

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

    for(int i=vecClusters.size()-1; i>=0; i--){
        vecClusters.at(i)->GetXaxis()->SetRangeUser(20,320);
        vecClusters.at(i)->GetYaxis()->SetRangeUser(1e-9,10);
        //vecClusters.at(i)->GetYaxis()->SetRangeUser(5,100);
        SetStyleHistoTH1ForGraphs(vecClusters.at(i),"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);

        vecClusters.at(i)->SetMarkerStyle(styles[i]);
        vecClusters.at(i)->SetMarkerColor(colors[i]);
        vecClusters.at(i)->SetLineColor(colors[i]);
        vecClusters.at(i)->SetMarkerSize(2);
        TString save = (i==(vecClusters.size()-1)? "p,e" : "p,e,same");
        vecClusters.at(i)->Draw(save.Data());
    }
    //c->SaveAs(Form("%s/RejectionFactors/clusters_R0%i.%s",output.Data(),radius,fileType.Data()));

    c->SetLogy(0);
    c->SetLogx(1);

    TLegend *legend =  GetAndSetLegend2(0.12,(0.86-(2)*textSize),0.31,0.86,textSize,1);

    RFacLow->GetXaxis()->SetMoreLogLabels();
    //RFacLow->GetXaxis()->SetRangeUser(1,200);
    RFacLow->GetYaxis()->SetRangeUser(5,110);
    RFacLow->SetMarkerStyle(styles[0]);
    RFacLow->SetMarkerColor(colors[0]);
    RFacLow->SetLineColor(colors[0]);
    RFacLow->SetMarkerSize(2);
    RFacLow->Sumw2();

    RFacHigh->SetMarkerStyle(styles[7]);
    RFacHigh->SetMarkerColor(colors[2]);
    RFacHigh->SetLineColor(colors[2]);
    RFacHigh->SetMarkerSize(2);
    RFacHigh->Sumw2();

    SetStyleHistoTH1ForGraphs(RFacLow,"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);
    SetStyleHistoTH1ForGraphs(RFacHigh,"","p_{T} (GeV/c)","RF",0.03,0.04,0.03,0.04,1,1.2);

    RFacLow->Draw("p,e");
    RFacHigh->Draw("p,e,same");

    legend->AddEntry(RFacLow, Form("EMC7/INT7 RF = %1.2f #pm %1.2f",valLow,errLow), "p");
    legend->AddEntry(RFacHigh, Form("EJE/EMC7 RF = %1.2f #pm %1.2f",valHigh,errHigh), "p");
    legend->Draw("same");

    drawLatexAdd(Form("pp #it{#sqrt{s_{NN}}} = 8 TeV; Full Jets, R = 0.%i",radius),0.14,0.9, textSize,false, false, false);

    c->SaveAs(Form("%s/RejectionFactors/RF_R0%i.%s",output.Data(),radius,fileType.Data()));
}
