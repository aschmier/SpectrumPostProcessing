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

void plotFilterComparison(TString defaultfile, TString filteredfile, TString output)
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 6;
    TString filetype = "pdf";

    gSystem->Exec("mkdir -p "+output);

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

    for(int radius = minradius; radius <= maxradius; radius++){
        TFile *fdefault = TFile::Open(defaultfile);
        if(!fdefault || fdefault->IsZombie()){
            cout << "Default file not found!" << endl;
            return;
        }

        TFile *ffiltered = TFile::Open(filteredfile);
        if(!ffiltered || ffiltered->IsZombie()){
            cout << "Filtered file not found!" << endl;
            return;
        }

        TDirectory *baseRadDef     = (TDirectory*)fdefault->Get(Form("R0%i",radius));
        TDirectory *baseRegDef = (TDirectory*)baseRadDef->Get("reg6");
        TH1D *specUnfDef       = (TH1D*)baseRegDef->Get("normalized_reg6");

        TDirectory *baseRadFil     = (TDirectory*)ffiltered->Get(Form("R0%i",radius));
        TDirectory *baseRegFil = (TDirectory*)baseRadFil->Get("reg6");
        TH1D *specUnfFil       = (TH1D*)baseRegFil->Get("normalized_reg6");

        TH1D *ratio = (TH1D*)specUnfDef->Clone(Form("ratio_R0%i",radius));
        ratio->Divide(specUnfFil, specUnfDef, 1, 1, "b");
        TH1D *statUnc = (TH1D*)specUnfDef->Clone(Form("statUnc_R0%i",radius));
        for(int bin = 0; bin <= statUnc->GetNbinsX(); bin++){
            statUnc->SetBinError(bin,0);
        }
        statUnc->Divide(specUnfDef,statUnc);
        ratio->GetYaxis()->SetRangeUser(0.94,1.06);
        if(radius <= 4) ratio->GetXaxis()->SetRangeUser(20,240);
        else if(radius == 5) ratio->GetXaxis()->SetRangeUser(20,160);
        else if(radius == 6) ratio->GetXaxis()->SetRangeUser(20,130);
        else cout << "Invalid radius specified!" << endl;
        ratio->SetMarkerStyle(styles[radius-minradius]);
        ratio->SetMarkerSize(2.5);
        ratio->SetMarkerColor(colors[radius-minradius]);
        ratio->SetLineColor(colors[radius-minradius]);
        ratio->SetFillColor(colors[radius-minradius]);
        ratio->SetFillStyle(3001);
        SetStyleHistoTH1ForGraphs(ratio,"","#it{p}_{T} (GeV/#it{c})","Filtered/Unfiltered",textSize,0.04,textSize,0.04,1,1.2);
        ratio->Draw("p,e");
        statUnc->SetFillColor(colors[radius-minradius]);
        statUnc->SetLineColor(colors[radius-minradius]);
        statUnc->SetFillStyle(0);
        statUnc->Draw("e2,same");

        TLegend *legendErrorKey =  GetAndSetLegend2(0.15,0.15,0.35,0.15+((2)*textSize*1.7)/2,textSize);
        legendErrorKey->AddEntry(ratio, "Ratio", "p");
        legendErrorKey->AddEntry(statUnc, "Statistical Uncertainty", "f");
        legendErrorKey->Draw();

        DrawGammaLines(20.,240.,1.,1.,8.,16,9);

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.14,0.90, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.14,0.86, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("Filter: #it{N}_{jet} > 10/#it{bin}_{response}",0.14,0.82, textSize,kFALSE, kFALSE, false);

        c->SaveAs(Form("%s/filterComparison_R0%i.%s",output.Data(),radius,filetype.Data()));
    }
}
