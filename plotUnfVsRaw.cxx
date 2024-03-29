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


void plotUnfVsRaw(TString unfoldingFile, TString type, TString output, TString fileType)
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 6;
    vector<double> partlevelbin = getJetPtBinningNonLinTruePoor();


    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    vector<TH1D*> vecSpectra[maxradius-minradius+1];
    TH1D *specRaw[maxradius-minradius+1];

    TFile *f = TFile::Open(unfoldingFile);
    if(!f || f->IsZombie()){
        cout << "File not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *baseRad       = (TDirectory*)f->Get(Form("R0%i",radius));
        TDirectory *baseRaw       = (TDirectory*)baseRad->Get("rawlevel");
        TH1D *specRaw_fine        = (TH1D*)baseRaw->Get(Form("hraw_fine_R0%i",radius));
        specRaw[radius-minradius] = (TH1D*)specRaw_fine->Rebin(partlevelbin.size()-1, "specRaw", partlevelbin.data());
        specRaw[radius-minradius]->Scale(1.,"width");
        for(int reg = 1; reg <= 10; reg++){
            TDirectory *baseReg   = (TDirectory*)baseRad->Get(Form("reg%i",reg));
            TH1D *specUnf         = (TH1D*)baseReg->Get(Form("normalized_reg%i",reg));
            vecSpectra[radius-minradius].push_back(specUnf);
        }
    }

    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

    TLegend *legend =  GetAndSetLegend2(0.15,0.75,0.55,0.75+(10/2)*textSize,textSize,2);

    for(int radius = minradius; radius <= maxradius; radius++){
        legend->Clear();
        for(int reg = 0; reg < 9; reg++){
            vecSpectra[radius-minradius].at(reg)->Divide( vecSpectra[radius-minradius].at(reg), specRaw[radius-minradius], 1, 1, "b" );
            vecSpectra[radius-minradius].at(reg)->GetXaxis()->SetRangeUser(20,320);
            //vecSpectra[radius-minradius].at(reg)->GetYaxis()->SetRangeUser(0.1,1.2);
            SetStyleHistoTH1ForGraphs(vecSpectra[radius-minradius].at(reg),"","#it{p}_{T} (GeV/#it{c})","#it{reg}=#it{x} / raw",textSize,0.04,textSize,0.04,1,1.2);
            vecSpectra[radius-minradius].at(reg)->SetFillColor(colors[reg]);
            vecSpectra[radius-minradius].at(reg)->SetMarkerStyle(styles[reg]);
            vecSpectra[radius-minradius].at(reg)->SetMarkerSize(2.5);
            vecSpectra[radius-minradius].at(reg)->SetMarkerColor(colors[reg]);
            vecSpectra[radius-minradius].at(reg)->SetLineColor(colors[reg]);
            legend->AddEntry(vecSpectra[radius-minradius].at(reg), Form("#it{reg} %i / raw",reg), "p");
            if(reg == 1){
                vecSpectra[radius-minradius].at(reg)->Draw("p,e");
            }else{
                vecSpectra[radius-minradius].at(reg)->Draw("p,e,same");
            }
        }
        legend->Draw();

        //DrawGammaLines(20.,300.,1.,1.,8.,16,9);
        //DrawGammaLines(20.,300.,1.05,1.05,6.,16,8);
        //DrawGammaLines(20.,300.,0.95,0.95,6.,16,8);

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("%s Unfolding",type.Data()),0.93,0.82, textSize,kFALSE, kFALSE, true);

        c->SaveAs(Form("%s/UnfoldingComparisons/UnfoldedVsRaw/UnfVsRaw_%s_R0%i.%s",output.Data(),type.Data(),radius,fileType.Data()));
    }

}
