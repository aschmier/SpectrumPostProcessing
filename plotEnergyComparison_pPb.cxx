#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TH2D.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"
#include "fstream"

void plotEnergyComparison_pPb(TString analysisresultspp, TString analysisresultspPb, TString output, TString fileType, int minradius, int maxradius)
{
    Double_t textSize     = 0.03;

    int scale[5] = {1,3,10,30,100};

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.1,1.,1.5,1.2,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    double alphasval = 0.25;

    Color_t colors[13]      = {kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    //Color_t colorsSys[14]   = {kGray, kRed-7, kYellow+6, kGreen-6, kCyan-6, kBlue-6, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    Color_t colorpp    = kGreen+2;
    Color_t colorpPb    = kOrange+1;

    vector<TGraphErrors*> vecSpectrumpp;
    vector<TGraphErrors*> vecSpectrumSyspp;
    vector<TGraphErrors*> vecRatiopp;
    vector<TGraphErrors*> vecRatioSyspp;

    vector<TGraphErrors*> vecSpectrumpPb;
    vector<TGraphErrors*> vecSpectrumSyspPb;
    vector<TGraphErrors*> vecRatiopPb;
    vector<TGraphErrors*> vecRatioSyspPb;

    gSystem->Exec("mkdir -p "+output+"/EnergyComparisons");

    // Open root files
    TFile *fresultspp = TFile::Open(analysisresultspp);
    if(!fresultspp || fresultspp->IsZombie()){
        cout << "pp file not found!" << endl;
        return;
    }

    TFile *fresultspPb = TFile::Open(analysisresultspPb);
    if(!fresultspPb || fresultspPb->IsZombie()){
        cout << "pPb file not found!" << endl;
        return;
    }

    // pp 8 TeV
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *rdir = (TDirectory*)fresultspp->Get(Form("R0%i",radius));
        TGraphErrors *spectrum     = (TGraphErrors*)rdir->Get(Form("finalSpectrum_R0%i",radius));
        TGraphErrors *spectrumSys  = (TGraphErrors*)rdir->Get(Form("finalSpectrumSys_R0%i",radius));

        for(int point = 0; point < spectrum->GetN(); point++){
            double staterror = spectrum->GetErrorY(point);
            spectrum->SetPointError(point,0,staterror);
            double syserror = spectrumSys->GetErrorY(point);
            spectrumSys->SetPointError(point,spectrumSys->GetErrorX(point),syserror);
        }

        spectrum->SetMarkerStyle(stylesfilled[radius-minradius]);
        spectrum->SetMarkerSize(sizesfilled[radius-minradius]);
        spectrum->SetMarkerColor(colorpp);
        spectrum->SetLineColor(colorpp);

        spectrumSys->SetFillColorAlpha(colorpp,alphasval);
        spectrumSys->SetLineColor(colorpp);
        spectrumSys->SetFillStyle(1);

        vecSpectrumpp.push_back(spectrum);
        vecSpectrumSyspp.push_back(spectrumSys);

        if(radius != minradius){
            TGraphErrors *ratio        = (TGraphErrors*)rdir->Get(Form("finalRatio_R0%i",radius));
            TGraphErrors *ratioSys     = (TGraphErrors*)rdir->Get(Form("finalRatioSys_R0%i",radius));

            for(int point = 0; point < ratio->GetN(); point++){
                double staterror = ratio->GetErrorY(point);
                ratio->SetPointError(point,0,staterror);
                double syserror = ratioSys->GetErrorY(point);
                ratioSys->SetPointError(point,ratioSys->GetErrorX(point),syserror);
            }

            ratio->SetMarkerStyle(stylesfilled[radius-(minradius)]);
            ratio->SetMarkerSize(sizesfilled[radius-(minradius)]);
            ratio->SetMarkerColor(colorpp);
            ratio->SetLineColor(colorpp);

            ratioSys->SetLineColor(colorpp);
            ratioSys->SetFillColorAlpha(colorpp,alphasval);
            ratioSys->SetFillStyle(1);

            vecRatiopp.push_back(ratio);
            vecRatioSyspp.push_back(ratioSys);
        }
    }

    // pPb 8.16 TeV
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *rdir = (TDirectory*)fresultspPb->Get(Form("R0%i",radius));
        if(!rdir) cout << "R0" << radius << " not found!" << endl;
        TGraphErrors *spectrum     = (TGraphErrors*)rdir->Get(Form("finalSpectrum_R0%i",radius));
        TGraphErrors *spectrumSys  = (TGraphErrors*)rdir->Get(Form("finalSpectrumSys_R0%i",radius));

        for(int point = 0; point < spectrum->GetN(); point++){
            double staterror = spectrum->GetErrorY(point);
            spectrum->SetPointError(point,0,staterror);
            double syserror = spectrumSys->GetErrorY(point);
            spectrumSys->SetPointError(point,spectrumSys->GetErrorX(point),syserror);
        }

        spectrum->SetMarkerStyle(stylesfilled[radius-minradius]);
        spectrum->SetMarkerSize(sizesfilled[radius-minradius]);
        spectrum->SetMarkerColor(colorpPb);
        spectrum->SetLineColor(colorpPb);

        spectrumSys->SetFillColorAlpha(colorpPb,alphasval);
        spectrumSys->SetLineColor(colorpPb);
        spectrumSys->SetFillStyle(1);

        spectrum->Scale(1./208.);
        spectrumSys->Scale(1./208.);

        vecSpectrumpPb.push_back(spectrum);
        vecSpectrumSyspPb.push_back(spectrumSys);

        if(radius != minradius){
            TGraphErrors *ratio        = (TGraphErrors*)rdir->Get(Form("finalRatio_R0%i",radius));
            TGraphErrors *ratioSys     = (TGraphErrors*)rdir->Get(Form("finalRatioSys_R0%i",radius));

            for(int point = 0; point < ratio->GetN(); point++){
                double staterror = ratio->GetErrorY(point);
                ratio->SetPointError(point,0,staterror);
                double syserror = ratioSys->GetErrorY(point);
                ratioSys->SetPointError(point,ratioSys->GetErrorX(point),syserror);
            }

            ratio->SetMarkerStyle(stylesfilled[radius-(minradius)]);
            ratio->SetMarkerSize(sizesfilled[radius-(minradius)]);
            ratio->SetMarkerColor(colorpPb);
            ratio->SetLineColor(colorpPb);

            ratioSys->SetLineColor(colorpPb);
            ratioSys->SetFillColorAlpha(colorpPb,alphasval);
            ratioSys->SetFillStyle(1);

            vecRatiopPb.push_back(ratio);
            vecRatioSyspPb.push_back(ratioSys);
        }
    }

    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("cSpectrum", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();

    TCanvas *cRatio   = new TCanvas("c2", "", 800, 800);
    DrawPaperCanvasSettings(cRatio,0.14,0.025,0.025,0.08);

    gStyle->SetOptStat(0);

    TLegend *legendSpectrum;
    TLegend *legendRatio;

    TLegend *legendErrorKey =  GetAndSetLegend2(0.19,0.11,0.39,0.11+(2)*textSize,textSize);
    legendErrorKey->AddEntry(vecSpectrumSyspp.at(0), "Systematic Uncertainty", "f");
    legendErrorKey->AddEntry(vecSpectrumpp.at(0), "Statistical Uncertainty", "e");

    TLegend *legendErrorKeyRatio =  GetAndSetLegend2(0.6,0.11,0.8,0.11+(2)*textSize,textSize);
    legendErrorKeyRatio->AddEntry(vecRatioSyspp.at(0), "Systematic Uncertainty", "f");
    legendErrorKeyRatio->AddEntry(vecRatiopp.at(0), "Statistical Uncertainty", "e");

    // Set up dummy histos for plotting
    TH1D *dummySpectrum = new TH1D("dummySpectrum","",350,0,350);
    dummySpectrum->GetXaxis()->SetRangeUser(0,340);
    dummySpectrum->GetYaxis()->SetRangeUser(2e-8,3e-2);
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    TH1D *dummyRatio = new TH1D("dummyRatio","",350,0,350);
    dummyRatio->GetXaxis()->SetRangeUser(0,340);
    dummyRatio->GetYaxis()->SetRangeUser(0,1.4);
    SetStyleHistoTH1ForGraphs(dummyRatio,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}^{#it{R}=0.2}}{d#it{p}_{T}d#it{#eta}}/#frac{d^{2}#it{#sigma}^{#it{R}=0.#it{X}}}{d#it{p}_{T}d#it{#eta}}",textSize,0.035,textSize,0.035,1,1.7);

    TLine * line = new TLine (0,1,340,1);
    line->SetLineColor(14);
    line->SetLineWidth(3);
    line->SetLineStyle(7);

    cSpectrum->cd();
    ///////////////////////////////
    // Plot spectrum comparisons //
    ///////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius == 2 || radius == 4) legendSpectrum    =  GetAndSetLegend2(0.71,0.82-(4)*textSize,0.91,0.82,textSize);
        else legendSpectrum    =  GetAndSetLegend2(0.71,0.82-(2)*textSize,0.91,0.82,textSize);

        TGraphErrors *finalSpectrumpp = (TGraphErrors*)vecSpectrumpp.at(radius-minradius)->Clone(Form("finalSpectrum_pp_R0%i", radius));
        TGraphErrors *finalSpectrumSyspp = (TGraphErrors*)vecSpectrumSyspp.at(radius-minradius)->Clone(Form("finalSpectrumSys_pp_R0%i", radius));
        legendSpectrum->AddEntry(finalSpectrumpp, "pp #sqrt{#it{s}} = 8 TeV", "p");

        TGraphErrors *finalSpectrumpPb = (TGraphErrors*)vecSpectrumpPb.at(radius-minradius)->Clone(Form("finalSpectrum_pPb_R0%i", radius));
        TGraphErrors *finalSpectrumSyspPb = (TGraphErrors*)vecSpectrumSyspPb.at(radius-minradius)->Clone(Form("finalSpectrumSys_pPb_R0%i", radius));
        legendSpectrum->AddEntry(finalSpectrumpPb, "p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV", "p");

        dummySpectrum->Draw("axis");
        finalSpectrumpp->Draw("p,e1,same");
        finalSpectrumSyspp->Draw("e2,same");
        finalSpectrumpPb->Draw("p,e1,same");
        finalSpectrumSyspPb->Draw("e2,same");

        legendSpectrum->Draw("same");
        legendErrorKey->Draw("same");

        drawLatexAdd("ALICE Preliminary",0.93,0.93, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.%i", radius),0.93,0.9, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

        cSpectrum->SaveAs(Form("%s/EnergyComparisons/SpectrumComparison_R0%i.%s",output.Data(),radius,fileType.Data()));

        legendSpectrum->Clear();
    }

    cRatio->cd();
    ////////////////////////////
    // Plot ratio comparisons //
    ////////////////////////////
    for(int radius = minradius+1; radius <= maxradius; radius++){
        if(radius == 4) legendRatio       = GetAndSetLegend2(0.73,0.94-(4)*textSize,0.93,0.94,textSize);
        else legendRatio       = GetAndSetLegend2(0.73,0.94-(3)*textSize,0.93,0.94,textSize);

        TGraphErrors *finalRatiopp = (TGraphErrors*)vecRatiopp.at(radius-(minradius+1))->Clone(Form("finalRatio_pp_R02R0%i", radius));
        TGraphErrors *finalRatioSyspp = (TGraphErrors*)vecRatioSyspp.at(radius-(minradius+1))->Clone(Form("finalRatioSys_pp_R02R0%i", radius));
        legendRatio->AddEntry(finalRatiopp, "pp #sqrt{#it{s}} = 8 TeV", "p");

        TGraphErrors *finalRatiopPb = (TGraphErrors*)vecRatiopPb.at(radius-(minradius+1))->Clone(Form("finalRatio_pPb_R02R0%i", radius));
        TGraphErrors *finalRatioSyspPb = (TGraphErrors*)vecRatioSyspp.at(radius-(minradius+1))->Clone(Form("finalRatioSys_pPb_R02R0%i", radius));
        legendRatio->AddEntry(finalRatiopPb, "p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV", "p");

        dummyRatio->Draw("axis");
        finalRatioSyspp->Draw("e2,same");
        finalRatiopp->Draw("p,e1,same");
        finalRatioSyspPb->Draw("e2,same");
        finalRatiopPb->Draw("p,e1,same");

        line->Draw("same");
        legendRatio->Draw("same");
        legendErrorKeyRatio->Draw("same");

        drawLatexAdd("ALICE Preliminary",0.17,0.92, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.2/0.%i", radius),0.17,0.88, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.17,0.84, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.17,0.8, textSize,kFALSE, kFALSE, false);

        cRatio->SaveAs(Form("%s/EnergyComparisons/RatioComparison_R0%i.%s",output.Data(),radius,fileType.Data()));

        legendRatio->Clear();
    }
}
