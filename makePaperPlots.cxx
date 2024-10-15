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

void makePaperPlots(TString ppfileFine, TString ppfileCoarse, TString pPbfile, TString output, TString fileType)
{
    Double_t textSize     = 0.03;

    int minradius = 2;
    int maxradius = 5;
    double minPt  = 0.01;
    double maxPt  = 250.;

    int scale[5] = {1,3,10,30,100};

    // 0 = ppFine, 1 = ppCourse, 2 = pPb
    vector<TGraphErrors*> vecSpectrumStat[3];
    vector<TGraphErrors*> vecSpectrumSyst[3];
    vector<TGraphErrors*> vecRatioStat[3];
    vector<TGraphErrors*> vecRatioSyst[3];
    vector<TGraphErrors*> vecPythiaStat[3];
    vector<TGraphErrors*> vecRatioPythiaStat[3];
    vector<TGraphErrors*> vecPowhegStat[3];
    vector<TGraphErrors*> vecPowhegSyst[3];
    vector<TGraphErrors*> vecRatioPowhegStat[3];
    vector<TGraphErrors*> vecRatioPowhegSyst[3];

    // Open root files
    TFile *fppFine = TFile::Open(ppfileFine);
    if(!fppFine || fppFine->IsZombie()){
        cout << "pp file not found!" << endl;
        return;
    }

    TFile *fppCoarse = TFile::Open(ppfileCoarse);
    if(!fppCoarse || fppCoarse->IsZombie()){
        cout << "Coarse rebinned pp file not found!" << endl;
        return;
    }

    TFile *fpPb = TFile::Open(pPbfile);
    if(!fpPb || fpPb->IsZombie()){
        cout << "pPb file not found!" << endl;
        return;
    }

    // Get spectra and ratios for fine binned pp
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *dppFine = (TDirectory*)fppFine->Get(Form("R0%i",radius));

        TGraphErrors *spectrumStat = (TGraphErrors*)dppFine->Get(Form("spectrumStat_R0%i",radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)dppFine->Get(Form("spectrumSyst_R0%i",radius));
        TGraphErrors *powhegStat  = (TGraphErrors*)dppFine->Get(Form("powhegStat_R0%i",radius));
        TGraphErrors *powhegSyst  = (TGraphErrors*)dppFine->Get(Form("powhegSyst_R0%i",radius));
        TGraphErrors *pythiaStat  = (TGraphErrors*)dppFine->Get(Form("pythiaStat_R0%i",radius));

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

        if(radius != minradius){
            TGraphErrors *ratioStat = (TGraphErrors*)dppFine->Get(Form("ratioStat_R0%i",radius));
            TGraphErrors *ratioSyst = (TGraphErrors*)dppFine->Get(Form("ratioSyst_R0%i",radius));
            TGraphErrors *ratioPowhegStat = (TGraphErrors*)dppFine->Get(Form("ratioPowhegStat_%i",radius));
            TGraphErrors *ratioPowhegSyst = (TGraphErrors*)dppFine->Get(Form("ratioPowhegSyst_%i",radius));
            TGraphErrors *ratioPythiaStat = (TGraphErrors*)dppFine->Get(Form("ratioPythiaStat_%i",radius));

            ratioStat = FormatSpectrum(ratioStat,radius,false,false,false);
            ratioSyst = FormatSpectrum(ratioSyst,radius,true,false,false);
            ratioPowhegStat = FormatSpectrum(ratioPowhegStat,radius,false,false,true);
            ratioPowhegSyst = FormatSpectrum(ratioPowhegSyst,radius,true,false,true);
            ratioPythiaStat = FormatSpectrum(ratioPythiaStat,radius,false,true,false);

            vecRatioStat[0].push_back(ratioStat);
            vecRatioSyst[0].push_back(ratioSyst);
            vecRatioPowhegStat[0].push_back(ratioPowhegStat);
            vecRatioPowhegSyst[0].push_back(ratioPowhegSyst);
            vecRatioPythiaStat[0].push_back(ratioPythiaStat);
        }
    }

    // Get spectra and ratios for coarse binned pp
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *dppCoarse = (TDirectory*)fppCoarse->Get(Form("R0%i",radius));

        TGraphErrors *spectrumStat = (TGraphErrors*)dppCoarse->Get(Form("spectrumStat_R0%i",radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)dppCoarse->Get(Form("spectrumSyst_R0%i",radius));
        TGraphErrors *powhegStat  = (TGraphErrors*)dppCoarse->Get(Form("powhegStat_R0%i",radius));
        TGraphErrors *powhegSyst  = (TGraphErrors*)dppCoarse->Get(Form("powhegSyst_R0%i",radius));
        TGraphErrors *pythiaStat  = (TGraphErrors*)dppCoarse->Get(Form("pythiaStat_R0%i",radius));

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

        if(radius != minradius){
            TGraphErrors *ratioStat = (TGraphErrors*)dppCoarse->Get(Form("ratioStat_R0%i",radius));
            TGraphErrors *ratioSyst = (TGraphErrors*)dppCoarse->Get(Form("ratioSyst_R0%i",radius));
            TGraphErrors *ratioPowhegStat = (TGraphErrors*)dppCoarse->Get(Form("ratioPowhegStat_%i",radius));
            TGraphErrors *ratioPowhegSyst = (TGraphErrors*)dppCoarse->Get(Form("ratioPowhegSyst_%i",radius));
            TGraphErrors *ratioPythia = (TGraphErrors*)dppCoarse->Get(Form("ratioPythiaStat_%i",radius));

            ratioStat = FormatSpectrum(ratioStat,radius,false,false,false);
            ratioSyst = FormatSpectrum(ratioSyst,radius,true,false,false);
            ratioPowhegStat = FormatSpectrum(ratioPowhegStat,radius,false,false,true);
            ratioPowhegSyst = FormatSpectrum(ratioPowhegSyst,radius,true,false,true);
            ratioPythia = FormatSpectrum(ratioPythia,radius,false,true,false);

            vecRatioStat[1].push_back(ratioStat);
            vecRatioSyst[1].push_back(ratioSyst);
            vecRatioPowhegStat[1].push_back(ratioPowhegStat);
            vecRatioPowhegSyst[1].push_back(ratioPowhegSyst);
            vecRatioPythiaStat[1].push_back(ratioPythia);
        }
    }

    // Get spectra and ratios for pPb
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *dpPb = (TDirectory*)fpPb->Get(Form("R0%i",radius));

        TGraphErrors *spectrumStat = (TGraphErrors*)dpPb->Get(Form("spectrumStat_R0%i",radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)dpPb->Get(Form("spectrumSyst_R0%i",radius));
        TGraphErrors *powhegStat  = (TGraphErrors*)dpPb->Get(Form("powhegStat_R0%i",radius));
        TGraphErrors *powhegSyst  = (TGraphErrors*)dpPb->Get(Form("powhegSyst_R0%i",radius));
        TGraphErrors *pythiaStat  = (TGraphErrors*)dpPb->Get(Form("pythiaStat_R0%i",radius));
        
        spectrumStat = FormatSpectrum(spectrumStat,radius,false,false,false);
        spectrumSyst = FormatSpectrum(spectrumSyst,radius,true,false,false);
        powhegStat = FormatSpectrum(powhegStat,radius,false,false,true);
        powhegSyst = FormatSpectrum(powhegSyst,radius,true,false,true);
        pythiaStat = FormatSpectrum(pythiaStat,radius,false,true,false);

        vecSpectrumStat[2].push_back(spectrumStat);
        vecSpectrumSyst[2].push_back(spectrumSyst);
        vecPowhegStat[2].push_back(powhegStat);
        vecPowhegSyst[2].push_back(powhegSyst);
        vecPythiaStat[2].push_back(pythiaStat);

        if(radius != minradius){
            TGraphErrors *ratioStat = (TGraphErrors*)dpPb->Get(Form("ratioStat_R0%i",radius));
            TGraphErrors *ratioSyst = (TGraphErrors*)dpPb->Get(Form("ratioSyst_R0%i",radius));
            TGraphErrors *ratioPowhegStat = (TGraphErrors*)dpPb->Get(Form("ratioPowhegStat_%i",radius));
            TGraphErrors *ratioPowhegSyst = (TGraphErrors*)dpPb->Get(Form("ratioPowhegSyst_%i",radius));
            TGraphErrors *ratioPythia = (TGraphErrors*)dpPb->Get(Form("ratioPythiaStat_%i",radius));

            ratioStat = FormatSpectrum(ratioStat,radius,false,false,false);
            ratioSyst = FormatSpectrum(ratioSyst,radius,true,false,false);
            ratioPowhegStat = FormatSpectrum(ratioPowhegStat,radius,false,false,true);
            ratioPowhegSyst = FormatSpectrum(ratioPowhegSyst,radius,true,false,true);
            ratioPythia = FormatSpectrum(ratioPythia,radius,false,true,false);

            vecRatioStat[2].push_back(ratioStat);
            vecRatioSyst[2].push_back(ratioSyst);
            vecRatioPowhegStat[2].push_back(ratioPowhegStat);
            vecRatioPowhegSyst[2].push_back(ratioPowhegSyst);
            vecRatioPythiaStat[2].push_back(ratioPythia);
        }
    }

    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("c", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();
    cSpectrum->SetLogx();

    gStyle->SetOptStat(0);

    // Set up dummy histos for plotting
    TH1D *dummySpectrumLogX = new TH1D("dummySpectrumLogX","",300,0,300);
    dummySpectrumLogX->GetXaxis()->SetRangeUser(18,260);
    SetStyleHistoTH1ForGraphs(dummySpectrumLogX,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);
    dummySpectrumLogX->GetXaxis()->SetMoreLogLabels(true);

    // Set Up legends
    TLegend *legendErrorKey;
    TLegend *legendSpectrum;
    TLegend *legendSimNewer =  GetAndSetLegend2(0.19,0.2,0.39,0.2+2*textSize,textSize);

    /////////////////////////////////////
    // Plot scaled cross-sections logx //
    /////////////////////////////////////
    cSpectrum->cd();

    for(int i : {0,2}){
        if(i==0){ 
            dummySpectrumLogX->GetYaxis()->SetRangeUser(3e-8,9e-1);
            legendSpectrum = GetAndSetLegend2(0.73,0.79-(maxradius-minradius+1)*textSize,0.93,0.79,textSize);
        }
        if(i==2){
            dummySpectrumLogX->GetYaxis()->SetRangeUser(3e-8,9e-1);
            legendSpectrum =  GetAndSetLegend2(0.73,0.79-(maxradius-minradius)*textSize,0.93,0.79,textSize);
        }

        for(int radius = minradius; radius <= maxradius; radius++){
            if(i==2 && radius==maxradius) continue;
            TGraphErrors *spectrumStat = (TGraphErrors*)vecSpectrumStat[i].at(radius-minradius)->Clone(Form("spectrumStat_R0%i", radius));
            TGraphErrors *spectrumSyst = (TGraphErrors*)vecSpectrumSyst[i].at(radius-minradius)->Clone(Form("spectrumSyst_R0%i", radius));
            TGraphErrors *powhegStat   = (TGraphErrors*)vecPowhegStat[i].at(radius-minradius)->Clone(Form("powhegStat_R0%i", radius));
            TGraphErrors *powhegSyst   = (TGraphErrors*)vecPowhegSyst[i].at(radius-minradius)->Clone(Form("powhegSyst_R0%i", radius));
            TGraphErrors *pythiaStat   = (TGraphErrors*)vecPythiaStat[i].at(radius-minradius)->Clone(Form("pythiaStat_R0%i", radius));

            spectrumStat->Scale((double)scale[radius-minradius]);
            spectrumSyst->Scale((double)scale[radius-minradius]);
            powhegStat->Scale((double)scale[radius-minradius]);
            powhegSyst->Scale((double)scale[radius-minradius]);
            pythiaStat->Scale((double)scale[radius-minradius]);

            legendSpectrum->AddEntry(spectrumStat, Form("R=0.%i",radius), "p");

            if(radius==minradius){
                legendErrorKey = GetAndSetLegend2(0.19,0.11,0.39,0.11+2*textSize,textSize);
                legendErrorKey->AddEntry(spectrumSyst, "Systematic Uncertainty", "f");
                legendErrorKey->AddEntry(spectrumStat, "Statistical Uncertainty", "e");
                dummySpectrumLogX->Draw("axis");
                legendSimNewer->AddEntry(pythiaStat, "PYTHIA8 Monash", "l");
                legendSimNewer->AddEntry(powhegStat, "POWHEG+PYTHIA8", "l");
            }

            spectrumSyst->Draw("e2,same");
            spectrumStat->Draw("p,e1,same");
            //powhegSyst->Draw("l,e3,same");
            powhegStat->Draw("l,e3,same");
            pythiaStat->Draw("l,e3,same");
        }

        legendSpectrum->Draw("same");
        legendErrorKey->Draw("same");
        legendSimNewer->Draw("same");

        if(i==0) drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
        if(i==2) drawLatexAdd("p-Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.9, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

        if(i==0) cSpectrum->SaveAs(Form("%s/jetspectrum_pp_logx_scaled.%s",output.Data(),fileType.Data()));
        if(i==2) cSpectrum->SaveAs(Form("%s/jetspectrum_pPb_logx_scaled.%s",output.Data(),fileType.Data()));

        legendSpectrum->Clear();
        legendErrorKey->Clear();
        legendSimNewer->Clear();

    }

    ////////////////////////////////////////////////
    // Plot combined ratio simulation comparisons //
    ////////////////////////////////////////////////
    
    Double_t textsizeLabels = 0;
    Double_t textsizeFac    = 0;
    Double_t textSizeLabelsPixel = 30;

    Double_t arrayBoundariesX[3];
    Double_t arrayBoundariesY[2];
    Double_t relativeMarginsX[3];
    Double_t relativeMarginsY[3];

    ReturnCorrectValuesForCanvasScaling(1000,500, 2, 1,0.08, 0.005, 0.005,0.11,arrayBoundariesX,arrayBoundariesY,relativeMarginsX,relativeMarginsY);
    Double_t margin = relativeMarginsX[0]*2.7*1000;
    TLegend *legendSimRatioCombined;


    TCanvas *canvas = new TCanvas("canvasSimratio","",1000,500);;
    TPad    *ratioPad[2];
    TH2F    *dummyHist;

    gStyle->SetOptStat(0);

    TLegend *legendErrorKeyRatioCombined;

    DrawGammaCanvasSettings( canvas,  0.15, 0.025, 0.025, 0.08);
    canvas->cd();

    ratioPad[0] = new TPad("ratioPadpp", "", arrayBoundariesX[0], arrayBoundariesY[1], arrayBoundariesX[1], arrayBoundariesY[0],-1, -1, -2);
    ratioPad[1] = new TPad("ratioPadpPb", "", arrayBoundariesX[1], arrayBoundariesY[1], arrayBoundariesX[2], arrayBoundariesY[0],-1, -1, -2);

    DrawGammaPadSettings( ratioPad[0], relativeMarginsX[0], relativeMarginsX[1], relativeMarginsY[0], relativeMarginsY[2]);
    DrawGammaPadSettings( ratioPad[1], relativeMarginsX[1], relativeMarginsX[2], relativeMarginsY[0], relativeMarginsY[2]);

    ratioPad[0]->Draw();
    ratioPad[1]->Draw();


    if (ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) < ratioPad[0]->YtoPixel(ratioPad[0]->GetY1()) ){
      	textsizeLabels              = (Double_t)textSizeLabelsPixel/ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) ;
      	textsizeFac                 = (Double_t)1./ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) ;
    }else{
      	textsizeLabels              = (Double_t)textSizeLabelsPixel/ratioPad[0]->YtoPixel(ratioPad[0]->GetY1());
      	textsizeFac                 = (Double_t)1./ratioPad[0]->YtoPixel(ratioPad[0]->GetY1());
    }

    legendSimRatioCombined = GetAndSetLegend2(0.17,0.88-(5*0.75*textsizeLabels),0.47,0.88,0.75*textsizeLabels);
    dummyHist = new TH2F("dummyHist","dummyHist", 1000, minPt,maxPt, 1000., 0.2,1.65);
    SetStyleHistoTH2ForGraphs(dummyHist, "#it{p}_{T} (GeV/#it{c})", "#frac{d^{2}#it{#sigma}^{#it{R}=0.2}}{d^{2}#it{#sigma}^{#it{R}=0.#it{X}}}", 0.65*textsizeLabels, 0.75*textsizeLabels, 0.65*textsizeLabels, 0.75*textsizeLabels, 1, 1.15/(textsizeFac*margin),512,505,42,42);

    ratioPad[0]->cd();
    dummyHist->DrawCopy();
    DrawGammaLines(minPt,maxPt,1.,1.,8.,16,9);

    for(int radius = minradius+1; radius <= maxradius; radius++){
        TGraphErrors *ratioStat = (TGraphErrors*)vecRatioStat[0].at(radius-minradius-1)->Clone(Form("ratioStat_R0%i", radius));
        TGraphErrors *ratioSyst = (TGraphErrors*)vecRatioSyst[0].at(radius-minradius-1)->Clone(Form("ratioSyst_R0%i", radius));

        TGraphErrors *ratioPythiaStat = (TGraphErrors*)vecRatioPythiaStat[0].at(radius-minradius-1)->Clone(Form("ratioPythiaStat_%i",radius));
        TGraphErrors *ratioPowhegStat = (TGraphErrors*)vecRatioPowhegStat[0].at(radius-minradius-1)->Clone(Form("ratioPowhegStat_%i",radius));
        TGraphErrors *ratioPowhegSyst = (TGraphErrors*)vecRatioPowhegSyst[0].at(radius-minradius-1)->Clone(Form("ratioPowhegSyst_%i",radius));

        legendSimRatioCombined->AddEntry(ratioStat, Form("R = 0.2/0.%i", radius), "p");
        if(radius==maxradius) legendSimRatioCombined->AddEntry(ratioPythiaStat, "PYTHIA8 Monash", "l");
        if(radius==maxradius) legendSimRatioCombined->AddEntry(ratioPowhegStat, "POWHEG+PYTHIA8", "l");

        ratioSyst->Draw("e2,same");
        ratioStat->Draw("p,e1,same");
        ratioPythiaStat->Draw("l,e3,same");
        ratioPowhegStat->Draw("l,e3,same");
        //ratioPowhegSyst->Draw("l,e3,same");
    }

    legendSimRatioCombined->Draw("same");
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.19,0.91, 0.75*textsizeLabels,kFALSE, kFALSE, false);

    ratioPad[1]->cd();
    dummyHist->DrawCopy();
    DrawGammaLines(minPt,maxPt,1.,1.,8.,16,9);

    for(int radius = minradius+1; radius <= maxradius-1; radius++){
        TGraphErrors *ratioStat = (TGraphErrors*)vecRatioStat[2].at(radius-minradius-1)->Clone(Form("ratioStat_R0%i", radius));
        TGraphErrors *ratioSyst = (TGraphErrors*)vecRatioSyst[2].at(radius-minradius-1)->Clone(Form("ratioSyst_R0%i", radius));

        TGraphErrors *ratioPythiaStat = (TGraphErrors*)vecRatioPythiaStat[2].at(radius-minradius-1)->Clone(Form("ratioPythiaStat_%i",radius));
        TGraphErrors *ratioPowhegStat = (TGraphErrors*)vecRatioPowhegStat[2].at(radius-minradius-1)->Clone(Form("ratioPowhegStat_%i",radius));
        TGraphErrors *ratioPowhegSyst = (TGraphErrors*)vecRatioPowhegSyst[2].at(radius-minradius-1)->Clone(Form("ratioPowhegSyst_%i",radius));

        ratioSyst->Draw("e2,same");
        ratioStat->Draw("p,e1,same");
        ratioPythiaStat->Draw("l,e3,same");
        ratioPowhegStat->Draw("l,e3,same");
        //ratioPowhegSyst->Draw("l,e3,same");

        if(radius==minradius+1){
            legendErrorKeyRatioCombined = GetAndSetLegend2(0.45,0.62,0.65,0.62+2*(0.75*textsizeLabels),(0.75*textsizeLabels));
            legendErrorKeyRatioCombined->AddEntry(ratioStat, "Systematic Uncertainty", "f");
            legendErrorKeyRatioCombined->AddEntry(ratioSyst, "Statistical Uncertainty", "e2");
            legendErrorKeyRatioCombined->Draw("same");
        }
    }


    legendErrorKeyRatioCombined->Draw("same");
    drawLatexAdd("p#minusPb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.95,0.91, 0.75*textsizeLabels,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.95,0.86, 0.75*textsizeLabels,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.95,0.81, 0.75*textsizeLabels,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.95,0.76, 0.75*textsizeLabels,kFALSE, kFALSE, true);

    canvas->SaveAs(Form("%s/xsectionratios.%s",output.Data(),fileType.Data()));
    
{
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

        TGraphErrors *spectrumStat = (TGraphErrors*)vecSpectrumStat[0].at(radius-minradius)->Clone(Form("spectrumStat_R0%i", radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)vecSpectrumSyst[0].at(radius-minradius)->Clone(Form("spectrumSyst_R0%i", radius));
        TGraphErrors *powhegStat = (TGraphErrors*)vecPowhegStat[0].at(radius-minradius)->Clone(Form("powhegStat_R0%i", radius));
        TGraphErrors *powhegSyst = (TGraphErrors*)vecPowhegSyst[0].at(radius-minradius)->Clone(Form("powhegSyst_R0%i", radius));
        TGraphErrors *pythiaStat = (TGraphErrors*)vecPythiaStat[0].at(radius-minradius)->Clone(Form("pythiaStat_R0%i", radius));

        TGraphErrors *ratioPythiaStatData = (TGraphErrors*)pythiaStat->Clone(Form("ratioPythiaStatData_R0%i", radius));
        TGraphErrors *ratioPowhegStatData = (TGraphErrors*)powhegStat->Clone(Form("ratioPowhegStatData_R0%i", radius));
        TGraphErrors *ratioPowhegSystData = (TGraphErrors*)powhegSyst->Clone(Form("ratioPowhegSystData_R0%i", radius));
        TGraphErrors *ratioDataDataStat   = (TGraphErrors*)spectrumStat->Clone(Form("ratioDataDataStat_R0%i", radius));
        TGraphErrors *ratioDataDataSyst   = (TGraphErrors*)spectrumSyst->Clone(Form("ratioDataDataSyst_R0%i", radius));

        for(int i = 0; i < spectrumStat->GetN(); i++){
            Double_t xDataStat, yDataStat, exDataStat, eyDataStat;
            Double_t xDataSyst, yDataSyst, exDataSyst, eyDataSyst;
            Double_t xPythiaStat, yPythiaStat, exPythiaStat, eyPythiaStat;
            Double_t xPowhegStat, yPowhegStat, exPowhegStat, eyPowhegStat;
            Double_t xPowhegSyst, yPowhegSyst, exPowhegSyst, eyPowhegSyst;

            spectrumStat->GetPoint(i, xDataStat, yDataStat);
            exDataStat = spectrumStat->GetErrorX(i);
            eyDataStat = spectrumStat->GetErrorY(i)/yDataStat;

            spectrumSyst->GetPoint(i, xDataSyst, yDataSyst);
            exDataSyst = spectrumSyst->GetErrorX(i);
            eyDataSyst = spectrumSyst->GetErrorY(i)/yDataSyst;

            pythiaStat->GetPoint(i, xPythiaStat, yPythiaStat);
            exPythiaStat = pythiaStat->GetErrorX(i);
            eyPythiaStat = pythiaStat->GetErrorY(i)/yPythiaStat;

            powhegStat->GetPoint(i, xPowhegStat, yPowhegStat);
            exPowhegStat = powhegStat->GetErrorX(i);
            eyPowhegStat = powhegStat->GetErrorY(i)/yPowhegStat;

            powhegSyst->GetPoint(i, xPowhegSyst, yPowhegSyst);
            exPowhegSyst = powhegSyst->GetErrorX(i);
            eyPowhegSyst = powhegSyst->GetErrorY(i)/yPowhegSyst;

            if(xDataStat != xPythiaStat || xDataStat != xPowhegStat || xDataStat != xPowhegSyst || xDataStat != xDataSyst) {
                printf("Error: xData != xMC\n");
                continue;
            }

            ratioPythiaStatData->SetPoint(i, xDataStat, yPythiaStat/yDataStat);
            ratioPythiaStatData->SetPointError(i, exPythiaStat, eyPythiaStat*(yPythiaStat/yDataStat));
            ratioPowhegStatData->SetPoint(i, xDataStat, yPowhegStat/yDataStat);
            ratioPowhegStatData->SetPointError(i, exPowhegStat, eyPowhegStat*(yPowhegStat/yDataStat));
            ratioPowhegSystData->SetPoint(i, xDataSyst, yPowhegSyst/yDataSyst);
            ratioPowhegSystData->SetPointError(i, exPowhegSyst, eyPowhegSyst*(yPowhegSyst/yDataSyst));
            ratioDataDataStat->SetPoint(i, xDataStat, 1);
            ratioDataDataStat->SetPointError(i, exDataStat, eyDataStat);
            ratioDataDataSyst->SetPoint(i, xDataSyst, 1);
            ratioDataDataSyst->SetPointError(i, exDataSyst, eyDataSyst);
        }

        DrawGammaLines(0.,260.,1.,1.,8.,16,9);
        //ratioPowhegSystData->Draw("e3,same");
        ratioDataDataSyst->Draw("e2,same");
        ratioDataDataStat->Draw("p,e1,same");
        ratioPythiaStatData->Draw("l,e3,same");
        ratioPowhegStatData->Draw("l,e3,same");
        dummyHist->Draw("axis,same");

        if(radius==maxradius){
            drawLatexAdd(Form("#it{R} = 0.%i", radius),0.25,0.31, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.94,0.88, 0.85*textsizeLabels,kFALSE, kFALSE, true);
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

    ///////////////////////////////////////
    // Plot cross-section ratios with pp //
    ///////////////////////////////////////
{
    TLegend *legendErrorKey;

    Double_t textsizeLabels = 0;
    Double_t textsizeFac    = 0;
    Double_t textSizeLabelsPixel = 750*0.05;

    Double_t arrayBoundariesX[4];
    Double_t arrayBoundariesY[2];
    Double_t relativeMarginsX[3];
    Double_t relativeMarginsY[3];

    ReturnCorrectValuesForCanvasScaling(2250,750, 3, 1,0.058, 0.003, 0.01,0.111,arrayBoundariesX,arrayBoundariesY,relativeMarginsX,relativeMarginsY);
    Double_t margin = relativeMarginsX[0]*2.7*1350;

    TCanvas *canvas = new TCanvas("canvasSimppvppb","",0,0,2250,750);;
    TPad    *ratioPad[3];
    TH2F    *dummyHist;

    gStyle->SetOptStat(0);

    TLegend *legendErrorKeyRatioCombined;

    DrawGammaCanvasSettings( canvas,  0.05, 0.01, 0.01,0.095);
    canvas->cd();

    ratioPad[0] = new TPad("padR02", "", arrayBoundariesX[0], arrayBoundariesY[1], arrayBoundariesX[1], arrayBoundariesY[0],-1, -1, -2);
    ratioPad[1] = new TPad("padR03", "", arrayBoundariesX[1], arrayBoundariesY[1], arrayBoundariesX[2], arrayBoundariesY[0],-1, -1, -2);
    ratioPad[2] = new TPad("padR04", "", arrayBoundariesX[2], arrayBoundariesY[1], arrayBoundariesX[3], arrayBoundariesY[0],-1, -1, -2);

    DrawGammaPadSettings( ratioPad[0], relativeMarginsX[0], relativeMarginsX[1], relativeMarginsY[0], relativeMarginsY[2]);
    DrawGammaPadSettings( ratioPad[1], relativeMarginsX[1], relativeMarginsX[1], relativeMarginsY[0], relativeMarginsY[2]);
    DrawGammaPadSettings( ratioPad[2], relativeMarginsX[1], relativeMarginsX[2], relativeMarginsY[0], relativeMarginsY[2]);

    ratioPad[0]->Draw();
    ratioPad[1]->Draw();
    ratioPad[2]->Draw();


    if (ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) < ratioPad[0]->YtoPixel(ratioPad[0]->GetY1()) ){
      	textsizeLabels              = (Double_t)textSizeLabelsPixel/ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) ;
      	textsizeFac                 = (Double_t)1./ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) ;
    }else{
      	textsizeLabels              = (Double_t)textSizeLabelsPixel/ratioPad[0]->YtoPixel(ratioPad[0]->GetY1());
      	textsizeFac                 = (Double_t)1./ratioPad[0]->YtoPixel(ratioPad[0]->GetY1());
    }

    dummyHist = new TH2F("dummyHist2","dummyHist", 1000, minPt,maxPt, 1000., 0.01,1.65);
    dummyHist->GetYaxis()->SetRangeUser(0.,1.6);
    SetStyleHistoTH2ForGraphs(dummyHist, "#it{p}_{T} (GeV/#it{c})", "#frac{d^{2}#it{#sigma}^{#it{R}=0.2}}{d^{2}#it{#sigma}^{#it{R}=0.X}}", 0.85*textsizeLabels, 0.95*textsizeLabels, 0.85*textsizeLabels, 0.95*textsizeLabels, 1, 1.35,512,505);


    TLegend *legendRatio;

    for(int radius = minradius+1; radius <= maxradius; radius++){
        ratioPad[radius-(minradius+1)]->cd();
        dummyHist->DrawCopy();
        DrawGammaLines(minPt,maxPt,1.,1.,8.,16,9);

        TGraphErrors *ratioStat_pp = (TGraphErrors*)vecRatioStat[0].at(radius-minradius-1)->Clone(Form("ratioStat_pp_R0%i", radius));
        TGraphErrors *ratioSyst_pp = (TGraphErrors*)vecRatioSyst[0].at(radius-minradius-1)->Clone(Form("ratioSyst_pp_R0%i", radius));

        TGraphErrors *ratioStat_pPb = (TGraphErrors*)vecRatioStat[2].at(radius-minradius-1)->Clone(Form("ratioStat_pPb_R0%i", radius));
        TGraphErrors *ratioSyst_pPb = (TGraphErrors*)vecRatioSyst[2].at(radius-minradius-1)->Clone(Form("ratioSyst_pPb_R0%i", radius));
        ratioStat_pPb = FormatSpectrum(ratioStat_pPb,radius,false,false,false,true);
        ratioSyst_pPb = FormatSpectrum(ratioSyst_pPb,radius,true,false,false,true);


        ratioStat_pp->Scale(1.2);
        ratioSyst_pp->Scale(1.2);

        ratioSyst_pp->Draw("e2,same");
        ratioStat_pp->Draw("p,e1,same");

        ratioSyst_pPb->Draw("e2,same");
        ratioStat_pPb->Draw("p,e1,same");
    

        if(radius==minradius+1){
            legendErrorKey = GetAndSetLegend2(0.19,0.16,0.35,0.14+(2)*(0.85*textsizeLabels),0.85*textsizeLabels);
            legendErrorKey->AddEntry(ratioSyst_pp, "Systematic Uncertainty", "f");
            legendErrorKey->AddEntry(ratioStat_pp, "Statistical Uncertainty", "e");
            legendErrorKey->Draw("same");
        }
        
        if(radius==minradius+1) legendRatio = GetAndSetLegend2(0.19,0.86,0.35,0.86+(2)*(0.85*textsizeLabels),0.85*textsizeLabels);
        if(radius==minradius+2) legendRatio = GetAndSetLegend2(0.04,0.86,0.2,0.86+(2)*(0.85*textsizeLabels),0.85*textsizeLabels);
        if(radius==minradius+3) legendRatio = GetAndSetLegend2(0.04,0.86,0.2,0.86+(2)*(0.85*textsizeLabels),0.85*textsizeLabels);

        legendRatio->AddEntry(ratioStat_pp, "pp #sqrt{#it{s}} = 8 TeV", "p");
        legendRatio->AddEntry(ratioStat_pPb, "p#minusPb #sqrt{#it{s}_{NN}} = 8.16 TeV", "p");
        legendRatio->Draw("same");

        drawLatexAdd(Form("#it{R} = 0.2 / #it{R} = 0.%i", radius),0.95,0.91, 0.85*textsizeLabels,kFALSE, kFALSE, true);

        if(radius==maxradius){
            drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.95,0.26, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.95,0.21, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.95,0.16, 0.85*textsizeLabels,kFALSE, kFALSE, true);
        }

        dummyHist->Draw("axis,same");
    }

    canvas->SaveAs(Form("%s/xsectionratios_ppVSpPb.%s",output.Data(),fileType.Data()));
}
}
