#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"

void compareDpT(TString file1, TString file2, TString outputdir, TString fileType, TString suffix, int minradius, int maxradius)
{
    double textSize  = 0.03;
    TString jetType  = "Full";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    TString triggers[3] = {"INT7","EJ2","EJ1"};
    double nEvt[3];

    gSystem->Exec("mkdir -p "+outputdir);

    TFile *f1 = TFile::Open(file1.Data());
    if(!f1 || f1->IsZombie()){
        cout << "File 1 not found!" << endl;
        return;
    }

    TFile *f2 = TFile::Open(file2.Data());
    if(!f2 || f2->IsZombie()){
        cout << "File 2 not found!" << endl;
        return;
    }

    TCanvas* canvas       = new TCanvas("canvas", "", 1000, 800);
    double leftMargin   = 0.1;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    // Make dummy histogram to set the axes
    TH1D *dummy = new TH1D("dummy","",320,0,320);
    SetStyleHistoTH1ForGraphs(dummy,"","#it{p}_{T} {GeV/#it{c}}","Cross-Section Ratio",textSize,0.035,textSize,0.035,1,1.2);
    dummy->GetYaxis()->SetRangeUser(0.8,1.2);
    dummy->GetXaxis()->SetRangeUser(0.,260.);

    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *baseRad1     = (TDirectory*)f1->Get(Form("R0%i",radius));
        TDirectory *baseRad2     = (TDirectory*)f2->Get(Form("R0%i",radius));
        for(int reg = 6; reg <= 6; reg++){
            TDirectory *baseReg1 = (TDirectory*)baseRad1->Get(Form("reg%i",reg));
            TH1D *specUnf1       = (TH1D*)baseReg1->Get(Form("normalized_reg%i",reg));
            TDirectory *baseReg2 = (TDirectory*)baseRad2->Get(Form("reg%i",reg));
            TH1D *specUnf2       = (TH1D*)baseReg2->Get(Form("normalized_reg%i",reg));

            // Take the ratio of the two spectra
            TH1D *ratio = (TH1D*)specUnf1->Clone(Form("ratio_R0%i_reg%i",radius,reg));
            ratio->Divide(specUnf2);
            ratio->SetMarkerStyle(20);
            ratio->SetMarkerSize(1.5);
            ratio->SetMarkerColor(1);
            ratio->SetLineColor(1);
            ratio->GetXaxis()->SetRangeUser(20,240);

            dummy->Draw("axis");
            ratio->Draw("p,e,same");

            TLine *l80 = new TLine (0,1.,260,1.);
            l80->SetLineColor(14);
            l80->SetLineWidth(3);
            l80->SetLineStyle(7);
            l80->Draw("same");

            drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.14,0.9, textSize,kFALSE, kFALSE, false);
            drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.14,0.86, textSize,kFALSE, kFALSE, false);
            drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.14,0.82, textSize,kFALSE, kFALSE, false);
            drawLatexAdd(Form("R = 0.%i, Unfolding Iteration %i", radius, reg),0.14,0.78,textSize,kFALSE, kFALSE, false);
            drawLatexAdd("#delta#it{p}_{T} smeared using triggers INT7/EJ1",0.14,0.74,textSize,kFALSE, kFALSE, false);

            canvas->SaveAs(Form("%s/compareDpT_R0%i_reg%i_INT7-EJ1.%s",outputdir.Data(),radius,reg,fileType.Data()));
        }
    }

    f1->Close();
    f2->Close();
}
