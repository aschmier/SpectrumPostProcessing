#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

void plotMinBiasComp(TString data_file, TString mc_file, TString outputdir, TString trigger, TString system)
{
    double textSize  = 0.03;
    int minradius    = 2;
    int maxradius    = 2;
    TString jetType  = "Full";
    TString fileType = "png";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<TH1D*> vecData;
    vector<TH1D*> vecMC;
    vector<double> partlevelbin = getJetPtBinningNonLinTrue8TeV();
    vector<double> detlevelbin = getJetPtBinningNonLinSmear8TeV();
    double xsec_pp   = 55.8;
    double xsec_pPb  = 2095.;

    TFile *fdata = TFile::Open(data_file);
    if(!fdata || fdata->IsZombie()){
        cout << "Data file not found!" << endl;
        return;
    }

    TFile *fmc = TFile::Open(mc_file);
    if(!fmc || fmc->IsZombie()){
        cout << "MC file not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        TString dirnameData     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, trigger.Data());
        TString dirnameMC     = Form("JetSpectrum_%sJets_R0%i_%s_Clusterizer5x5", jetType.Data(), radius, trigger.Data());    

        TDirectory *datadir = (TDirectory*)fdata->Get(dirnameData.Data());
        TList *datalist     = (TList*)datadir->Get(dirnameData.Data());
        TH1D *dataevents    = (TH1D*)datalist->FindObject("hClusterCounter");
        TH2D *dataspec2d    = (TH2D*)datalist->FindObject("hJetSpectrum");
        TH1D *dataspec1d    = (TH1D*)dataspec2d->ProjectionY(Form("dataspec1d_R0%i",radius),1,1);
        TH1D *rdataspec     = (TH1D*)dataspec1d->Rebin(detlevelbin.size()-1, Form("rdataspec_%i",radius), detlevelbin.data());
        rdataspec->Scale(1.,"width");
        rdataspec->Scale(1/dataevents->GetBinContent(1));
        vecData.push_back(rdataspec);

        TDirectory *mcdir   = (TDirectory*)fmc->Get(dirnameMC.Data());
        TList *mclist       = (TList*)mcdir->Get(dirnameMC.Data());
        TH1D *mcevents      = (TH1D*)mclist->FindObject("hClusterCounter");
        TH2D *mcspec2d      = (TH2D*)mclist->FindObject("hJetSpectrum");
        TH1D *mcspec1d      = (TH1D*)mcspec2d->ProjectionY(Form("mcspec1d_R0%i",radius),1,1);
        TH1D *rmcspec       = (TH1D*)mcspec1d->Rebin(detlevelbin.size()-1, Form("rmcspec_%i",radius), detlevelbin.data());
        rmcspec->Scale(1.,"width");
        rmcspec->Scale(1/mcevents->GetBinContent(1));
        vecMC.push_back(rmcspec);
    }

    TCanvas* canvas       = new TCanvas("canvas","",10,10,750,500);
    double leftMargin   = 0.1;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.77,(0.82-(2)*textSize),0.87,0.82,textSize,1);

    for(int radius = minradius; radius <= maxradius; radius++){
        vecData.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecData.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecData.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecData.at(radius-minradius)->GetXaxis()->SetRangeUser(20,320);
        vecData.at(radius-minradius)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecData.at(radius-minradius),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecData.at(radius-minradius)->Draw("p,e");
        legend->AddEntry(vecData.at(radius-minradius), "pp MC (INT7)", "p");

        vecMC.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius+1]);
        vecMC.at(radius-minradius)->SetMarkerColor(colors[radius-minradius+1]);
        vecMC.at(radius-minradius)->SetLineColor(colors[radius-minradius+1]);
        SetStyleHistoTH1ForGraphs(vecMC.at(radius-minradius),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecMC.at(radius-minradius)->Draw("p,e,same");
        legend->AddEntry(vecMC.at(radius-minradius), "pPb MC (INT7)", "p");


        legend->Draw();
        drawLatexAdd(Form("%s #it{#sqrt{s_{NN}}} = 8 TeV", system.Data()),0.78,0.88, 0.03, false, false, false);
        drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.78,0.84, 0.03, false, false, false);
        canvas->SaveAs(Form("%s/minbiascomp_%s_R0%i.%s", outputdir.Data(), system.Data(), radius, fileType.Data()));

        legend->Clear();

        canvas->SetLogy(0);

        TH1D *dummy = (TH1D*)vecData.at(radius-minradius)->Clone("ratio");
        dummy->Divide(vecData.at(radius-minradius),vecMC.at(radius-minradius));
        dummy->Divide(vecMC.at(radius-minradius),vecData.at(radius-minradius));
        dummy->Scale(2095./(55.8*208.));
        SetStyleHistoTH1ForGraphs(dummy,"","p_{T} (GeV/c)","pPb/pp",0.03,0.04,0.03,0.04,1,1.2);
        dummy->GetXaxis()->SetRangeUser(20,320);
        //dummy->GetYaxis()->SetRangeUser(0.85,1.15);
        //dummy->SetFillColor(17);
        dummy->SetMarkerStyle(21);
        dummy->SetMarkerSize(2.5);
        //dummy->SetMarkerColor(17);
        dummy->Sumw2();
        dummy->Draw();

        TLine * l1 = new TLine (0,1,280,1);
        l1->SetLineColor(14);
        l1->SetLineWidth(3);
        l1->SetLineStyle(7);
        l1->Draw("same");

        drawLatexAdd(Form("%s #it{#sqrt{s_{NN}}} = 8 TeV",system.Data()),0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);

        canvas->SaveAs(Form("%s/ratio_%s_R0%i.%s",outputdir.Data(),system.Data(),radius,fileType.Data()));

    }
}
