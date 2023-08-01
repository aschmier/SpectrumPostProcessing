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

void plotRpA(TString ppFile, TString pAFile, TString output, TString type, Int_t rad)
{
    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Setup ////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // Define variables
    Double_t textSize     = 0.04;
    Int_t radius          = rad;
    Int_t regnum          = 6;
    TString baseDirectory = "/home/austin/alice/RpA_results";
    TString fileType      = "png";

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    vector<double> partlevelbin = getJetPtBinningNonLinTrue8TeV();
    double xsec_pp = 55.8;
    double xsec_pPb = 2095.;

    // Open unfolding results file
    TFile *fpp = TFile::Open(ppFile);
    if(!fpp || fpp->IsZombie()) return;

    TFile *fpA = TFile::Open(pAFile);
    if(!fpA || fpA->IsZombie()) return;

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Get the histograms ///////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // Get pp histogram
    TDirectory *regDir_pp = (TDirectory*)fpp->Get(Form("R0%i",radius));
    TDirectory *reg_pp    = (TDirectory*)regDir_pp->Get(Form("reg%i",regnum));
    TDirectory *raw_pp    = (TDirectory*)regDir_pp->Get("rawlevel");
    TH1D *normUnfolded_pp = (TH1D*)reg_pp->Get(Form("normalized_reg%i",regnum));
    //TH1D *rawspectrum_pp  = (TH1D*)raw_pp->Get(Form("hraw_R0%i",radius));
    TH1D *finespectrum_pp = (TH1D*)raw_pp->Get(Form("hraw_fine_R0%i",radius));
    TH1D *raw_rebin_pp = (TH1D*)finespectrum_pp->Rebin(partlevelbin.size()-1, "raw_rebin_pp", partlevelbin.data());
    raw_rebin_pp->Scale(1.,"width");

    //TH1D *mbrebinned_pp = (TH1D*)raw

    // Get pA histogram
    TDirectory *regDir_pA = (TDirectory*)fpA->Get(Form("R0%i",radius));
    TDirectory *reg_pA    = (TDirectory*)regDir_pA->Get(Form("reg%i",regnum));
    TDirectory *raw_pA    = (TDirectory*)regDir_pA->Get("rawlevel");
    TH1D *normUnfolded_pA = (TH1D*)reg_pA->Get(Form("normalized_reg%i",regnum));
    //TH1D *rawspectrum_pA  = (TH1D*)raw_pA->Get(Form("hraw_R0%i",radius));
    TH1D *finespectrum_pA  = (TH1D*)raw_pA->Get(Form("hraw_fine_R0%i",radius));
    TH1D *raw_rebin_pA = (TH1D*)finespectrum_pA->Rebin(partlevelbin.size()-1, "raw_rebin_pA", partlevelbin.data());
    raw_rebin_pA->Scale(1.,"width");

    TH1D *RpA_unf = (TH1D*)normUnfolded_pp->Clone("RpA_unf");
    RpA_unf->Divide(normUnfolded_pA,normUnfolded_pp);

    TH1D *RpA_raw = (TH1D*)raw_rebin_pp->Clone("RpA_raw");
    RpA_raw->Divide(raw_rebin_pA,raw_rebin_pp);

    TH1D *pp_foldraw = (TH1D*)raw_rebin_pp->Clone("pp_foldraw");
    pp_foldraw->Divide(normUnfolded_pp,raw_rebin_pp,1,1,"b");

    TH1D *pA_foldraw = (TH1D*)raw_rebin_pA->Clone("pA_foldraw");
    pA_foldraw->Divide(normUnfolded_pA,raw_rebin_pA,1,1,"b");

    // Plotting stuff
    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.075,0.01,0.01,0.1);
    gStyle->SetOptStat(0);

    TH1D *dummyRpA = new TH1D("dummyRpA","",350,0,350);
    dummyRpA->GetXaxis()->SetRangeUser(0,260);
    dummyRpA->GetYaxis()->SetRangeUser(0,2);
    SetStyleHistoTH1ForGraphs(dummyRpA,"","#it{p}_{T} (GeV/c)","#it{R}_{pPb}",textSize,textSize*(4/3),textSize,textSize*(4/3),1.1,0.9);

    TH1D *dummyFoldRaw = new TH1D("dummyFoldRaw","",350,0,350);
    dummyFoldRaw->GetYaxis()->SetRangeUser(0,48);
    dummyFoldRaw->GetXaxis()->SetRangeUser(0,260);
    SetStyleHistoTH1ForGraphs(dummyFoldRaw,"","#it{p}_{T} (GeV/c)","Unfolded/Raw",textSize,textSize*(4/3),textSize,textSize*(4/3),1.1,0.8);

    TLine * l1 = new TLine (0,1,260,1);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);

    TLegend *legendFoldRaw =  GetAndSetLegend2(0.09,0.79-(2)*textSize,0.29,0.79,textSize);

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Draw the unfolded RpA ////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // Scaling
    RpA_unf->Scale(1./208.);

    // Set up histogram
    RpA_unf->GetXaxis()->SetRangeUser(20,240);
    RpA_unf->SetMarkerStyle(4);
    RpA_unf->SetLineColor(kBlack);
    RpA_unf->SetMarkerColor(kBlack);
    RpA_unf->SetMarkerSize(2.5);
    RpA_unf->Sumw2();


    dummyRpA->Draw("axis");
    RpA_unf->Draw("p,e1,same");
    l1->Draw("same");

    drawLatexAdd("#sqrt{#it{s}_{NN}} = 8[.16] TeV",0.11,0.275, textSize,kFALSE, kFALSE, false);
    drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.11,0.23, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Unfolded",0.11,0.19, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Statistical Errors Only",0.11,0.15, textSize,kFALSE, kFALSE, false);

    drawLatexAdd("Corrected for trigger eff and vtx finding eff",0.97,0.35, textSize,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Not scaled for #sqrt{#it{s}} difference",0.97,0.27, textSize,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Triggers combined using RFs",0.97,0.23, textSize,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Turnovers not yet optimized for pPb",0.97,0.19, textSize,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Scaled by x-secs and 1/208",0.97,0.15, textSize,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/RpA_unfolded_R0%i.%s",output.Data(),radius,fileType.Data()));

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Draw the raw RpA /////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // Scaling
    RpA_raw->Scale(1./208.);

    // Set up histogram
    RpA_raw->GetXaxis()->SetRangeUser(20,240);
    RpA_raw->SetMarkerStyle(4);
    RpA_raw->SetLineColor(kBlack);
    RpA_raw->SetMarkerColor(kBlack);
    RpA_raw->SetMarkerSize(2.5);
    RpA_raw->Sumw2();

    dummyRpA->Draw("axis");
    RpA_raw->Draw("p,e1,same");
    l1->Draw("same");

    drawLatexAdd("#sqrt{#it{s}_{NN}} = 8[.16] TeV",0.11,0.275, textSize,kFALSE, kFALSE, false);
    drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.11,0.23, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Raw (No Unfolding)",0.11,0.19, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Statistical Errors Only",0.11,0.15, textSize,kFALSE, kFALSE, false);

    drawLatexAdd("Not scaled for #sqrt{#it{s}} difference",0.97,0.27, textSize,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Triggers combined using RFs",0.97,0.23, textSize,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Turnovers not yet optimized for pPb",0.97,0.19, textSize,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Scaled by x-secs and 1/208",0.97,0.15, textSize,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/RpA_raw_R0%i.%s",output.Data(),radius,fileType.Data()));

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Draw the pA Fold/Raw /////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    pp_foldraw->GetXaxis()->SetRangeUser(20,240);
    pp_foldraw->SetMarkerStyle(4);
    pp_foldraw->SetLineColor(kBlack);
    pp_foldraw->SetMarkerColor(kBlack);
    pp_foldraw->SetMarkerSize(2);
    pp_foldraw->Sumw2();
    legendFoldRaw->AddEntry(pp_foldraw,"pp","p");

    pA_foldraw->GetXaxis()->SetRangeUser(20,240);
    pA_foldraw->SetMarkerStyle(25);
    pA_foldraw->SetLineColor(kRed+2);
    pA_foldraw->SetMarkerColor(kRed+2);
    pA_foldraw->SetMarkerSize(1.7);
    pA_foldraw->Sumw2();
    legendFoldRaw->AddEntry(pA_foldraw,"p-Pb","p");

    dummyFoldRaw->Draw("axis");
    pp_foldraw->Draw("p,e1,same");
    pA_foldraw->Draw("p,e1,same");
    l1->Draw("same");
    legendFoldRaw->Draw("same");

    drawLatexAdd("#sqrt{#it{s}_{NN}} = 8[.16] TeV",0.11,0.92, textSize,kFALSE, kFALSE, false);
    drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.11,0.875, textSize,kFALSE, kFALSE, false);
    drawLatexAdd(Form("(%s Unfolding, #it{Reg} = %i) / Raw",type.Data(),regnum),0.11,0.83, textSize,kFALSE, kFALSE, false);

    c->SaveAs(Form("%s/FoldRaw_R0%i.%s",output.Data(),radius,fileType.Data()));

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Draw the pp and pA normUnfolded spectra together /////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    c->SetLogy(1);
    TLegend *legendNormUnfolded =  GetAndSetLegend2(0.87,0.79-(2)*textSize,0.97,0.79,textSize);
    TH1D *dummyNormUnfolded = new TH1D("dummyNormUnfolded","",350,0,350);
    dummyNormUnfolded->GetXaxis()->SetRangeUser(0,260);
    dummyNormUnfolded->GetYaxis()->SetRangeUser(1e-8,1e-2);
    SetStyleHistoTH1ForGraphs(dummyNormUnfolded,"","#it{p}_{T} (GeV/c)","#it{R}_{pPb}",textSize,textSize*(4/3),textSize,textSize*(4/3),1.1,0.9);

    TH1D *pp_normUnfolded = (TH1D*) normUnfolded_pp->Clone("pp_normUnfolded");
    TH1D *pA_normUnfolded = (TH1D*) normUnfolded_pA->Clone("pA_normUnfolded");

    // Scaling
    pA_normUnfolded->Scale(1./208.);

    // Set up histogram
    pp_normUnfolded->GetXaxis()->SetRangeUser(20,240);
    pp_normUnfolded->SetMarkerStyle(4);
    pp_normUnfolded->SetLineColor(kBlack);
    pp_normUnfolded->SetMarkerColor(kBlack);
    pp_normUnfolded->SetMarkerSize(2);
    pp_normUnfolded->Sumw2();
    legendNormUnfolded->AddEntry(pp_normUnfolded,"pp","p");

    pA_normUnfolded->GetXaxis()->SetRangeUser(20,240);
    pA_normUnfolded->SetMarkerStyle(25);
    pA_normUnfolded->SetLineColor(kRed+2);
    pA_normUnfolded->SetMarkerColor(kRed+2);
    pA_normUnfolded->SetMarkerSize(1.7);
    pA_normUnfolded->Sumw2();
    legendNormUnfolded->AddEntry(pA_normUnfolded,"p-Pb","p");

    dummyNormUnfolded->Draw("axis");
    pp_normUnfolded->Draw("p,e1,same");
    pA_normUnfolded->Draw("p,e1,same");
    legendNormUnfolded->Draw("same");

    drawLatexAdd("#sqrt{#it{s}_{NN}} = 8[.16] TeV",0.96,0.92, textSize,kFALSE, kFALSE, true);
    drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.96,0.875, textSize,kFALSE, kFALSE, true);
    drawLatexAdd(Form("%s Unfolding, #it{Reg} = %i",type.Data(),regnum),0.96,0.83, textSize,kFALSE, kFALSE, true);

    c->SaveAs(Form("%s/NormUnfolded_R0%i.%s",output.Data(),radius,fileType.Data()));

   
}
