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

void checkNewProduction(TString ppfile, TString pPbFile, TString outputdir, TString fileType = "png", int radius = 2){
    // Open pp and pPb MC files and verify that they exist
    TFile *fpp = new TFile(ppfile);
    TFile *fpPb = new TFile(pPbFile);
    if(!fpp->IsOpen() || !fpPb->IsOpen()){
        cout << "Error: MC files not found" << endl;
        return;
    }

    // Create output directory if it doesn't exist
    if(!gSystem->OpenDirectory(outputdir)){
        gSystem->mkdir(outputdir);
    }

    // Create output file
    TString outputfile = outputdir + "/checkNewProduction." + fileType;
    TFile *out = new TFile(outputfile, "RECREATE");

    // Create output canvas
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    c1->SetLogy();
    gStyle->SetOptStat(0);

    // Create output legend
    TLegend *leg = new TLegend(0.6, 0.6, 0.9, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);

    // Get hJetSpectrum from pp and pPb MC file directories
    TString dirname = Form("JetSpectrum_FullJets_R0%i_INT7_default", radius);
    TDirectory *dirpp = (TDirectory*)fpp->Get(dirname.Data());
    TDirectory *dirpPb = (TDirectory*)fpPb->Get(dirname.Data());
    TList *listpp = (TList*)dirpp->Get(dirname.Data());
    TList *listpPb = (TList*)dirpPb->Get(dirname.Data());
    TH1D *hClusterCounterpp = (TH1D*)listpp->FindObject("hClusterCounter");
    TH1D *hClusterCounterpPb = (TH1D*)listpPb->FindObject("hClusterCounter");

    TH2D *hJetSpectrumpp2D = (TH2D*)listpp->FindObject("hJetSpectrum");
    TH1D *hJetSpectrumpp = (TH1D*)hJetSpectrumpp2D->ProjectionY("hJetSpectrumpp", 1, 1);
    TH2D *hJetSpectrumpPb2D = (TH2D*)listpPb->FindObject("hJetSpectrum");
    TH1D *hJetSpectrumpPb = (TH1D*)hJetSpectrumpPb2D->ProjectionY("hJetSpectrumpPb", 1, 1);

    // Draw pp and pPb spectra on the same canvas, add to legend, and save
    hJetSpectrumpp->SetMarkerStyle(20);
    hJetSpectrumpp->SetMarkerColor(kRed);
    hJetSpectrumpp->SetLineColor(kRed);
    hJetSpectrumpp->SetMarkerSize(0.8);
    hJetSpectrumpp->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hJetSpectrumpp->GetYaxis()->SetTitle("dN/dp_{T} (GeV/c)^{-1}");
    hJetSpectrumpp->GetYaxis()->SetTitleOffset(1.4);
    hJetSpectrumpp->SetTitle("");
    hJetSpectrumpp->GetXaxis()->SetRangeUser(20, 240);
    hJetSpectrumpp->Draw("p E1");
    hJetSpectrumpPb->SetMarkerStyle(20);
    hJetSpectrumpPb->SetMarkerColor(kBlue);
    hJetSpectrumpPb->SetLineColor(kBlue);
    hJetSpectrumpPb->SetMarkerSize(0.8);
    hJetSpectrumpPb->Draw("p E1 SAME");
    leg->AddEntry(hJetSpectrumpp, "pp MC", "P");
    leg->AddEntry(hJetSpectrumpPb, "p-Pb MC", "P");
    leg->Draw();
    c1->SaveAs(Form("%s/newProductionQA_R0%i.%s", outputdir.Data(), radius, fileType.Data()));

    // Make a dummy plot and draw the ratio of pPb to pp on it
    c1->SetLogy(0);
    TH1D *hDummy = new TH1D("hDummy", "hDummy", 220, 20, 240);
    hDummy->GetYaxis()->SetRangeUser(0.5, 1.5);
    hDummy->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hDummy->GetYaxis()->SetTitle("p-Pb / pp");
    hDummy->GetYaxis()->SetTitleOffset(1.4);
    hDummy->SetTitle("");
    hDummy->Draw("axis");
    DrawGammaLines(20.,240.,1.,1.,8.,16,9);
    TH1D *hRatio = (TH1D*)hJetSpectrumpPb->Clone("hRatio");
    hRatio->Divide(hJetSpectrumpp);
    hRatio->SetMarkerStyle(20);
    hRatio->SetMarkerColor(kBlack);
    hRatio->SetLineColor(kBlack);
    hRatio->SetMarkerSize(0.8);
    hRatio->Draw("p E1 SAME");

    c1->SaveAs(Form("%s/newProductionRatio_R0%i.%s", outputdir.Data(), radius, fileType.Data()));

}