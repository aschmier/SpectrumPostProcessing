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
    Int_t regnum          = 7;
    TString baseDirectory = "/home/austin/alice/RpA_results";
    TString fileType      = "png";

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    vector<double> partlevelbin = getJetPtBinningNonLinTruePoor();
    double xsec_pp = 55.8;
    double xsec_pPb = 2095.;

    // Open unfolding results file
    TFile *fpp = TFile::Open(ppFile);
    if(!fpp || fpp->IsZombie()) return;

    TFile *fpA = TFile::Open(pAFile);
    if(!fpA || fpA->IsZombie()) return;

    // Set up canvas for plotting
    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

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
    RpA_unf->Divide(normUnfolded_pA,normUnfolded_pp,1,1,"b");

    TH1D *RpA_raw = (TH1D*)raw_rebin_pp->Clone("RpA_raw");
    RpA_raw->Divide(raw_rebin_pA,raw_rebin_pp);

    TH1D *pp_foldraw = (TH1D*)raw_rebin_pp->Clone("pp_foldraw");
    pp_foldraw->Divide(normUnfolded_pp,raw_rebin_pp);

    TH1D *pA_foldraw = (TH1D*)raw_rebin_pA->Clone("pA_foldraw");
    pA_foldraw->Divide(normUnfolded_pA,raw_rebin_pA);

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Draw the unfolded RpA ////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // Scaling
    RpA_unf->Scale(1./208.);

    // Set up histogram
    RpA_unf->GetXaxis()->SetRangeUser(20,320);
    SetStyleHistoTH1ForGraphs(RpA_unf,"","#it{p}_{T} (GeV/c)","#it{R}_{pPb}",0.03,0.04,0.03,0.04,1,1.2);
    RpA_unf->SetMarkerStyle(4);
    RpA_unf->SetLineColor(kBlack);
    RpA_unf->SetMarkerColor(kBlack);
    RpA_unf->SetMarkerSize(2.5);
    RpA_unf->Draw();

    TLine * l1 = new TLine (0,1,280,1);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);
    l1->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("%s Unfolding, #it{Reg} = %i",type.Data(),regnum),0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/RpA_unfolded_R0%i.%s",output.Data(),radius,fileType.Data()));

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Draw the raw RpA /////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // Scaling
    RpA_raw->Scale(1./208.);

    // Set up histogram
    RpA_raw->GetXaxis()->SetRangeUser(20,320);
    SetStyleHistoTH1ForGraphs(RpA_raw,"","p_{T} (GeV/c)","#it{R}_{pPb}",0.03,0.04,0.03,0.04,1,1.2);
    RpA_raw->SetMarkerStyle(4);
    RpA_raw->SetLineColor(kBlack);
    RpA_raw->SetMarkerColor(kBlack);
    RpA_raw->SetMarkerSize(2.5);
    RpA_raw->Sumw2();
    RpA_raw->Draw();

    TLine * l2 = new TLine (20,1,320,1);
    l2->SetLineColor(14);
    l2->SetLineWidth(3);
    l2->SetLineStyle(7);
    l2->Draw("same");


    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Raw (No Unfolding)",0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Triggers combined using RFs",0.95,0.79, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("(Not yet optimized for pPb)",0.95,0.75, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Scaled by x-secs and 1/208",0.95,0.71, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/RpA_raw_R0%i.%s",output.Data(),radius,fileType.Data()));

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Draw the pp Fold/Raw /////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // Set up histogram
    pp_foldraw->GetXaxis()->SetRangeUser(20,320);
    SetStyleHistoTH1ForGraphs(pp_foldraw,"","p_{T} (GeV/c)","Fold/Raw (pp)",0.03,0.04,0.03,0.04,1,1.2);
    pp_foldraw->SetMarkerStyle(4);
    pp_foldraw->SetLineColor(kBlack);
    pp_foldraw->SetMarkerColor(kBlack);
    pp_foldraw->SetMarkerSize(2.5);
    pp_foldraw->Sumw2();
    pp_foldraw->Draw();

    TLine * l3 = new TLine (0,1,280,1);
    l3->SetLineColor(14);
    l3->SetLineWidth(3);
    l3->SetLineStyle(7);
    l3->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("(%s Unfolding, #it{Reg} = %i) / Raw",type.Data(),regnum),0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/FoldRaw_pp_R0%i.%s",output.Data(),radius,fileType.Data()));

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Draw the pA Fold/Raw /////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    // Set up histogram
    pA_foldraw->GetXaxis()->SetRangeUser(20,320);
    SetStyleHistoTH1ForGraphs(pA_foldraw,"","p_{T} (GeV/c)","Fold/Raw (pPb)",0.03,0.04,0.03,0.04,1,1.2);
    pA_foldraw->SetMarkerStyle(4);
    pA_foldraw->SetLineColor(kBlack);
    pA_foldraw->SetMarkerColor(kBlack);
    pA_foldraw->SetMarkerSize(2.5);
    pA_foldraw->Sumw2();
    pA_foldraw->Draw();

    TLine * l4 = new TLine (0,1,280,1);
    l4->SetLineColor(14);
    l4->SetLineWidth(3);
    l4->SetLineStyle(7);
    l4->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("(%s Unfolding, #it{Reg} = %i) / Raw",type.Data(),regnum),0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/FoldRaw_pA_R0%i.%s",output.Data(),radius,fileType.Data()));
}
