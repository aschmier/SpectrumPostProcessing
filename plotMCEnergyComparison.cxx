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

TGraphErrors *FormatSpectrum(TGraphErrors *g, int radius, bool sys, bool pythia = false, bool powheg = false, int minradius = 2, int maxradius = 5, bool pPbComp = false){

    int stylesempty[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    Double_t textSize     = 0.03;
    Int_t regnum          = 6;
    double alphasval = 0.25;

    Color_t kPythiaColor = kRed+2;
    Color_t kPowhegColor = kBlue+2;

    Color_t colors[12] = {kBlack, kGreen+2, kCyan+2, kMagenta+2, kOrange+7, kSpring+9, kTeal+2, kAzure-4, kViolet+5, kPink-4};
    if(pPbComp){
        for(int c = 0; c < 10; c++){
            colors[c] = colors[c+3];
        }
    }
    int linestyles[4] = {7,3,4,10};

    for(int point = 0; point < g->GetN(); point++){
        double e = g->GetErrorY(point);
        if(sys) g->SetPointError(point,g->GetErrorX(point),e);
        else g->SetPointError(point,0,e);
    }

    if(sys || pythia || powheg){
        if(pythia){
            g->SetFillColorAlpha(kPythiaColor,alphasval);
            g->SetLineColor(kPythiaColor);
            g->SetMarkerColor(kPythiaColor);
        }else if(powheg){
            g->SetFillColorAlpha(kPowhegColor,alphasval);
            g->SetLineColor(kPowhegColor);
            g->SetMarkerColor(kPowhegColor);
        }else{
            g->SetFillColorAlpha(colors[radius-minradius],alphasval);
            g->SetLineColor(colors[radius-minradius]);
        }
            if(!pythia && !powheg) g->SetFillStyle(0);
            if(pythia || powheg) g->SetLineWidth(3);
            if(pythia) g->SetLineStyle(linestyles[0]);
            if(powheg) g->SetLineStyle(linestyles[1]);
    }else{
            g->SetMarkerStyle(stylesfilled[radius-minradius]);
            g->SetMarkerSize(sizesfilled[radius-minradius]);
            g->SetMarkerColor(colors[radius-minradius]);
            g->SetLineColor(colors[radius-minradius]);
            g->SetLineWidth(3);
    }

    for(int point = (g->GetN())-1; point >= 0; point--){
        if(g->GetPointX(point) > 240) g->RemovePoint(point);
        if(radius > 4 && g->GetPointX(point) > 160) g->RemovePoint(point);
        if(g->GetPointX(point) < 20) g->RemovePoint(point);
    }

    return g;
}

void plotMCEnergyComparison(TString nFile8TeV, TString nFile13TeV, TString nFile13TeVPOWHEG, TString output, TString fileType)
{
    Double_t textSize     = 0.03;

    int minradius = 2;
    int maxradius = 5;
    double minPt  = 0.01;
    double maxPt  = 250.;

    int scale[5] = {1,3,10,30,100};

    // 0 = 8 TeV, 1 = 13 TeV
    vector<TGraphErrors*> vecSpectrumStat[2];
    vector<TGraphErrors*> vecSpectrumSyst[2];
    vector<TGraphErrors*> vecRatioStat[2];
    vector<TGraphErrors*> vecRatioSyst[2];
    vector<TGraphErrors*> vecPythiaStat[2];
    vector<TGraphErrors*> vecRatioPythiaStat[2];
    vector<TGraphErrors*> vecPowhegStat[2];
    vector<TGraphErrors*> vecPowhegSyst[2];
    vector<TGraphErrors*> vecRatioPowhegStat[2];
    vector<TGraphErrors*> vecRatioPowhegSyst[2];

    // Open root files
    TFile *f8TeV = TFile::Open(nFile8TeV);
    if(!f8TeV || f8TeV->IsZombie()){
        cout << "8 TeV file not found!" << endl;
        return;
    }

    TFile *f13TeV = TFile::Open(nFile13TeV);
    if(!f13TeV || f13TeV->IsZombie()){
        cout << "13 TeV file not found!" << endl;
        return;
    }

    TFile *f13TeVPOWHEG = TFile::Open(nFile13TeVPOWHEG);
    if(!f13TeVPOWHEG || f13TeVPOWHEG->IsZombie()){
        cout << "13 TeV POWHEG file not found!" << endl;
        return;
    }

    // Get spectra for 8 TeV
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *dir8TeV = (TDirectory*)f8TeV->Get(Form("R0%i",radius));

        TGraphErrors *spectrumStat = (TGraphErrors*)dir8TeV->Get(Form("spectrumStat_R0%i",radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)dir8TeV->Get(Form("spectrumSyst_R0%i",radius));
        TGraphErrors *powhegStat  = (TGraphErrors*)dir8TeV->Get(Form("powhegStat_R0%i",radius));
        TGraphErrors *powhegSyst  = (TGraphErrors*)dir8TeV->Get(Form("powhegSyst_R0%i",radius));
        TGraphErrors *pythiaStat  = (TGraphErrors*)dir8TeV->Get(Form("pythiaStat_R0%i",radius));

        spectrumStat = FormatSpectrum(spectrumStat,radius,false,false,false);
        spectrumSyst = FormatSpectrum(spectrumSyst,radius,true,false,false);
        powhegStat = FormatSpectrum(powhegStat,radius,false,false,true);
        powhegSyst = FormatSpectrum(powhegSyst,radius,true,false,true);
        pythiaStat = FormatSpectrum(pythiaStat,radius,false,true,false);

        vecSpectrumStat[0].push_back(spectrumStat);
        vecSpectrumSyst[0].push_back(spectrumSyst);
        vecPowhegStat[0].push_back(powhegStat);
        vecPowhegSyst[0].push_back(powhegSyst);
        vecPythiaStat[0].push_back(pythiaStat);
    }

    // Get spectra for 13 TeV
    for(int radius = minradius; radius <= maxradius; radius++){
        //////////////////////////////////////////////////////////////////////////////////////////
        // Use this section to get 13 TeV spectra from f13TeV and f13TeVPOWHEG in TGraph format //
        //////////////////////////////////////////////////////////////////////////////////////////

        spectrumStat = FormatSpectrum(spectrumStat,radius,false,false,false);
        spectrumSyst = FormatSpectrum(spectrumSyst,radius,true,false,false);
        powhegStat = FormatSpectrum(powhegStat,radius,false,false,true);
        powhegSyst = FormatSpectrum(powhegSyst,radius,true,false,true);
        pythiaStat = FormatSpectrum(pythiaStat,radius,false,true,false);

        vecSpectrumStat[1].push_back(spectrumStat);
        vecSpectrumSyst[1].push_back(spectrumSyst);
        vecPowhegStat[1].push_back(powhegStat);
        vecPowhegSyst[1].push_back(powhegSyst);
        vecPythiaStat[1].push_back(pythiaStat);
    }
    
    /////////////////////////////////
    // Plot simulation comparisons //
    /////////////////////////////////
    Double_t textsizeLabels = 0;
    Double_t textsizeFac    = 0;
    Double_t textSizeLabelsPixel = 30;

    Double_t arrayBoundariesX[2];
    Double_t arrayBoundariesY[5];
    Double_t relativeMarginsX[3];
    Double_t relativeMarginsY[3];

    ReturnCorrectValuesForCanvasScaling(800,1000, 1, 4,0.1, 0.025, 0.005,0.075,arrayBoundariesX,arrayBoundariesY,relativeMarginsX,relativeMarginsY);
    Double_t margin = relativeMarginsX[0]*2.7*800;

    TCanvas *canvas = new TCanvas("canvasSimcomp","",800,1000);;
    TPad    *ratioPad[maxradius-minradius+1];
    TH2F    *dummyHist;

    gStyle->SetOptStat(0);

    TLegend *legendErrorKey;
    TLegend *legendSim;

    DrawGammaCanvasSettings( canvas,  0.15, 0.025, 0.025, 0.08);
    canvas->cd();

    for(int radius = minradius; radius <= maxradius; radius++){
        ratioPad[radius-minradius]       = new TPad(Form("ratioPadR0%i", radius), "", arrayBoundariesX[0], arrayBoundariesY[radius-minradius+1], arrayBoundariesX[1], arrayBoundariesY[radius-minradius],-1, -1, -2);

        if(radius==minradius) DrawGammaPadSettings( ratioPad[radius-minradius], relativeMarginsX[0], relativeMarginsX[2], relativeMarginsY[0], relativeMarginsY[1]);
        else if(radius==maxradius) DrawGammaPadSettings( ratioPad[radius-minradius], relativeMarginsX[0], relativeMarginsX[2], relativeMarginsY[1], relativeMarginsY[2]);
        else DrawGammaPadSettings( ratioPad[radius-minradius], relativeMarginsX[0], relativeMarginsX[2], relativeMarginsY[1], relativeMarginsY[1]);

        ratioPad[radius-minradius]->Draw();
    }
    for(int radius = minradius; radius <= maxradius; radius++){

        if (ratioPad[radius-minradius]->XtoPixel(ratioPad[radius-minradius]->GetX2()) < ratioPad[radius-minradius]->YtoPixel(ratioPad[radius-minradius]->GetY1()) ){
          	textsizeLabels              = (Double_t)textSizeLabelsPixel/ratioPad[radius-minradius]->XtoPixel(ratioPad[radius-minradius]->GetX2()) ;
          	textsizeFac                 = (Double_t)1./ratioPad[radius-minradius]->XtoPixel(ratioPad[radius-minradius]->GetX2()) ;
        }else{
          	textsizeLabels              = (Double_t)textSizeLabelsPixel/ratioPad[radius-minradius]->YtoPixel(ratioPad[radius-minradius]->GetY1());
          	textsizeFac                 = (Double_t)1./ratioPad[radius-minradius]->YtoPixel(ratioPad[radius-minradius]->GetY1());
        }

        dummyHist = new TH2F(Form("dummyHist_R0%i", radius),Form("dummyHist_R0%i", radius), 1000, minPt,maxPt, 1000., 0.2,2.05);
        SetStyleHistoTH2ForGraphs(dummyHist, "#it{p}_{T} (GeV/#it{c})", "MC/Data     ", 0.85*textsizeLabels, textsizeLabels, 0.85*textsizeLabels, textsizeLabels, 1, 0.35/(textsizeFac*margin),512,505,42,42);

        ratioPad[radius-minradius]->cd();
        dummyHist->DrawCopy();

        // Process 8 TeV points

        TGraphErrors *spectrumStat8 = (TGraphErrors*)vecSpectrumStat[0].at(radius-minradius)->Clone(Form("spectrumStat8_R0%i", radius));
        TGraphErrors *spectrumSyst8 = (TGraphErrors*)vecSpectrumSyst[0].at(radius-minradius)->Clone(Form("spectrumSyst8_R0%i", radius));
        TGraphErrors *powhegStat8 = (TGraphErrors*)vecPowhegStat[0].at(radius-minradius)->Clone(Form("powhegStat8_R0%i", radius));
        TGraphErrors *powhegSyst8 = (TGraphErrors*)vecPowhegSyst[0].at(radius-minradius)->Clone(Form("powhegSyst8_R0%i", radius));
        TGraphErrors *pythiaStat8 = (TGraphErrors*)vecPythiaStat[0].at(radius-minradius)->Clone(Form("pythiaStat8_R0%i", radius));

        TGraphErrors *ratioPythiaStatData8 = (TGraphErrors*)pythiaStat8->Clone(Form("ratioPythiaStatData8_R0%i", radius));
        TGraphErrors *ratioPowhegStatData8 = (TGraphErrors*)powhegStat8->Clone(Form("ratioPowhegStatData8_R0%i", radius));
        TGraphErrors *ratioPowhegSystData8 = (TGraphErrors*)powhegSyst8->Clone(Form("ratioPowhegSystData8_R0%i", radius));
        TGraphErrors *ratioDataDataStat8   = (TGraphErrors*)spectrumStat8->Clone(Form("ratioDataDataStat8_R0%i", radius));
        TGraphErrors *ratioDataDataSyst8   = (TGraphErrors*)spectrumSyst8->Clone(Form("ratioDataDataSyst8_R0%i", radius));

        for(int i = 0; i < spectrumStat8->GetN(); i++){
            Double_t xDataStat, yDataStat, exDataStat, eyDataStat;
            Double_t xDataSyst, yDataSyst, exDataSyst, eyDataSyst;
            Double_t xPythiaStat, yPythiaStat, exPythiaStat, eyPythiaStat;
            Double_t xPowhegStat, yPowhegStat, exPowhegStat, eyPowhegStat;
            Double_t xPowhegSyst, yPowhegSyst, exPowhegSyst, eyPowhegSyst;

            spectrumStat8->GetPoint(i, xDataStat, yDataStat);
            exDataStat = spectrumStat8->GetErrorX(i);
            eyDataStat = spectrumStat8->GetErrorY(i)/yDataStat;

            spectrumSyst8->GetPoint(i, xDataSyst, yDataSyst);
            exDataSyst = spectrumSyst8->GetErrorX(i);
            eyDataSyst = spectrumSyst8->GetErrorY(i)/yDataSyst;

            pythiaStat8->GetPoint(i, xPythiaStat, yPythiaStat);
            exPythiaStat = pythiaStat8->GetErrorX(i);
            eyPythiaStat = pythiaStat8->GetErrorY(i)/yPythiaStat;

            powhegStat8->GetPoint(i, xPowhegStat, yPowhegStat);
            exPowhegStat = powhegStat8->GetErrorX(i);
            eyPowhegStat = powhegStat8->GetErrorY(i)/yPowhegStat;

            powhegSyst8->GetPoint(i, xPowhegSyst, yPowhegSyst);
            exPowhegSyst = powhegSyst8->GetErrorX(i);
            eyPowhegSyst = powhegSyst8->GetErrorY(i)/yPowhegSyst;

            if(xDataStat != xPythiaStat || xDataStat != xPowhegStat || xDataStat != xPowhegSyst || xDataStat != xDataSyst) {
                printf("Error: xData != xMC\n");
                continue;
            }

            ratioPythiaStatData8->SetPoint(i, xDataStat, yPythiaStat/yDataStat);
            ratioPythiaStatData8->SetPointError(i, exPythiaStat, eyPythiaStat*(yPythiaStat/yDataStat));
            ratioPowhegStatData8->SetPoint(i, xDataStat, yPowhegStat/yDataStat);
            ratioPowhegStatData8->SetPointError(i, exPowhegStat, eyPowhegStat*(yPowhegStat/yDataStat));
            ratioPowhegSystData8->SetPoint(i, xDataSyst, yPowhegSyst/yDataSyst);
            ratioPowhegSystData8->SetPointError(i, exPowhegSyst, eyPowhegSyst*(yPowhegSyst/yDataSyst));
            ratioDataDataStat8->SetPoint(i, xDataStat, 1);
            ratioDataDataStat8->SetPointError(i, exDataStat, eyDataStat);
            ratioDataDataSyst8->SetPoint(i, xDataSyst, 1);
            ratioDataDataSyst8->SetPointError(i, exDataSyst, eyDataSyst);
        }

        // Process 13 TeV points

        TGraphErrors *spectrumStat13 = (TGraphErrors*)vecSpectrumStat[1].at(radius-minradius)->Clone(Form("spectrumStat13_R0%i", radius));
        TGraphErrors *spectrumSyst13 = (TGraphErrors*)vecSpectrumSyst[1].at(radius-minradius)->Clone(Form("spectrumSyst13_R0%i", radius));
        TGraphErrors *powhegStat13 = (TGraphErrors*)vecPowhegStat[1].at(radius-minradius)->Clone(Form("powhegStat13_R0%i", radius));
        TGraphErrors *powhegSyst13 = (TGraphErrors*)vecPowhegSyst[1].at(radius-minradius)->Clone(Form("powhegSyst13_R0%i", radius));

        TGraphErrors *ratioPowhegStatData13 = (TGraphErrors*)powhegStat13->Clone(Form("ratioPowhegStatData13_R0%i", radius));
        TGraphErrors *ratioPowhegSystData13 = (TGraphErrors*)powhegSyst13->Clone(Form("ratioPowhegSystData13_R0%i", radius));
        TGraphErrors *ratioDataDataStat13   = (TGraphErrors*)spectrumStat13->Clone(Form("ratioDataDataStat13_R0%i", radius));
        TGraphErrors *ratioDataDataSyst13   = (TGraphErrors*)spectrumSyst13->Clone(Form("ratioDataDataSyst13_R0%i", radius));

        for(int i = 0; i < spectrumStat13->GetN(); i++){
            Double_t xDataStat, yDataStat, exDataStat, eyDataStat;
            Double_t xDataSyst, yDataSyst, exDataSyst, eyDataSyst;
            Double_t xPowhegStat, yPowhegStat, exPowhegStat, eyPowhegStat;
            Double_t xPowhegSyst, yPowhegSyst, exPowhegSyst, eyPowhegSyst;

            spectrumStat13->GetPoint(i, xDataStat, yDataStat);
            exDataStat = spectrumStat13->GetErrorX(i);
            eyDataStat = spectrumStat13->GetErrorY(i)/yDataStat;

            spectrumSyst13->GetPoint(i, xDataSyst, yDataSyst);
            exDataSyst = spectrumSyst13->GetErrorX(i);
            eyDataSyst = spectrumSyst13->GetErrorY(i)/yDataSyst;

            powhegStat13->GetPoint(i, xPowhegStat, yPowhegStat);
            exPowhegStat = powhegStat13->GetErrorX(i);
            eyPowhegStat = powhegStat13->GetErrorY(i)/yPowhegStat;

            powhegSyst13->GetPoint(i, xPowhegSyst, yPowhegSyst);
            exPowhegSyst = powhegSyst13->GetErrorX(i);
            eyPowhegSyst = powhegSyst13->GetErrorY(i)/yPowhegSyst;

            if(xDataStat != xPowhegStat || xDataStat != xPowhegSyst || xDataStat != xDataSyst) {
                printf("Error: xData != xMC\n");
                continue;
            }

            ratioPowhegStatData13->SetPoint(i, xDataStat, yPowhegStat/yDataStat);
            ratioPowhegStatData13->SetPointError(i, exPowhegStat, eyPowhegStat*(yPowhegStat/yDataStat));
            ratioPowhegSystData13->SetPoint(i, xDataSyst, yPowhegSyst/yDataSyst);
            ratioPowhegSystData13->SetPointError(i, exPowhegSyst, eyPowhegSyst*(yPowhegSyst/yDataSyst));
            ratioDataDataStat13->SetPoint(i, xDataStat, 1);
            ratioDataDataStat13->SetPointError(i, exDataStat, eyDataStat);
            ratioDataDataSyst13->SetPoint(i, xDataSyst, 1);
            ratioDataDataSyst13->SetPointError(i, exDataSyst, eyDataSyst);
        }

        // Plotting time

        DrawGammaLines(0.,260.,1.,1.,8.,16,9);
        ratioPowhegSystData8->Draw("e3,same");
        ratioDataDataSyst8->Draw("e2,same");
        ratioDataDataStat8->Draw("p,e1,same");
        ratioPythiaStatData8->Draw("l,e3,same");
        ratioPowhegStatData8->Draw("l,e3,same");

        ratioPowhegSystData13->Draw("e3,same");
        ratioDataDataSyst13->Draw("e2,same");
        ratioDataDataStat13->Draw("p,e1,same");
        ratioPowhegStatData13->Draw("l,e3,same");

        dummyHist->Draw("axis,same");

        if(radius==maxradius){
            drawLatexAdd(Form("#it{R} = 0.%i", radius),0.25,0.31, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("pp #sqrt{#it{s}} = 8, 13 TeV",0.94,0.88, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.94,0.78, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.94,0.42, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.94,0.32, 0.85*textsizeLabels,kFALSE, kFALSE, true);
        }else drawLatexAdd(Form("#it{R} = 0.%i", radius),0.25,0.07, 0.85*textsizeLabels,kFALSE, kFALSE, true);

        if(radius==minradius){
            legendErrorKey = GetAndSetLegend2(0.58,0.03,0.78,0.03+2*(0.85*textsizeLabels),(0.85*textsizeLabels));
            legendErrorKey->AddEntry(vecSpectrumSyst[0].at(0), "Systematic Uncertainty", "f");
            legendErrorKey->AddEntry(vecSpectrumStat[0].at(0), "Statistical Uncertainty", "e");
            legendErrorKey->Draw("same");
        }

        if(radius==(minradius+1)){
            legendSim = GetAndSetLegend2(0.58,0.03,0.78,0.03+2*(0.85*textsizeLabels),(0.85*textsizeLabels));
            legendSim->AddEntry(ratioPythiaStatData, "PYTHIA8 Monash", "l");
            legendSim->AddEntry(ratioPowhegStatData, "POWHEG+PYTHIA8", "l");
            legendSim->Draw("same");
        }
    }
    canvas->SaveAs(Form("%s/mccomparison_allradii.%s",output.Data(),fileType.Data()));
}
