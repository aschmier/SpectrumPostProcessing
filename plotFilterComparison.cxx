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
    TString filetype = "png";

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
        ratio->GetYaxis()->SetRangeUser(0.98,1.02);
        ratio->GetXaxis()->SetRangeUser(20,240);
        ratio->Draw("p,e");

        DrawGammaLines(20.,240.,1.,1.,8.,16,9);

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Filtered/Unfiltered",0.93,0.82, textSize,kFALSE, kFALSE, true);

        c->SaveAs(Form("%s/filterComparison_R0%i.%s",output.Data(),radius,filetype.Data()));
    }
}
