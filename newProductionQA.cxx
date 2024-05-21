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

void newProductionQA(TString ppFile, TString pPbFile, TString outputdir, int radius, TString fileType, int pthardbin){
    // Open files
    TFile *fpp = TFile::Open(ppFile);
    if(!fpp) std::cout << "File not found: " << ppFile << std::endl;
    TFile *fpPb = TFile::Open(pPbFile);
    if(!fpPb) std::cout << "File not found: " << pPbFile << std::endl;

    // Get fHistTrials, fHistXsection, hClusterCounter, hJetSpectrum from the TList within the TDirectory for the given radius and the INT7 trigger
    TString dirname = Form("JetSpectrum_FullJets_R0%i_INT7_default", radius);
    TDirectory *dirpp = (TDirectory*)fpp->Get(dirname.Data());
    TDirectory *dirpPb = (TDirectory*)fpPb->Get(dirname.Data());
    TList *listpp = (TList*)dirpp->Get(dirname.Data());
    TList *listpPb = (TList*)dirpPb->Get(dirname.Data());
    TH1F *fHistTrialspp = (TH1F*)listpp->FindObject("fHistTrials");
    TProfile *fHistXsectionpp = (TProfile*)listpp->FindObject("fHistXsection");
    TH1D *hClusterCounterpp = (TH1D*)listpp->FindObject("hClusterCounter");
    TH1D *hClusterCounterpPb = (TH1D*)listpPb->FindObject("hClusterCounter");

    double eventspp = hClusterCounterpp->GetBinContent(1);
    double eventspPb = hClusterCounterpPb->GetBinContent(1);
    double trialspp = fHistTrialspp->GetBinContent(pthardbin+1);
    double trialspPb = fHistTrialspp->GetBinContent(pthardbin+1);
    double xsectionpp = fHistXsectionpp->GetBinContent(pthardbin+1);
    double xsectionpPb = fHistXsectionpp->GetBinContent(pthardbin+1);

    double scalefactorpp = xsectionpp/(trialspp*eventspp);
    double scalefactorpPb = xsectionpPb/(trialspPb*eventspPb);

    double scaledtrialspp = trialspp/eventspp;
    double scaledtrialspPb = trialspPb/eventspPb;

    // print scaled trials
    std::cout << "Scaled Trials pp: " << scaledtrialspp << std::endl;
    std::cout << "Scaled Trials pPb: " << scaledtrialspPb << std::endl;

    TH2D *hJetSpectrumpp2D = (TH2D*)listpp->FindObject("hJetSpectrum");
    TH1D *hJetSpectrumpp = (TH1D*)hJetSpectrumpp2D->ProjectionY("hJetSpectrumpp", 1, 1);
    if(pthardbin == 0) hJetSpectrumpp->Scale(1./eventspp);
    else hJetSpectrumpp->Scale(scalefactorpp);
    TH2D *hJetSpectrumpPb2D = (TH2D*)listpPb->FindObject("hJetSpectrum");
    TH1D *hJetSpectrumpPb = (TH1D*)hJetSpectrumpPb2D->ProjectionY("hJetSpectrumpPb", 1, 1);
    if(pthardbin == 0) hJetSpectrumpPb->Scale(1./eventspPb);
    else hJetSpectrumpPb->Scale(scalefactorpPb);

    TH1D *hJetSpectrumRatio = (TH1D*)hJetSpectrumpPb->Clone("hJetSpectrumRatio");
    hJetSpectrumRatio->Divide(hJetSpectrumpp);

    // Create canvas and plot hJetSpectrumRatio
    TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
    c1->SetTicks();
    c1->SetLeftMargin(0.15);
    c1->SetBottomMargin(0.15);
    gStyle->SetOptStat(0);
    hJetSpectrumRatio->SetTitle("");
    hJetSpectrumRatio->GetXaxis()->SetTitle("p_{T} (GeV/c)");
    hJetSpectrumRatio->GetXaxis()->SetTitleSize(0.05);
    hJetSpectrumRatio->GetXaxis()->SetTitleOffset(1.2);
    hJetSpectrumRatio->GetXaxis()->SetLabelSize(0.05);
    hJetSpectrumRatio->GetXaxis()->SetRangeUser(0, 20);
    hJetSpectrumRatio->GetYaxis()->SetTitle("#frac{p-Pb}{pp}");
    hJetSpectrumRatio->GetYaxis()->SetTitleSize(0.05);
    hJetSpectrumRatio->GetYaxis()->SetTitleOffset(1.2);
    hJetSpectrumRatio->GetYaxis()->SetLabelSize(0.05);
    hJetSpectrumRatio->GetYaxis()->SetRangeUser(0.5, 1.5);
    hJetSpectrumRatio->SetMarkerStyle(20);
    hJetSpectrumRatio->SetMarkerSize(0.8);
    hJetSpectrumRatio->SetMarkerColor(kBlack);
    hJetSpectrumRatio->SetLineColor(kBlack);
    hJetSpectrumRatio->Draw("p,E1");

    TString pthardlabel;
    if(pthardbin == 0) pthardlabel = "All";
    else pthardlabel = Form("%i", pthardbin);

    // Create legend and add pt hard bin label
    TLegend *leg = new TLegend(0.2, 0.7, 0.4, 0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->AddEntry(hJetSpectrumRatio, Form("p_{T} Hard Bin %s", pthardlabel.Data()), "p");

    // Add legend to canvas
    leg->Draw();

    // save canvas
    c1->SaveAs(Form("%s/simcomp_R0%i_pthard%s.%s", outputdir.Data(), radius, pthardlabel.Data(), fileType.Data()));
}