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
#include "/home/austin/alice/SpectrumPostProcessing/ParseHEPData.cxx"

struct Point {
  double fX, fY, fEX, fEY;
};

TGraphErrors* xTScale(TGraphErrors *g1, TGraphErrors *g2, double energyIN, double energyOUT){
    energyIN = energyIN * 1000.;
    energyOUT = energyOUT * 1000.;

     // Fit g2 with power law
    TF1 *powerlaw_g2 = new TF1("powerlaw_g2", "[0] * TMath::Power(x,[1])+[2]", 0.,  320.);
    //powerlaw_g2->SetParLimits(0, 60000, 120000);
    powerlaw_g2->SetParLimits(1, -5.5, -4.5);
    powerlaw_g2->FixParameter(2, 0);
    g2->Fit(powerlaw_g2, "", "", 20, 240);
    //g2->Draw();

    const int npoints = g1->GetN();
    std::array<Point, 11> points1;
    std::array<Point, 11> points2;
    // Fill array with points from TGraphErrors
    for(int i = 0; i < npoints; i++){
        double x, y, ex, ey;
        g1->GetPoint(i, x, y);
        ex = g1->GetErrorX(i);
        ey = g1->GetErrorY(i);
        points1[i] = {2*x/energyOUT, y, ex, ey};
        points2[i] = {2*x/energyIN, powerlaw_g2->Eval(x), ex, 0};
    }

    // Fill new TGraphErrors with scaled points
    //TGraphErrors *g2Scaled = new TGraphErrors;
    //int ip(0);
    //for(auto p : points2){
    //    g2Scaled->SetPoint(ip, p.fX, p.fY);
        //g2Scaled->SetPointError(ip, p.fEX, TMath::Sqrt(TMath::Power(p.fEY/p.fY, 2) + TMath::Power(points2.at(ip).fEY/points2.at(ip).fY, 2)));
    //    ip++;
    //}
    //g1->Draw();
    //g2Scaled->Draw("same");

    TGraphErrors *nXT = new TGraphErrors;
    int ip1(0);
    int ip2(0);
    for(auto p1 : points1){
        for(auto p2 : points2){
            if(points1.at(ip1).fX == points2.at(ip2).fX){
                nXT->SetPoint(ip1, points1.at(ip1).fX, log(points1.at(ip1).fY / points2.at(ip2).fY)/log(energyOUT/energyIN));
            }
            ip2++;
        }
        //nXT->SetPoint(ip, points1.at(ip).fX, log(points1.at(ip).fY / points2.at(ip).fY)/log(energyOUT/energyIN));
        //nXT->SetPointError(ip, 2*points1.at(ip).fEX/energyIN, TMath::Sqrt(TMath::Power(points1.at(ip).fEY/points1.at(ip).fY, 2) + TMath::Power(points2.at(ip).fEY/points2.at(ip).fY, 2))/log(energyIN/energyOUT));
        ip1++;
    }
    nXT->Draw();
/*
    TGraphErrors *specIN = new TGraphErrors;
    int ip(0);
    for(auto p : points){
        specIN->SetPoint(ip, 2*p.fX/energyIN, p.fY* TMath::Power(energyIN, 4.9));
        specIN->SetPointError(ip, 2*p.fEX/energyIN, p.fEY*TMath::Power(energyIN, 4.9));
        ip++;
    }

    // Fit with power law
    TF1 *powerlaw = new TF1("powerlaw", "[0] * TMath::Power(x,[1])+[2]", 0.,  1.);
    powerlaw->SetParLimits(0, 60000, 120000);
    powerlaw->SetParLimits(1, -5.5, -4.5);
    powerlaw->FixParameter(2, 0);
    specIN->Fit(powerlaw, "N", "", 3e-2, 1e-1);

    TCanvas *powerlawplot = new TCanvas("powerlawplot", "Power law fit");
    powerlawplot->cd();
    powerlawplot->SetLogy();
    powerlawplot->SetLogx();
    TH1 *axis = new TH1D("specaxis", ";x_{t}; #sqrt{s}^{4.9} d#sigma/dx_{t}", 100, 1e-3, 1.);
    axis->SetStats(false);
    axis->GetYaxis()->SetRangeUser(1e8, 1e15);
    axis->Draw("axis");
    specIN->SetMarkerStyle(20);
    specIN->SetMarkerColor(kBlack);
    specIN->SetLineColor(kBlack);
    specIN->Draw("epsame");
    powerlaw->SetLineColor(kRed);
    powerlaw->Draw("lsame");

    TPaveText *pave = new TPaveText(0.15, 0.15, 0.45, 0.22, "NDC");
    pave->SetBorderSize(0);
    pave->SetFillStyle(0);
    pave->SetTextFont(42);
    pave->AddText(Form("#sqrt{s} = %d TeV", energyIN));
    pave->Draw();

    for(auto p  : points) {
        std::cout << "Scaled cross section " << energyOUT << " TeV: pt = " << p.fX << ", " << powerlaw->Eval(2*p.fX / energyOUT)/TMath::Power(energyOUT, 4.9) << std::endl;
    }

    // Fill new TGraphErrors with scaled points
    TGraphErrors *specOUT = new TGraphErrors;
    ip = 0;
    for(auto p : points){
        specOUT->SetPoint(ip, p.fX, powerlaw->Eval(2*p.fX / energyOUT)/TMath::Power(energyOUT, 4.9));
        specOUT->SetPointError(ip, 2*p.fEX/energyOUT, p.fEY*TMath::Power(energyIN, 4.9)/TMath::Power(energyOUT, 4.9));
        ip++;
    }
*/
    return nXT;
}

void makeXTplots(TString file1, TString file2, TString folder3, double E1, double E2, double E3, TString output, TString filetype){
    Double_t textSize     = 0.03;

    int scale[5] = {1,3,10,30,100};

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.1,1.,1.5,1.2,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    double alphasval = 0.25;

    int minradius = 2;
    int maxradius = 2;

    Color_t colors[13]      = {kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    Color_t color8TeV    = kGreen+2;
    Color_t color5TeV    = kOrange+1;
    Color_t color13TeV   = kBlack;
    
    vector<TGraphErrors*> vecSpectrum[3];
    vector<TGraphErrors*> vecSpectrumSys[3];

    vector<TGraphErrors*> vecXT[3];
    
    gSystem->Exec("mkdir -p "+output);

     std::map<int, TString> mapSpectrum5TeV = {
        {2, "Table2"},
        {3, "Table3"},
        {4, "Table4"},
        {5, "Table5"},
        {6, "Table6"}
    };

    // Open root files
    TFile *fE1 = TFile::Open(file1);
    if(!fE1 || fE1->IsZombie()){
        cout << "E1 file not found!" << endl;
        return;
    }

    TFile *fE2 = TFile::Open(file2);
    if(!fE2 || fE2->IsZombie()){
        cout << "E2 file not found!" << endl;
        return;
    }

    // E1 (base energy)
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *rdir           = (TDirectory*)fE1->Get(Form("R0%i",radius));
        TGraphErrors *spectrumStat = (TGraphErrors*)rdir->Get(Form("finalSpectrum_R0%i",radius));
        TGraphErrors *spectrumSys  = (TGraphErrors*)rdir->Get(Form("finalSpectrumSys_R0%i",radius));

        for(int point = 0; point < spectrumStat->GetN(); point++){
            double staterror = spectrumStat->GetErrorY(point);
            spectrumStat->SetPointError(point,0,staterror);
            double syserror = spectrumSys->GetErrorY(point);
            spectrumSys->SetPointError(point,spectrumSys->GetErrorX(point),syserror);
        }

        spectrumStat->SetMarkerStyle(stylesfilled[radius-minradius]);
        spectrumStat->SetMarkerSize(sizesfilled[radius-minradius]);
        spectrumStat->SetMarkerColor(color8TeV);
        spectrumStat->SetLineColor(color8TeV);

        spectrumSys->SetFillColorAlpha(color8TeV,alphasval);
        spectrumSys->SetLineColor(color8TeV);
        spectrumSys->SetFillStyle(1);

        vecSpectrum[0].push_back((TGraphErrors*)spectrumStat);
        vecSpectrumSys[0].push_back((TGraphErrors*)spectrumSys);
    }

    // E2
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *rdir                 = (TDirectory*)fE2->Get(Form("R0%i",radius));
        if(!rdir) cout << "R0" << radius << " not found!" << endl;
        TH1D *spectrumStatTH1            = (TH1D*)rdir->Get(Form("stat_R0%i",radius));
        TGraphAsymmErrors *spectrumStat  = new TGraphAsymmErrors(spectrumStatTH1);
        TGraphAsymmErrors *spectrumCorr  = (TGraphAsymmErrors*)rdir->Get(Form("correlatedUncertainty_R0%i",radius));
        TGraphAsymmErrors *spectrumShape = (TGraphAsymmErrors*)rdir->Get(Form("shapeUncertainty_R0%i",radius));
        TGraphAsymmErrors *spectrumSys   = (TGraphAsymmErrors*)spectrumCorr->Clone(Form("combinedUncertaintyE2_R0%i", radius));

        for(int point = 0; point < spectrumSys->GetN(); point++){
            double syserror = sqrt(pow(spectrumCorr->GetErrorY(point),2) + pow(spectrumShape->GetErrorY(point),2));
            spectrumSys->SetPointEYhigh(point,syserror);
            spectrumSys->SetPointEYlow(point,syserror);
        }

        for(int point = 0; point < spectrumStat->GetN(); point++){
            double staterror = spectrumStat->GetErrorY(point);
            spectrumStat->SetPointEYhigh(point,staterror);
            spectrumStat->SetPointEYlow(point,staterror);
            spectrumStat->SetPointEXhigh(point,0);
            spectrumStat->SetPointEXlow(point,0);
            double syserror = spectrumSys->GetErrorY(point);
            spectrumSys->SetPointEYhigh(point,syserror);
            spectrumSys->SetPointEYlow(point,syserror);
        }

        spectrumStat->SetMarkerStyle(styles[radius-minradius]);
        spectrumStat->SetMarkerSize(sizesempty[radius-minradius]);
        spectrumStat->SetMarkerColor(color13TeV);
        spectrumStat->SetLineColor(color13TeV);

        spectrumSys->SetFillColorAlpha(color13TeV,alphasval);
        spectrumSys->SetLineColor(color13TeV);
        spectrumSys->SetFillStyle(1);

        for(int point = (spectrumSys->GetN())-1; point >= 0; point--){
            if(spectrumSys->GetPointX(point) > 320) spectrumSys->RemovePoint(point);
            if(spectrumSys->GetPointX(point) < 20) spectrumSys->RemovePoint(point);
        }

        for(int point = (spectrumStat->GetN())-1; point >= 0; point--){
            if(spectrumStat->GetPointX(point) > 320) spectrumStat->RemovePoint(point);
            if(spectrumStat->GetPointX(point) < 20) spectrumStat->RemovePoint(point);
        }

        vecSpectrum[1].push_back((TGraphErrors*)spectrumStat);
        vecSpectrumSys[1].push_back((TGraphErrors*)spectrumSys);
    }
cout << __LINE__ << endl;
    // E3
    for(int radius = minradius; radius <=maxradius; radius++){
        TString tableNameSpec = Form("%s/%s.tsv", folder3.Data(), (mapSpectrum5TeV[radius]).Data());
        TGraphAsymmErrors *spectrumStat  = ParseHEPData(tableNameSpec, 12, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
        TGraphAsymmErrors *spectrumCorr  = ParseHEPData(tableNameSpec, 12, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);
        TGraphAsymmErrors *spectrumShape = ParseHEPData(tableNameSpec, 12, 0, 1, 2, 3, 8, 9, kFALSE, kTRUE);
        //TGraphAsymmErrors *spectrum5TeV_lumi = ParseHEPData(tableNameSpec, 12, 0, 1, 2, 3, 10, 11, kFALSE, kTRUE);
        TGraphAsymmErrors *spectrumSys   = (TGraphAsymmErrors*)spectrumCorr->Clone(Form("combinedUncertaintyE3_R0%i", radius));

        for(int point = 0; point < spectrumSys->GetN(); point++){
            double syserror = sqrt(pow(spectrumCorr->GetErrorY(point),2) + pow(spectrumShape->GetErrorY(point),2));
            spectrumSys->SetPointEYhigh(point,syserror);
            spectrumSys->SetPointEYlow(point,syserror);
        }

        for(int point = 0; point < spectrumStat->GetN(); point++){
            double staterror = spectrumStat->GetErrorY(point);
            spectrumStat->SetPointEYhigh(point,staterror);
            spectrumStat->SetPointEYlow(point,staterror);
            spectrumStat->SetPointEXhigh(point,0);
            spectrumStat->SetPointEXlow(point,0);
            double syserror = spectrumSys->GetErrorY(point);
            spectrumSys->SetPointEYhigh(point,syserror);
            spectrumSys->SetPointEYlow(point,syserror);
        }

        spectrumStat->SetMarkerStyle(styles[radius-minradius]);
        spectrumStat->SetMarkerSize(sizesempty[radius-minradius]);
        spectrumStat->SetMarkerColor(color5TeV);
        spectrumStat->SetLineColor(color5TeV);

        spectrumSys->SetFillColorAlpha(color5TeV,alphasval);
        spectrumSys->SetLineColor(color5TeV);
        spectrumSys->SetFillStyle(1);

        vecSpectrum[2].push_back((TGraphErrors*)spectrumStat);
        vecSpectrumSys[2].push_back((TGraphErrors*)spectrumSys);
    }

    // xT scaling
    for(int i = 0; i < vecSpectrum[0].size(); i++){

        TGraphErrors *gN1 = xTScale(vecSpectrum[0].at(i), vecSpectrum[1].at(i), E2, E1);
        //TGraphErrors *gN2 = xTScale(vecSpectrum[0].at(i), vecSpectrum[2].at(i), E1, E3);

        //vecXT[0].push_back(gN1);
        //vecXT[1].push_back(gN2);
    }
/*
    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("cSpectrum", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();

    TCanvas *cRatio   = new TCanvas("c2", "", 800, 800);
    DrawPaperCanvasSettings(cRatio,0.14,0.025,0.025,0.08);

    gStyle->SetOptStat(0);

    TLegend *legendSpectrum;

    TLegend *legendRatio       = GetAndSetLegend2(0.63,0.94-(3)*textSize,0.83,0.94,textSize);

    //TLegend *legendErrorKey =  GetAndSetLegend2(0.19,0.11,0.39,0.11+(2)*textSize,textSize);
    //legendErrorKey->AddEntry(vecSpectrumSyspp.at(0), "Systematic Uncertainty", "f");
    //legendErrorKey->AddEntry(vecSpectrumpp.at(0), "Statistical Uncertainty", "e");

    //TLegend *legendErrorKeyRatio =  GetAndSetLegend2(0.6,0.11,0.8,0.11+(2)*textSize,textSize);
    //legendErrorKeyRatio->AddEntry(vecRatioSyspp.at(0), "Systematic Uncertainty", "f");
    //legendErrorKeyRatio->AddEntry(vecRatiopp.at(0), "Statistical Uncertainty", "e");

    // Set up dummy histos for plotting
    TH1D *dummySpectrum = new TH1D("dummySpectrum","",350,0,350);
    dummySpectrum->GetXaxis()->SetRangeUser(0,340);
    dummySpectrum->GetYaxis()->SetRangeUser(2e-8,4);
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    TH1D *dummyRatio = new TH1D("dummyRatio","",10000,0,1);
    dummyRatio->GetXaxis()->SetRangeUser(1e-4,1e-1);
    dummyRatio->GetYaxis()->SetRangeUser(0,10);
    SetStyleHistoTH1ForGraphs(dummyRatio,"","#it{x}_{T}","n(#it{x}_{T}, #sqrt{s_{1}}, #sqrt{s_{2}})",textSize,0.035,textSize,0.035,1,1.7);

    TLine * line = new TLine (0,1,340,1);
    line->SetLineColor(14);
    line->SetLineWidth(3);
    line->SetLineStyle(7);

    cRatio->cd();

    dummyRatio->Draw("axis");

    for(int i = 0; i < vecXT[0].size(); i++){
        for(int j = 0; j < 1; j++){
            vecXT[j].at(i)->SetMarkerStyle(stylesfilled[i]);
            vecXT[j].at(i)->SetMarkerSize(sizesfilled[i]);
            vecXT[j].at(i)->SetMarkerColor(colors[i]);
            vecXT[j].at(i)->SetLineColor(colors[i]);
            vecXT[j].at(i)->Draw("p,e1,same");
        }
    }

    cRatio->SaveAs(Form("%s/nCalc.%s", output.Data(), filetype.Data()));
    */
}