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


void plotSingleRun(TString inputfile, int radius, int run, TString outputdir)
{
    double textSize  = 0.04;
    int minradius    = 2;
    int maxradius    = 2;
    TString jetType  = "Full";
    TString fileType = "png";
    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<double> detlevelbin = getJetPtBinningNonLinSmear8TeV();
    vector<TString> triggers{"INT7","EJ2","EJ1"};
    TString spectrumMergedFile = "/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/AnalysisResults.root";

    // Create output directory if it doesn't already exist
    TString command = "mkdir -p " + outputdir;
    gSystem->Exec(command);

    // Open input file. Loop over all triggers for given radius. Plot hJetSpectrum for each trigger on same canvas.
    TFile *input = new TFile(inputfile);

    TCanvas *c = new TCanvas("c","c",800,600);
    gStyle->SetOptStat(0);
    // Put a legend in the upper right corner
    TLegend *leg = new TLegend(0.7,0.7,0.9,0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(textSize);
    c->SetLogx();

    // Open merged spectrum file
    TFile *inputMerged = new TFile(spectrumMergedFile);

    // Create dummy histogram to set axis ranges
    TH1D *hDummy = new TH1D("hDummy","hDummy",240,0,240);
    hDummy->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hDummy->GetYaxis()->SetTitle("dN/dp_{T} (GeV/c)^{-1}");
    hDummy->GetYaxis()->SetTitleOffset(1.5);
    hDummy->GetYaxis()->SetRangeUser(1e-8,1e-1);
    hDummy->Draw("axis");

    for (int i = 0; i < triggers.size(); i++)
    {
        // Get hJetSpectrum from merged file
        TDirectory *dirMerged = (TDirectory*)inputMerged->Get(Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,triggers[i].Data()));
        TList *listMerged = (TList*)dirMerged->Get(Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,triggers[i].Data()));
        TH2D *hJetSpectrum2DMerged = (TH2D*)listMerged->FindObject("hJetSpectrum");
        TH1D *hJetSpectrumFineMerged = hJetSpectrum2DMerged->ProjectionY("hJetSpectrumFineMerged",1,1);
        TH1D *eventsMerged = (TH1D*)listMerged->FindObject("hClusterCounter");
        double nEventsMerged = eventsMerged->GetBinContent(1);
        hJetSpectrumFineMerged->Scale(1./nEventsMerged);
        TH1D *hJetSpectrumMerged = (TH1D*)hJetSpectrumFineMerged->Rebin(detlevelbin.size()-1,"hJetSpectrumMerged",detlevelbin.data());
        hJetSpectrumMerged->Scale(1., "width");
        hJetSpectrumMerged->SetLineColor(colors[i+3]);
        hJetSpectrumMerged->SetMarkerColor(colors[i+3]);
        hJetSpectrumMerged->SetMarkerStyle(styles[i+3]);
        hJetSpectrumMerged->SetMarkerSize(1.5);
        hJetSpectrumMerged->SetLineWidth(2);
        if(triggers[i] == "INT7") hJetSpectrumMerged->GetXaxis()->SetRangeUser(10,60);
        if(triggers[i] == "EJ2") hJetSpectrumMerged->GetXaxis()->SetRangeUser(10,120);
        if(triggers[i] == "EJ1") hJetSpectrumMerged->GetXaxis()->SetRangeUser(10,240);
        hJetSpectrumMerged->Draw("p,e,same");
        leg->AddEntry(hJetSpectrumMerged, Form("%s",triggers[i].Data()), "p");

        TDirectory *dir = (TDirectory*)input->Get(Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,triggers[i].Data()));
        TList *list = (TList*)dir->Get(Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,triggers[i].Data()));
        TH2D *hJetSpectrum2D = (TH2D*)list->FindObject("hJetSpectrum");
        TH1D *hJetSpectrumFine = hJetSpectrum2D->ProjectionY("hJetSpectrumFine",1,1);
        TH1D *events = (TH1D*)list->FindObject("hClusterCounter");
        double nEvents = events->GetBinContent(1);
        hJetSpectrumFine->Scale(1./nEvents);
        TH1D *hJetSpectrum = (TH1D*)hJetSpectrumFine->Rebin(detlevelbin.size()-1,"hJetSpectrum",detlevelbin.data());
        hJetSpectrum->Scale(1., "width");
        hJetSpectrum->SetLineColor(colors[i]);
        hJetSpectrum->SetMarkerColor(colors[i]);
        hJetSpectrum->SetMarkerStyle(styles[i]);
        hJetSpectrum->SetMarkerSize(1.5);
        hJetSpectrum->SetLineWidth(2);
        if(triggers[i] == "INT7") hJetSpectrum->GetXaxis()->SetRangeUser(10,60);
        if(triggers[i] == "EJ2") hJetSpectrum->GetXaxis()->SetRangeUser(10,120);
        if(triggers[i] == "EJ1") hJetSpectrum->GetXaxis()->SetRangeUser(10,240);
        //hJetSpectrum->GetYaxis()->SetRangeUser(1e-8,1e-2);
        hJetSpectrum->Draw("p,e,same");
        leg->AddEntry(hJetSpectrum,triggers[i],"p");
    }

    // Create text box with run number
    TPaveText *pt = new TPaveText(0.2,0.8,0.4,0.9,"NDC");
    pt->SetBorderSize(0);
    pt->SetFillStyle(0);
    pt->SetTextSize(textSize);
    pt->AddText(Form("Run %d",run));
    pt->Draw();

    leg->Draw();
    c->SetLogy();
    c->SaveAs(Form("%s/Run%d_R%i.%s",outputdir.Data(),run,radius,fileType.Data()));
    delete hDummy;
    delete c;
    delete input;
}
