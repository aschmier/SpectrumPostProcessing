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

TGraphErrors* xTScale(TGraphErrors *g, double energyIN, double energyOUT){
    energyIN = energyIN * 1000.;
    energyOUT = energyOUT * 1000.;
    double n = 3.2;

    int npoints = g->GetN();
    //const std::size_t size = npoints;
    std::array<Point, 20> points;
    // Fill array with points from TGraphErrors
    for(int i = 0; i < 20; i++){
        double x, y, ex, ey;
        if(i >= npoints) points[i] = {0., 0., 0., 0.};
        else{
            g->GetPoint(i, x, y);
            ex = g->GetErrorX(i);
            ey = g->GetErrorY(i);
            points[i] = {x, y, ex, ey};
        }

    }

  TGraphErrors *specIN = new TGraphErrors;
  int ip(0);
  for(auto p : points){
    specIN->SetPoint(ip, 2*p.fX/energyIN, p.fY* TMath::Power(energyIN, n));
    specIN->SetPointError(ip, 2*p.fEX/energyIN, p.fEY*TMath::Power(energyIN, n));
    ip++;
  }

  // Fit with power law
  TF1 *powerlaw = new TF1("powerlaw", "[0] * TMath::Power(x,[1])+[2]", 0.,  1.);
  powerlaw->SetParLimits(0, 60000, 120000);
  powerlaw->SetParLimits(1, -5.5, -4.5);
  powerlaw->FixParameter(2, 0);
  specIN->Fit(powerlaw, "N", "", 3e-2, 1e-1);
/*
  TCanvas *powerlawplot = new TCanvas("powerlawplot", "Power law fit");
  powerlawplot->cd();
  powerlawplot->SetLogy();
  powerlawplot->SetLogx();
  TH1 *axis = new TH1D("specaxis", ";x_{t}; #sqrt{s}^{n} d#sigma/dx_{t}", 100, 1e-3, 1.);
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
  pave->AddText(Form("pp #sqrt{s} = %f TeV", energyIN/1000.));
  pave->Draw();

  for(auto p  : points) {
    std::cout << "Scaled cross section 5 TeV: pt = " << p.fX << ", " << powerlaw->Eval(2*p.fX / energyOUT)/TMath::Power(energyOUT, n) << std::endl;
  }
*/
  return specIN;
}

void plotXTScaledSpectra(TString file1, TString file2, TString folder3, TString folder4, double E1, double E2, double E3, double E4, TString output, TString filetype, TString rootfiledir){
    Double_t textSize     = 0.03;

    double energies[3] = {E1, E2, E3};

    int scale[5] = {1,3,10,30,100};

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.1,1.,1.5,1.2,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    double alphasval = 0.25;

    int minradius = 2;
    int maxradius = 5;

    Color_t colors[13]      = {kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    Color_t color8TeV    = kGreen+2;
    Color_t color5TeV    = kOrange+1;
    Color_t color13TeV   = kBlack;
    Color_t color2p76TeV = kMagenta+2;

    
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

    TString spectrum2p76TeVR02 = "Table1";
    TString spectrum2p76TeVR04 = "Table3";

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

    TFile *fOutput = new TFile(Form("%s/pp_FullJet_Spectra.root",rootfiledir.Data()),"RECREATE");
    fOutput->mkdir("pp2p76TeV");
    fOutput->mkdir("pp8TeV");
    fOutput->mkdir("pp5TeV");
    fOutput->mkdir("pp13TeV");

    // E1 (base energy)
    fOutput->cd("pp8TeV");
    auto basedir = static_cast<TDirectory *>(gDirectory);
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

        basedir->mkdir(Form("R0%i",radius));
        basedir->cd(Form("R0%i",radius));

        spectrumSys->Write("spectrumSys");
        spectrumStat->Write("spectrumStat");
    }

    // E2
    fOutput->cd("pp13TeV");
    basedir = static_cast<TDirectory *>(gDirectory);
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

        basedir->mkdir(Form("R0%i",radius));
        basedir->cd(Form("R0%i",radius));

        spectrumSys->Write("spectrumSys");
        spectrumStat->Write("spectrumStat");
    }

    // E3
    fOutput->cd("pp5TeV");
    basedir = static_cast<TDirectory *>(gDirectory);
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

        basedir->mkdir(Form("R0%i",radius));
        basedir->cd(Form("R0%i",radius));

        spectrumSys->Write("spectrumSys");
        spectrumStat->Write("spectrumStat");
    }

    // E4
    fOutput->cd("pp2p76TeV");
    basedir = static_cast<TDirectory *>(gDirectory);
    for(int radius = minradius; radius <=maxradius; radius++){
        if(radius != 2 && radius != 4) continue;
        TString tableNameSpec = Form("%s/%s.tsv", folder4.Data(), radius == 2? spectrum2p76TeVR02.Data() : spectrum2p76TeVR04.Data());
        TGraphAsymmErrors *spectrumStat = ParseHEPData(tableNameSpec, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
        TGraphAsymmErrors *spectrumSys = ParseHEPData(tableNameSpec, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);

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
        spectrumStat->SetMarkerColor(color2p76TeV);
        spectrumStat->SetLineColor(color2p76TeV);

        spectrumSys->SetFillColorAlpha(color2p76TeV,alphasval);
        spectrumSys->SetLineColor(color2p76TeV);
        spectrumSys->SetFillStyle(1);

        //vecSpectrum[3].push_back(spectrumStat);
        //vecSpectrumSys[3].push_back(spectrumSys);

        basedir->mkdir(Form("R0%i",radius));
        basedir->cd(Form("R0%i",radius));
        
        spectrumSys->Write("spectrumSys");
        spectrumStat->Write("spectrumStat");
    }

    fOutput->Close();
    delete fOutput;

    // xT scaling
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int j = 0; j < 3; j++){
            //TGraphErrors *xTScaleGraph = xTScale(vecSpectrum[j].at(radius-minradius), energies[j], energies[0]);
            //if(!xTScaleGraph) cout << "xT scaling failed" << endl;
            vecXT[j].push_back(xTScale(vecSpectrum[j].at(radius-minradius), energies[j], energies[0]));

            cout << "XT scaled " << energies[j] << " to " << energies[0] << endl;
            for(int point = 0; point < vecXT[j].at(radius-minradius)->GetN(); point++){
                cout << "xT scaled point " << point << " " << vecXT[j].at(radius-minradius)->GetPointX(point) << " " << vecXT[j].at(radius-minradius)->GetPointY(point) << endl;
            }
        }
    }

    // Set up canvases and legends for plotting
    //TCanvas *cRatio   = new TCanvas("c2", "", 800, 800);
    //DrawPaperCanvasSettings(cRatio,0.14,0.025,0.025,0.08);
    //gStyle->SetOptStat(0);

    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("cSpectrum", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();
    cSpectrum->SetLogx();
    gStyle->SetOptStat(0);

    //TLegend *legendRatio       = GetAndSetLegend2(0.63,0.94-(3)*textSize,0.83,0.94,textSize);
    TLegend *legendSpectrum    =  GetAndSetLegend2(0.71,0.82-(4)*textSize,0.91,0.82,textSize);

    // Set up dummy histos for plotting
    //TH1D *dummyRatio = new TH1D("dummyRatio","",10000,0,1);
    //dummyRatio->GetXaxis()->SetRangeUser(1e-4,0.4);
    //dummyRatio->GetYaxis()->SetRangeUser(0,10);
    //SetStyleHistoTH1ForGraphs(dummyRatio,"","#it{x}_{T}","n(#it{x}_{T}, #sqrt{s_{1}}, #sqrt{s_{2}})",textSize,0.035,textSize,0.035,1,1.7);

    TH1D *dummySpectrum = new TH1D("dummySpectrum","",10000,0,1);
    dummySpectrum->GetXaxis()->SetRangeUser(0.002,0.1);
    dummySpectrum->GetYaxis()->SetRangeUser(2e5,2e11);
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{x}_{T}","(#sqrt{s})^{n} #frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    vector<TGraphErrors*> combinedXT;

    cSpectrum->cd();
    for(int radius = minradius; radius <= maxradius; radius++){
        combinedXT.push_back(new TGraphErrors());
        dummySpectrum->Draw();
        for(int j = 0; j < 3; j++){
            vecXT[j].at(radius-minradius)->SetMarkerStyle(stylesfilled[j]);
            vecXT[j].at(radius-minradius)->SetMarkerSize(sizesfilled[j]);
            vecXT[j].at(radius-minradius)->SetMarkerColor(colors[j]);
            vecXT[j].at(radius-minradius)->SetLineColor(colors[j]);
            vecXT[j].at(radius-minradius)->Draw("p,e1,same");
            if(j == 0) legendSpectrum->AddEntry(vecXT[j].at(radius-minradius), "pp 8TeV","p");
            if(j == 1) legendSpectrum->AddEntry(vecXT[j].at(radius-minradius), "pp 13TeV","p");
            if(j == 2) legendSpectrum->AddEntry(vecXT[j].at(radius-minradius), "pp 5TeV","p");
            for(int point = 0; point < vecXT[j].at(radius-minradius)->GetN(); point++){
                double x, y, ex, ey;
                vecXT[j].at(radius-minradius)->GetPoint(point, x, y);
                ex = vecXT[j].at(radius-minradius)->GetErrorX(point);
                ey = vecXT[j].at(radius-minradius)->GetErrorY(point);
                combinedXT[radius-minradius]->SetPoint(point,x,y);
                combinedXT[radius-minradius]->SetPointError(point,ex,ey);
            }
        }
        legendSpectrum->AddEntry(vecSpectrumSys[0].at(radius-minradius), "n = 3.2","");
        legendSpectrum->Draw();
        cSpectrum->SaveAs(Form("%s/xTSpectra_R0%i.%s", output.Data(), radius, filetype.Data()));
    }

    legendSpectrum->Clear();
    cout << "combinedXT size " << combinedXT.size() << endl;

    for(int radius = minradius; radius <= maxradius; radius++){
        // Plot combined xT spectra
        cSpectrum->cd();
        dummySpectrum->Draw();
        combinedXT[radius-minradius]->SetMarkerStyle(stylesfilled[0]);
        combinedXT[radius-minradius]->SetMarkerSize(sizesfilled[0]);
        combinedXT[radius-minradius]->SetMarkerColor(colors[0]);
        combinedXT[radius-minradius]->SetLineColor(colors[0]);
        combinedXT[radius-minradius]->Draw("p,e1,same");
        legendSpectrum->AddEntry(combinedXT[radius-minradius], "pp 8TeV","p");
        legendSpectrum->AddEntry(vecSpectrumSys[0].at(radius-minradius), "n = 3.2","");
        legendSpectrum->Draw();
        cSpectrum->SaveAs(Form("%s/combinedxTSpectra_R0%i.%s", output.Data(), radius, filetype.Data()));
    }



}