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

void plotJES(TString jes_file, TString outputdir, TString run = "", TString fileType = "pdf", int minradius = 2, int maxradius = 6)
{
    Double_t textSize     = 0.03;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14] = {1,2,3,4,6,7,8,9,28,30,40,41,46,49};
    vector<TGraphErrors*> vecMean;
    vector<TGraphErrors*> vecMedian;
    vector<TGraphErrors*> vecWidth;


    TFile *fjes = TFile::Open(jes_file);
    if(!fjes || fjes->IsZombie()){
        cout << "JES file not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
            TGraphErrors *mean   = (TGraphErrors*)fjes->Get(Form("EnergyScale_R0%i_mean", radius));
            TGraphErrors *median = (TGraphErrors*)fjes->Get(Form("EnergyScale_R0%i_median", radius));
            TGraphErrors *width  = (TGraphErrors*)fjes->Get(Form("EnergyScale_R0%i_width", radius));
            if(!mean || !median || !width){
                cout << "JES graphs not found!" << endl;
                return;
            }
            vecMean.push_back(mean);
            vecMedian.push_back(median);
            vecWidth.push_back(width);
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.1;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend =  GetAndSetLegend2(0.8,(0.92-(maxradius-1)*textSize),0.95,0.92,textSize,2);

    // Mean of JES
    TH1D *dummyMean   = (TH1D*)vecMean.at(0)->GetHistogram();
    dummyMean->GetYaxis()->SetRangeUser(-0.35,-0.23);
    dummyMean->GetXaxis()->SetRangeUser(20,240);
    SetStyleHistoTH1ForGraphs(dummyMean,"","p_{T}^{part} [GeV/c]","<(p_{T}^{det}-p_{T}^{part})/p_{T}^{part}>",0.03,0.04,0.03,0.04,1,1.2);
    dummyMean->Draw();

    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==2 || radius==3 || radius==4) vecMean.at(radius-minradius)->GetXaxis()->SetRangeUser(20,240);
        if(radius==5) vecMean.at(radius-minradius)->GetXaxis()->SetRangeUser(20,160);
        if(radius==6) vecMean.at(radius-minradius)->GetXaxis()->SetRangeUser(20,130);

        vecMean.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecMean.at(radius-minradius)->Draw("p,e1,same");
        legend->AddEntry(vecMean.at(radius-minradius), Form("R=0.%i",radius), "p");
    }
    legend->Draw();
    drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV",0.6,0.88, 0.03, false, false, false);
    //drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.63,0.88, 0.03, false, false, false);
    drawLatexAdd(Form("%s Jets", jetType.Data()),0.6,0.84, 0.03, false, false, false);
    if(run=="") canvas->SaveAs(Form("%s/EnergyScale/EnergyScaleMean.%s", outputdir.Data(),fileType.Data()));
    else canvas->SaveAs(Form("%s/EnergyScale/Runwise/EnergyScaleMean_%s.%s", outputdir.Data(),run.Data(),fileType.Data()));
    legend->Clear();

    // Median of JES
    TH1D *dummyMedian   = (TH1D*)vecMedian.at(0)->GetHistogram();
    dummyMedian->GetYaxis()->SetRangeUser(-0.35,-0.22);
    dummyMedian->GetXaxis()->SetRangeUser(20,240);
    SetStyleHistoTH1ForGraphs(dummyMedian,"","p_{T}^{part} [GeV/c]","<(p_{T}^{det}-p_{T}^{part})/p_{T}^{part}>",0.03,0.04,0.03,0.04,1,1.2);
    dummyMedian->Draw();

    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==2 || radius==3 || radius==4) vecMedian.at(radius-minradius)->GetXaxis()->SetRangeUser(20,240);
        if(radius==5) vecMedian.at(radius-minradius)->GetXaxis()->SetRangeUser(20,160);
        if(radius==6) vecMedian.at(radius-minradius)->GetXaxis()->SetRangeUser(20,130);
        vecMedian.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecMedian.at(radius-minradius)->Draw("p,e1,same");
        legend->AddEntry(vecMedian.at(radius-minradius), Form("R=0.%i",radius), "p");
    }
    legend->Draw();
    //drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.6,0.88, 0.03, false, false, false);
    drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV",0.63,0.88, 0.03, false, false, false);
    drawLatexAdd(Form("%s Jets", jetType.Data()),0.6,0.84, 0.03, false, false, false);
    if(run=="") canvas->SaveAs(Form("%s/EnergyScale/EnergyScaleMedian.%s", outputdir.Data(),fileType.Data()));
    else canvas->SaveAs(Form("%s/EnergyScale/Runwise/EnergyScaleMedian_%s.%s", outputdir.Data(),run.Data(),fileType.Data()));
    legend->Clear();

    // Width of JES
    TH1D *dummyWidth   = (TH1D*)vecWidth.at(0)->GetHistogram();
    dummyWidth->GetYaxis()->SetRangeUser(0.15,0.28);
    dummyWidth->GetXaxis()->SetRangeUser(20,240);
    SetStyleHistoTH1ForGraphs(dummyWidth,"","p_{T}^{part} [GeV/c]","#sigma((p_{T}^{det}-p_{T}^{part})/p_{T}^{part})",0.03,0.04,0.03,0.04,1,1.2);
    dummyWidth->Draw();

    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==2 || radius==3 || radius==4) vecWidth.at(radius-minradius)->GetXaxis()->SetRangeUser(20,240);
        if(radius==5) vecWidth.at(radius-minradius)->GetXaxis()->SetRangeUser(20,160);
        if(radius==6) vecWidth.at(radius-minradius)->GetXaxis()->SetRangeUser(20,130);
        vecWidth.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecWidth.at(radius-minradius)->Draw("p,e1,same");
        legend->AddEntry(vecWidth.at(radius-minradius), Form("R=0.%i",radius), "p");
    }
    legend->Draw();
    //drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.63,0.88, 0.03, false, false, false);
    drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV",0.6,0.88, 0.03, false, false, false);
    drawLatexAdd(Form("%s Jets", jetType.Data()),0.6,0.84, 0.03, false, false, false);
    if(run=="") canvas->SaveAs(Form("%s/EnergyScale/EnergyScaleWidth.%s", outputdir.Data(),fileType.Data()));
    else canvas->SaveAs(Form("%s/EnergyScale/Runwise/EnergyScaleWidth_%s.%s", outputdir.Data(),run.Data(),fileType.Data()));}
