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

void plotRawFromUnf(TString unfoldingFile, TString output, TString fileType, TString suffix)
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 6;
    vector<double> detlevelbin = getJetPtBinningNonLinSmear8TeV();


    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    vector<TH1D*> vecSpectra[maxradius-minradius+1];
    TString triggers[3] = {"INT7","EMC7","EJE"};


    TFile *f = TFile::Open(unfoldingFile);
    if(!f || f->IsZombie()){
        cout << "File not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int t = 0; t < 3; t++){
            TDirectory *baseRad  = (TDirectory*)f->Get(Form("R0%i",radius));
            TDirectory *rawlevel = (TDirectory*)baseRad->Get("rawlevel");
            TString dirname = Form("RawJetSpectrum_FullJets_R0%i_%s_ANY_%s", radius, triggers[t].Data(), suffix.Data());
            TH1D *spec           = (TH1D*)rawlevel->Get(dirname.Data());
            vecSpectra[radius-minradius].push_back(spec);
        }
    }

    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.75,0.75,0.95,0.75+(3)*textSize,textSize,1);

    // Plot spectra individually
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int t = 0; t < 3; t++){
            vecSpectra[radius-minradius].at(t)->GetXaxis()->SetRangeUser(10,240);
            SetStyleHistoTH1ForGraphs(vecSpectra[radius-minradius].at(t),"","#it{p}_{T} (GeV/#it{c})","(Abs. Counts) / (RF * Norm * TrigEff)",textSize,0.04,textSize,0.04,1,1.2);
            vecSpectra[radius-minradius].at(t)->SetFillColor(colors[t]);
            vecSpectra[radius-minradius].at(t)->SetMarkerStyle(styles[t]);
            vecSpectra[radius-minradius].at(t)->SetMarkerSize(1);
            vecSpectra[radius-minradius].at(t)->SetMarkerColor(colors[t]);
            vecSpectra[radius-minradius].at(t)->SetLineColor(colors[t]);
            vecSpectra[radius-minradius].at(t)->Draw("p,e");

            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
            drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
            drawLatexAdd(Form("Trigger: %s",triggers[t].Data()),0.93,0.82, textSize,kFALSE, kFALSE, true);


            c->SaveAs(Form("%s/RawFromUnf_%s_R0%i.%s",output.Data(),triggers[t].Data(),radius,fileType.Data()));
        }
    }

    // Overlay triggers
    for(int radius = minradius; radius <= maxradius; radius++){
        legend->Clear();
        for(int t = 0; t < 3; t++){
            TH1D *rebinSpectrum = (TH1D*)vecSpectra[radius-minradius].at(t)->Rebin(detlevelbin.size()-1, Form("rebinSpectrumHigh_R0%i_%s",radius,triggers[t].Data()), detlevelbin.data());

            vecSpectra[radius-minradius].at(t)->GetXaxis()->SetRangeUser(10,240);
            vecSpectra[radius-minradius].at(t)->GetYaxis()->SetRangeUser(1e-11,2e-4);
            SetStyleHistoTH1ForGraphs(vecSpectra[radius-minradius].at(t),"","#it{p}_{T} (GeV/#it{c})","(Abs. Counts) / (RF * Norm * TrigEff)",textSize,0.04,textSize,0.04,1,1.2);
            vecSpectra[radius-minradius].at(t)->SetFillColor(colors[t]);
            vecSpectra[radius-minradius].at(t)->SetMarkerStyle(styles[t]);
            vecSpectra[radius-minradius].at(t)->SetMarkerSize(1);
            vecSpectra[radius-minradius].at(t)->SetMarkerColor(colors[t]);
            vecSpectra[radius-minradius].at(t)->SetLineColor(colors[t]);
            legend->AddEntry(vecSpectra[radius-minradius].at(t), Form("%s",triggers[t].Data()), "p");
            if(t == 0){
                vecSpectra[radius-minradius].at(t)->Draw("p,e");
            }else{
                vecSpectra[radius-minradius].at(t)->Draw("p,e,same");
            }
        }
        legend->Draw();

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);

        c->SaveAs(Form("%s/RawFromUnfAll_R0%i.%s",output.Data(),radius,fileType.Data()));
    }

    // Overlay triggers Rebinned
    for(int radius = minradius; radius <= maxradius; radius++){
        legend->Clear();
        for(int t = 0; t < 3; t++){
            TH1D *rebinSpectrum = (TH1D*)vecSpectra[radius-minradius].at(t)->Rebin(detlevelbin.size()-1, Form("rebinSpectrum_R0%i_%s",radius,triggers[t].Data()), detlevelbin.data());
            rebinSpectrum->Scale(1.,"width");
            rebinSpectrum->GetXaxis()->SetRangeUser(10,240);
            rebinSpectrum->GetYaxis()->SetRangeUser(1e-11,2e-4);
            SetStyleHistoTH1ForGraphs(rebinSpectrum,"","#it{p}_{T} (GeV/#it{c})","(Abs. Counts) / (RF * Norm * TrigEff)",textSize,0.04,textSize,0.04,1,1.2);
            rebinSpectrum->SetFillColor(colors[t]);
            rebinSpectrum->SetMarkerStyle(styles[t]);
            rebinSpectrum->SetMarkerSize(1);
            rebinSpectrum->SetMarkerColor(colors[t]);
            rebinSpectrum->SetLineColor(colors[t]);
            legend->AddEntry(rebinSpectrum, Form("%s",triggers[t].Data()), "p");
            if(t == 0){
                rebinSpectrum->Draw("p,e");
            }else{
                rebinSpectrum->Draw("p,e,same");
            }
        }
        legend->Draw();

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);

        c->SaveAs(Form("%s/RawFromUnfAll_Rebinned_R0%i.%s",output.Data(),radius,fileType.Data()));
    }

    c->SetLogy(0);

    // Ratio of triggers, without rebin
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int t = 1; t < 3; t++){
            TH1D *ratio = (TH1D*)vecSpectra[radius-minradius].at(t)->Clone(Form("ratio_%s-%s", triggers[t].Data(), triggers[t-1].Data()));
            ratio->Divide(vecSpectra[radius-minradius].at(t),vecSpectra[radius-minradius].at(t-1));
            ratio->GetXaxis()->SetRangeUser(10,240);
            ratio->GetYaxis()->SetRangeUser(0.4,1.6);
            SetStyleHistoTH1ForGraphs(ratio,"","#it{p}_{T} (GeV/#it{c})",Form("%s / %s", triggers[t].Data(), triggers[t-1].Data()),textSize,0.04,textSize,0.04,1,1.2);
            ratio->SetFillColor(colors[t]);
            ratio->SetMarkerStyle(styles[t]);
            ratio->SetMarkerSize(1);
            ratio->SetMarkerColor(colors[t]);
            ratio->SetLineColor(colors[t]);
            ratio->Draw("p,e");

            TF1 *pol0 = new TF1("pol0","[0]",10,240);
            ratio->Fit(pol0, "QRMEX+", "", 20, 40);

            DrawGammaLines(10.,240.,1.,1.,8.,16,9);

            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
            drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
            drawLatexAdd("(Abs. Counts) / (RF * Norm * TrigEff)",0.93,0.82, textSize,kFALSE, kFALSE, true);

            c->SaveAs(Form("%s/ratio_%s-%s_R0%i.%s",output.Data(), triggers[t].Data(), triggers[t-1].Data(),radius,fileType.Data()));
        }
    }

    // Ratio of triggers, with rebin
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int t = 1; t < 3; t++){
            TH1D *rebinSpectrumHigh = (TH1D*)vecSpectra[radius-minradius].at(t)->Rebin(detlevelbin.size()-1, Form("rebinSpectrumHigh_R0%i_%s",radius,triggers[t].Data()), detlevelbin.data());
            TH1D *rebinSpectrumLow  = (TH1D*)vecSpectra[radius-minradius].at(t-1)->Rebin(detlevelbin.size()-1, Form("rebinSpectrumLow_R0%i_%s",radius,triggers[t-1].Data()), detlevelbin.data());
            rebinSpectrumHigh->Scale(1.,"width");
            rebinSpectrumLow->Scale(1.,"width");
            TH1D *ratio = (TH1D*)rebinSpectrumHigh->Clone(Form("ratioRebin_%s-%s", triggers[t].Data(), triggers[t-1].Data()));
            ratio->Divide(rebinSpectrumHigh,rebinSpectrumLow);
            ratio->GetXaxis()->SetRangeUser(10,240);
            ratio->GetYaxis()->SetRangeUser(0.4,1.6);
            SetStyleHistoTH1ForGraphs(ratio,"","#it{p}_{T} (GeV/#it{c})",Form("%s / %s", triggers[t].Data(), triggers[t-1].Data()),textSize,0.04,textSize,0.04,1,1.2);
            ratio->SetFillColor(colors[t]);
            ratio->SetMarkerStyle(styles[t]);
            ratio->SetMarkerSize(1);
            ratio->SetMarkerColor(colors[t]);
            ratio->SetLineColor(colors[t]);
            ratio->Draw("p,e");

            TF1 *pol0 = new TF1("pol0","[0]",10,240);
            if(t==1) ratio->Fit(pol0, "QRMEX+", "", 20, 40);
            if(t==2) ratio->Fit(pol0, "QRMEX+", "", 20, 40);

            DrawGammaLines(10.,240.,1.,1.,8.,16,9);

            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
            drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.93,0.86, textSize,kFALSE, kFALSE, true);
            drawLatexAdd("(Abs. Counts) / (RF * Norm * TrigEff)",0.93,0.82, textSize,kFALSE, kFALSE, true);

            c->SaveAs(Form("%s/ratioRebinned_%s-%s_R0%i.%s",output.Data(), triggers[t].Data(), triggers[t-1].Data(),radius,fileType.Data()));
        }
    }

}
