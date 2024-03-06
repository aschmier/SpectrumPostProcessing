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

void plotTriggerEfficiencies(TString resultsFile, TString output, TString fileType, TString system, int rmin = 2, int rmax = 6, int regnum = 6)
{
    // Define variables
    double textSize     = 0.03;
    TString jetType       = "Full";
    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    // Open unfolding results file
    TFile *f = TFile::Open(resultsFile.Data());
    if(!f || f->IsZombie()) return;

    gSystem->Exec("mkdir -p "+output+"/TriggerEfficiency");


    // Set up canvas for plotting
    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    double leftMargin         = 0.09;
    double rightMargin        = 0.025;
    double topMargin          = 0.025;
    double bottomMargin       = 0.1;
    DrawPaperCanvasSettings(c,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend = GetAndSetLegend2(0.39,0.81,0.59,0.81+((rmax-rmin+1)*textSize*1.5)/2,textSize,2);

    TLine * l = new TLine (20,1,240,1);
    l->SetLineColor(14);
    l->SetLineWidth(3);
    l->SetLineStyle(7);

    for(int r=rmin; r<=rmax; r++){
        // Get the histograms...
        TDirectory *rDir = (TDirectory*)f->Get(Form("R0%i",r));
        TDirectory *raw  = (TDirectory*)rDir->Get("rawlevel");
        TH1D *trigEffL0;
        if(system=="pp") trigEffL0 = (TH1D*)raw->Get(Form("TriggerEfficiency_EMC7_R0%i_rebinned",r));
        if(system=="pPb") trigEffL0 = (TH1D*)raw->Get(Form("TriggerEfficiency_EJ2_R0%i_rebinned",r));
        trigEffL0->GetXaxis()->SetRangeUser(20,240);
        trigEffL0->GetYaxis()->SetRangeUser(0,1.41);
        if(system=="pp") SetStyleHistoTH1ForGraphs(trigEffL0,"","p_{T}^{jet} [GeV/c]","EMC7 Trigger Efficiency",0.03,0.04,0.03,0.04,1,0.9);
        if(system=="pPb") SetStyleHistoTH1ForGraphs(trigEffL0,"","p_{T}^{jet} [GeV/c]","EJ2 Trigger Efficiency",0.03,0.04,0.03,0.04,1,0.9);
        trigEffL0->SetMarkerColor(colors[r-rmin]);
        trigEffL0->SetLineColor(colors[r-rmin]);
        trigEffL0->SetMarkerStyle(styles[r-rmin]);
        legend->AddEntry(trigEffL0, Form("#it{R}=0.%i",r), "p");
        if(r==rmin) trigEffL0->Draw("p,e");
        else trigEffL0->Draw("p,e,same");
    }
    legend->Draw();
    l->Draw("same");
    if(system=="pp") drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.13,0.89, textSize,kFALSE, kFALSE, false);
    if(system=="pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.13,0.89, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.13,0.85, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("PYTHIA8+GEANT3",0.13,0.81, textSize,kFALSE, kFALSE, false);
    if(system=="pp") c->SaveAs(Form("%s/TriggerEfficiency/hEfficiency_EMC7.%s",output.Data(),fileType.Data()));
    if(system=="pPb") c->SaveAs(Form("%s/TriggerEfficiency/hEfficiency_EJ2.%s",output.Data(),fileType.Data()));

    legend->Clear();

    for(int r=rmin; r<=rmax; r++){
        // Get the histograms...
        TDirectory *rDir = (TDirectory*)f->Get(Form("R0%i",r));
        TDirectory *raw  = (TDirectory*)rDir->Get("rawlevel");
        TH1D *trigEffL1;
        if(system=="pp") trigEffL1 = (TH1D*)raw->Get(Form("TriggerEfficiency_EJE_R0%i_rebinned",r));
        if(system=="pPb") trigEffL1 = (TH1D*)raw->Get(Form("TriggerEfficiency_EJ1_R0%i_rebinned",r));

        trigEffL1->GetXaxis()->SetRangeUser(20,240);
        trigEffL1->GetYaxis()->SetRangeUser(0,1.41);
        if(system=="pp") SetStyleHistoTH1ForGraphs(trigEffL1,"","p_{T}^{jet} [GeV/c]","EJE Trigger Efficiency",0.03,0.04,0.03,0.04,1,0.9);
        if(system=="pPb") SetStyleHistoTH1ForGraphs(trigEffL1,"","p_{T}^{jet} [GeV/c]","EJ1 Trigger Efficiency",0.03,0.04,0.03,0.04,1,0.9);
        trigEffL1->SetMarkerColor(colors[r-rmin]);
        trigEffL1->SetLineColor(colors[r-rmin]);
        trigEffL1->SetMarkerStyle(styles[r-rmin]);
        legend->AddEntry(trigEffL1, Form("#it{R}=0.%i",r), "p");
        if(r==rmin) trigEffL1->Draw("p,e");
        else trigEffL1->Draw("p,e,same");
    }
    legend->Draw();
    l->Draw("same");
    if(system=="pp") drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.13,0.89, textSize,kFALSE, kFALSE, false);
    if(system=="pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.13,0.89, textSize,kFALSE, kFALSE, false);

    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.13,0.85, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("PYTHIA8+GEANT3",0.13,0.81, textSize,kFALSE, kFALSE, false);
    if(system=="pp") c->SaveAs(Form("%s/TriggerEfficiency/hEfficiency_EJE.%s",output.Data(),fileType.Data()));
    if(system=="pPb") c->SaveAs(Form("%s/TriggerEfficiency/hEfficiency_EJ1.%s",output.Data(),fileType.Data()));

}
