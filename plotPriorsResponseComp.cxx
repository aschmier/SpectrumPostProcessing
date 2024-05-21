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

void plotPriorsResponseComp(TString defaultfile, TString priorsfile, TString type, TString output, TString fileType)
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 6;
    int reg;
    if(type == "Bayes") reg = 4;
    if(type == "SVD")   reg = 6;

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    vector<TH2D*> vecDefault;
    vector<TH2D*> vecPriors;

    TFile *fDefault = TFile::Open(defaultfile);
    if(!fDefault || fDefault->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    TFile *fPriors = TFile::Open(priorsfile);
    if(!fPriors || fPriors->IsZombie()){
        cout << "Priors file not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *radDef = (TDirectory*)fDefault->Get(Form("R0%i",radius));
        TDirectory *resDef = (TDirectory*)radDef->Get("response");
        TH2D *responseDef  = (TH2D*)resDef->Get(Form("Rawresponse_R0%i_fine_rebinned_standard",radius));
        vecDefault.push_back(responseDef);

        TDirectory *radPri = (TDirectory*)fDefault->Get(Form("R0%i",radius));
        TDirectory *resPri = (TDirectory*)radPri->Get("response");
        TH2D *responsePri  = (TH2D*)resPri->Get(Form("Rawresponse_R0%i_fine_rebinned_standard",radius));
        vecPriors.push_back(responsePri);
    }

    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogz();

    for(int radius = minradius; radius <= maxradius; radius++){
        TH2D *dummy = (TH2D*)vecPriors.at(radius-minradius)->Clone(Form("dummy_R0%i",radius));
        dummy->Divide( dummy, vecDefault.at(radius-minradius), 1, 1, "b" );
        //dummy->GetXaxis()->SetRangeUser(0,200);
        //dummy->GetYaxis()->SetRangeUser(0,300);
        SetStyleHistoTH2ForGraphs(dummy,"#it{p}_{T}^{det} (GeV/#it{c})","#it{p}_{T}^{part}",textSize,0.04,textSize,0.04,1,1.2);
        dummy->Draw("colz");

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("%s Unfolding",type.Data()),0.93,0.82, textSize,kFALSE, kFALSE, true);

        c->SaveAs(Form("%s/RatioResponse_%s_R0%i.%s",output.Data(),type.Data(),radius,fileType.Data()));
    }

}
