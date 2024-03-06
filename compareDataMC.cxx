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

void compareDataMC(TString unfoldedfilepp, TString unfoldedfilepPb, TString mcfilepp, TString mcfilepPb, TString output, int minradius, int maxradius, TString fileType)
{
    Double_t textSize     = 0.03;
    int styles[4] = {24,25,27,28};
    int colors[4] = {kBlack, kRed+2, kBlue+1, kGreen+2};
     vector<double> rebinning = getJetPtBinningNonLinSmear8TeV();


    TFile *fUnfpp = TFile::Open(unfoldedfilepp);
    if(!fUnfpp || fUnfpp->IsZombie()){
        cout << "pp Data file not found!" << endl;
        return;
    }

    TFile *fUnfpPb = TFile::Open(unfoldedfilepPb);
    if(!fUnfpPb || fUnfpPb->IsZombie()){
        cout << "pPb Data file not found!" << endl;
        return;
    }

    TFile *fMCpp = TFile::Open(mcfilepp);
    if(!fMCpp || fMCpp->IsZombie()){
        cout << "pp MC file not found!" << endl;
        return;
    }

    TFile *fMCpPb = TFile::Open(mcfilepPb);
    if(!fMCpPb || fMCpPb->IsZombie()){
        cout << "pPb MC file not found!" << endl;
        return;
    }

    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogy();

    // Make dummy histo
    TH1D *h = new TH1D("h","",260,0,260);
    SetStyleHistoTH1ForGraphs(h,"","p_{T} (GeV/c)","d#sigma/dp_{T}d#eta (mb/(GeV/c))",0.03,0.04,0.03,0.04,1,1.2);
    h->GetXaxis()->SetRangeUser(0,320);
    h->GetYaxis()->SetRangeUser(1e-9,2e-2);

    TLegend *legend =  GetAndSetLegend2(0.12,0.14,0.5,0.12+(4)*textSize*1.5,textSize);

    for(int radius = minradius; radius <=maxradius; radius++){
        c->Clear();
        legend->Clear();
        h->Draw("axis");

        TDirectory *dUnfpp         = (TDirectory*)fUnfpp->Get(Form("R0%i",radius));
        TDirectory *rawlevelpp     = (TDirectory*)dUnfpp->Get("rawlevel");
        TH1D *hrawpp               = (TH1D*)rawlevelpp->Get(Form("hraw_R0%i",radius));
        hrawpp->Scale(1., "width");

        TDirectory *dUnfpPb         = (TDirectory*)fUnfpPb->Get(Form("R0%i",radius));
        TDirectory *rawlevelpPb     = (TDirectory*)dUnfpPb->Get("rawlevel");
        TH1D *hrawpPb               = (TH1D*)rawlevelpPb->Get(Form("hraw_R0%i",radius));
        hrawpPb->Scale(1., "width");
        hrawpPb->Scale(1./208.);

        TDirectory *dMCpp        = (TDirectory*)fMCpp->Get(Form("JetSpectrum_FullJets_R0%i_INT7_default",radius));
        TList *lMCpp             = (TList*)dMCpp->Get(Form("JetSpectrum_FullJets_R0%i_INT7_default",radius));
        TH2D *hMCSpectrum2Dpp    = (TH2D*)lMCpp->FindObject("hJetSpectrum");
        TH1D *hMCSpectrumppfine  = (TH1D*)hMCSpectrum2Dpp->ProjectionY(Form("hMCSpectrumppfine_R0%i", radius), 1, 1);
        TH1D *hMCSpectrumpp      = (TH1D*)hMCSpectrumppfine->Rebin(rebinning.size()-1, Form("hMCSpectrumpp_R0%i",radius), rebinning.data());
        hMCSpectrumpp->Scale(1., "width");

        TDirectory *dMCpPb       = (TDirectory*)fMCpPb->Get(Form("JetSpectrum_FullJets_R0%i_INT7_default",radius));
        TList *lMCpPb            = (TList*)dMCpPb->Get(Form("JetSpectrum_FullJets_R0%i_INT7_default",radius));
        TH2D *hMCSpectrum2DpPb   = (TH2D*)lMCpPb->FindObject("hJetSpectrum");
        TH1D *hMCSpectrumpPbfine = (TH1D*)hMCSpectrum2DpPb->ProjectionY(Form("hMCSpectrumpPbfine_R0%i", radius), 1, 1);
        TH1D *hMCSpectrumpPb     = (TH1D*)hMCSpectrumpPbfine->Rebin(rebinning.size()-1, Form("hMCSpectrumpPb_R0%i",radius), rebinning.data());
        hMCSpectrumpPb->Scale(1., "width");

        hrawpp->SetFillColor(colors[0]);
        hrawpp->SetMarkerStyle(styles[0]);
        hrawpp->SetMarkerSize(2.5);
        hrawpp->SetMarkerColor(colors[0]);
        legend->AddEntry(hrawpp, "Data pp 8 TeV", "p");
        hrawpp->Draw("same");

        hrawpPb->SetFillColor(colors[3]);
        hrawpPb->SetMarkerStyle(styles[3]);
        hrawpPb->SetMarkerSize(2.5);
        hrawpPb->SetMarkerColor(colors[3]);
        legend->AddEntry(hrawpPb, "(Data p--Pb 8.16 TeV)/208", "p");
        hrawpPb->Draw("same");

        hMCSpectrumpp->SetFillColor(colors[1]);
        hMCSpectrumpp->SetMarkerStyle(styles[1]);
        hMCSpectrumpp->SetMarkerSize(2.5);
        hMCSpectrumpp->SetMarkerColor(colors[1]);
        legend->AddEntry(hMCSpectrumpp, "PYTHIA 8 TeV", "p");
        hMCSpectrumpp->Draw("same");

        hMCSpectrumpPb->SetFillColor(colors[2]);
        hMCSpectrumpPb->SetMarkerStyle(styles[2]);
        hMCSpectrumpPb->SetMarkerSize(2.5);
        hMCSpectrumpPb->SetMarkerColor(colors[2]);
        legend->AddEntry(hMCSpectrumpPb, "PYTHIA 8.16 TeV", "p");
        hMCSpectrumpPb->Draw("same");
    

        legend->Draw("same");

        drawLatexAdd("p--Pb (pp) #it{#sqrt{s_{NN}}} = 8.16 (8) TeV",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd("Full Jets",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);

        c->SaveAs(Form("%s/CompareDataMC/CompareDataMC_R0%i.%s",output.Data(),radius,fileType.Data()));
    }
}
