#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"

void plotNConst(TString ppIn, TString pPbNRIn, TString pPbWRIn, TString outputdir, TString suffix, int radius, TString charge)
{
    double textSize  = 0.03;
    int minradius    = 2;
    int maxradius    = 6;
    TString jetType  = "Full";
    TString fileType = "pdf";
    int ptbins[17] = {0,10,20,30,40,50,60,80,100,120,
                      140,160,180,200,240,280,320};
    int projbins[8] = {0,5,10,20,30,40,60,100};

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<TH2D*> vecConst[3];
    vector<TString> vecFiles;
    vecFiles.push_back(ppIn);
    vecFiles.push_back(pPbNRIn);
    vecFiles.push_back(pPbWRIn);
    TString systems[3] = {"pp", "pPb No Rejection", "pPb With Rejection"};
    TString systems_short[3] = {"pp", "pPbNR", "pPbWR"};
    TString directories[3] = {"pp","NoRejection","WithRejection"};
    double nEvt[3];

    TCanvas* canvas       = new TCanvas("canvas","",10,10,500,500);
    double leftMargin   = 0.12;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    for(int nfile = 0; nfile < vecFiles.size(); nfile++){
        TFile *f = TFile::Open(vecFiles.at(nfile));
        if(!f || f->IsZombie()){
            cout << "File not found!" << endl;
            return;
        }

        TString dirnameEvt     = Form("JetSpectrum_%sJets_R0%i_INT7_%s", jetType.Data(), radius, suffix.Data());
        TString listnameEvt     = Form("JetSpectrum_%sJets_R0%i_INT7_%s", jetType.Data(), radius, suffix.Data());
        TDirectory *dirEvt = (TDirectory*)f->Get(dirnameEvt.Data());
        TList *listEvt     = (TList*)dirEvt->Get(listnameEvt.Data());
        TH1D *events    = (TH1D*)listEvt->FindObject("hClusterCounter");
        nEvt[nfile] = events->GetBinContent(1);


        TString dirname     = Form("EnergyScaleResults_%sJet_R0%i_INT7_%s", jetType.Data(), radius, suffix.Data());
        TString listname     = Form("EnergyScaleHists_%sJet_R0%i_INT7_%s", jetType.Data(), radius, suffix.Data());

        TDirectory *dir = (TDirectory*)f->Get(dirname.Data());
        TList *list     = (TList*)dir->Get(listname.Data());
        for(int ptbin = 0; ptbin < 16; ptbin++){
            TString histname;
            if(charge == "All") histname = Form("hCompNConst_%i_%i",ptbins[ptbin],ptbins[ptbin+1]);
            else if(charge == "Charged") histname = Form("hCompNCharged_%i_%i",ptbins[ptbin],ptbins[ptbin+1]);
            else if(charge == "Neutral") histname = Form("hCompNNeutral_%i_%i",ptbins[ptbin],ptbins[ptbin+1]);
            else cout << "Incorrect particle type specified! Choose all, charged, or neutral." << endl;

            TH2D *hConst    = (TH2D*)list->FindObject(histname.Data());
            vecConst[nfile].push_back(hConst);
        }

        TLine *l = new TLine(0,0,100,100);
        l->SetLineColor(14);
        l->SetLineWidth(3);
        l->SetLineStyle(7);

        for(int ptbin = 0; ptbin < 16; ptbin++){
            vecConst[nfile].at(ptbin)->GetXaxis()->SetRangeUser(0,100);
            vecConst[nfile].at(ptbin)->GetYaxis()->SetRangeUser(0,100);
            SetStyleHistoTH2ForGraphs(vecConst[nfile].at(ptbin),Form("N Const %s Particle Level",charge.Data()),Form("N Const %s Detector Level",charge.Data()),0.03,0.04,0.03,0.04,1,1.1);
            vecConst[nfile].at(ptbin)->Draw("colz");
            l->Draw("same");
            drawLatexAdd(Form("System: %s",systems[nfile].Data()),0.15,0.90, textSize,kFALSE, kFALSE, false);
            canvas->SaveAs(Form("%s/%s/NConst_%s_%s_R0%i_%i_%i.%s", outputdir.Data(),directories[nfile].Data(), charge.Data(), systems_short[nfile].Data(), radius, ptbins[ptbin], ptbins[ptbin+1], fileType.Data()));
        }
    }

    TLegend *legend =  GetAndSetLegend2(0.75,(0.8-(3)*textSize),0.85,0.8,textSize,1);

    // y-projections
    for(int ptbin = 0; ptbin < 16; ptbin++){
        for(int projbin = 0; projbin < 7; projbin++){
            for(int nfile = 0; nfile < vecFiles.size(); nfile++){
                TH1D *proj = vecConst[nfile].at(ptbin)->ProjectionY(Form("projectiony_%i_%i_%i",ptbin,projbin,nfile),projbins[projbin]+1,projbins[projbin+1]+1);
                proj->SetLineColor(colors[nfile]);
                proj->SetMarkerColor(colors[nfile]);
                proj->SetMarkerStyle(styles[nfile]);
                proj->GetXaxis()->SetRangeUser(0,100);
                proj->Scale(1/nEvt[nfile]);
                SetStyleHistoTH1ForGraphs(proj,"","N Const Detector Level","counts",0.03,0.04,0.03,0.04,1,1.1);
                if(nfile==0) proj->Draw("p,e");
                else proj->Draw("p,e,same");
                legend->AddEntry(proj, directories[nfile], "p");
            }
            legend->Draw();
            drawLatexAdd(Form("%i < p_{T}^{part} < %i GeV",ptbins[ptbin],ptbins[ptbin+1]),0.93,0.90, textSize,kFALSE, kFALSE, true);
            drawLatexAdd(Form("%i < N_{const}^{part} < %i",projbins[projbin],projbins[projbin+1]),0.93,0.84, textSize,kFALSE, kFALSE, true);
            //drawLatexAdd("ALICE Data",0.93,0.82, textSize,kFALSE, kFALSE, true);
            canvas->SaveAs(Form("%s/Projections/Y/NConstProjectionY_R0%i_pt%i-%i_constpart%i-%i.%s", outputdir.Data(), radius, ptbins[ptbin], ptbins[ptbin+1], projbins[projbin], projbins[projbin+1], fileType.Data()));
            legend->Clear();
        }
    }

    // x-projections
    for(int ptbin = 0; ptbin < 16; ptbin++){
        for(int projbin = 0; projbin < 7; projbin++){
            for(int nfile = 0; nfile < vecFiles.size(); nfile++){
                TH1D *proj = vecConst[nfile].at(ptbin)->ProjectionX(Form("projectionx_%i_%i_%i",ptbin,projbin,nfile),projbins[projbin]+1,projbins[projbin+1]+1);
                proj->SetLineColor(colors[nfile]);
                proj->SetMarkerColor(colors[nfile]);
                proj->SetMarkerStyle(styles[nfile]);
                proj->GetXaxis()->SetRangeUser(0,100);
                proj->Scale(1/nEvt[nfile]);
                SetStyleHistoTH1ForGraphs(proj,"","N Const Particle Level","counts",0.03,0.04,0.03,0.04,1,1.1);
                if(nfile==0) proj->Draw("p,e");
                else proj->Draw("p,e,same");
                legend->AddEntry(proj, directories[nfile], "p");
            }
            legend->Draw();
            drawLatexAdd(Form("%i < p_{T}^{part} < %i GeV",ptbins[ptbin],ptbins[ptbin+1]),0.93,0.90, textSize,kFALSE, kFALSE, true);
            drawLatexAdd(Form("%i < N_{const}^{det} < %i",projbins[projbin],projbins[projbin+1]),0.93,0.84, textSize,kFALSE, kFALSE, true);
            //drawLatexAdd("ALICE Data",0.93,0.82, textSize,kFALSE, kFALSE, true);
            canvas->SaveAs(Form("%s/Projections/X/NConstProjectionX_R0%i_pt%i-%i_constpart%i-%i.%s", outputdir.Data(), radius, ptbins[ptbin], ptbins[ptbin+1], projbins[projbin], projbins[projbin+1], fileType.Data()));
            legend->Clear();
        }
    }

}
