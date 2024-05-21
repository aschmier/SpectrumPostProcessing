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

void plotJESComp(TString jesfile0, TString jesfile1, TString outputdir, TString fileType = "pdf")
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 5;
    TString jetType = "Full";

    int styles_empty[11] = {4,25,27,28,35,36,38,40,42,44,46};
    int styles_filled[11] = {8,21,33,34,48,49,41,43,45,47,22};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4, kRed};
    vector<TGraphErrors*> vecMean0;
    vector<TGraphErrors*> vecMedian0;
    vector<TGraphErrors*> vecWidth0;
    vector<TGraphErrors*> vecMean1;
    vector<TGraphErrors*> vecMedian1;
    vector<TGraphErrors*> vecWidth1;


    TFile *fjes0 = TFile::Open(jesfile0);
    if(!fjes0 || fjes0->IsZombie()){
        cout << "First JES file not found!" << endl;
        return;
    }

    TFile *fjes1 = TFile::Open(jesfile1);
    if(!fjes1 || fjes1->IsZombie()){
        cout << "Second JES file not found!" << endl;
        return;
    }


    for(int radius = minradius; radius <= maxradius; radius++){
            TGraphErrors *mean0   = (TGraphErrors*)fjes0->Get(Form("EnergyScale_R0%i_mean", radius));
            TGraphErrors *median0 = (TGraphErrors*)fjes0->Get(Form("EnergyScale_R0%i_median", radius));
            TGraphErrors *width0  = (TGraphErrors*)fjes0->Get(Form("EnergyScale_R0%i_width", radius));
            vecMean0.push_back(mean0);
            vecMedian0.push_back(median0);
            vecWidth0.push_back(width0);

            TDirectory *rDir = (TDirectory*)fjes1->Get(Form("R0%i",radius));
            TGraphErrors *mean1   = (TGraphErrors*)rDir->Get(Form("meanAll_R0%i", radius));
            TGraphErrors *median1 = (TGraphErrors*)rDir->Get(Form("medianAll_R0%i", radius));
            TGraphErrors *width1  = (TGraphErrors*)rDir->Get(Form("widthAll_R0%i", radius));
            vecMean1.push_back(mean1);
            vecMedian1.push_back(median1);
            vecWidth1.push_back(width1);
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.1;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend      = GetAndSetLegend2(0.15,0.15,0.55,0.15+((vecMean0.size()*2)*textSize*1.5)/2,textSize,2);
    TLegend *legendwidth = GetAndSetLegend2(0.25,0.65,0.65,0.65+((vecMean0.size()*2)*textSize*1.5)/2,textSize,2);

    // Mean of JES
    TH1D *dummyMean   = (TH1D*)vecMean0.at(0)->GetHistogram();
    dummyMean->GetYaxis()->SetRangeUser(-0.35,-0.23);
    dummyMean->GetXaxis()->SetRangeUser(20,240);
    SetStyleHistoTH1ForGraphs(dummyMean,"","p_{T}^{part} [GeV/c]","<(p_{T}^{det}-p_{T}^{part})/p_{T}^{part}>",0.03,0.04,0.03,0.04,1,1.2);
    dummyMean->Draw();

    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==2 || radius==3 || radius==4) vecMean0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,240);
        if(radius==5) vecMean0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,160);
        if(radius==6) vecMean0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,130);

        vecMean0.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecMean0.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecMean0.at(radius-minradius)->SetMarkerStyle(styles_empty[radius-minradius]);
        vecMean0.at(radius-minradius)->Draw("p,e1,same");
        legend->AddEntry(vecMean0.at(radius-minradius), Form("8TeV,  R=0.%i",radius), "p");

        int colornum = vecMean0.size() + (radius-minradius);
        vecMean1.at(radius-minradius)->SetLineColor(colors[colornum]);
        vecMean1.at(radius-minradius)->SetMarkerColor(colors[colornum]);
        vecMean1.at(radius-minradius)->SetMarkerStyle(styles_filled[radius-minradius]);
        vecMean1.at(radius-minradius)->Draw("p,e1,same");
        legend->AddEntry(vecMean1.at(radius-minradius), Form("13TeV, R=0.%i",radius), "p");
    }
    legend->Draw();
    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.85, textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/EnergyScale/EnergyScaleMean_Comparison.%s", outputdir.Data(),fileType.Data()));
    legend->Clear();

    // Median of JES
    TH1D *dummyMedian   = (TH1D*)vecMedian0.at(0)->GetHistogram();
    dummyMedian->GetYaxis()->SetRangeUser(-0.35,-0.22);
    dummyMedian->GetXaxis()->SetRangeUser(20,240);
    SetStyleHistoTH1ForGraphs(dummyMedian,"","p_{T}^{part} [GeV/c]","<(p_{T}^{det}-p_{T}^{part})/p_{T}^{part}>",0.03,0.04,0.03,0.04,1,1.2);
    dummyMedian->Draw();

    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==2 || radius==3 || radius==4) vecMedian0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,240);
        if(radius==5) vecMedian0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,160);
        if(radius==6) vecMedian0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,130);

        vecMedian0.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecMedian0.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecMedian0.at(radius-minradius)->SetMarkerStyle(styles_empty[radius-minradius]);
        vecMedian0.at(radius-minradius)->Draw("p,e1,same");
        legend->AddEntry(vecMedian0.at(radius-minradius), Form("8TeV,  R=0.%i",radius), "p");

        vecMedian1.at(radius-minradius)->SetLineColor(colors[(vecMedian0.size() + (radius-minradius))]);
        vecMedian1.at(radius-minradius)->SetMarkerColor(colors[(vecMedian0.size() + (radius-minradius))]);
        vecMedian1.at(radius-minradius)->SetMarkerStyle(styles_filled[radius-minradius]);
        vecMedian1.at(radius-minradius)->Draw("p,e1,same");
        legend->AddEntry(vecMedian1.at(radius-minradius), Form("13TeV, R=0.%i",radius), "p");
    }
    legend->Draw();
    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.85, textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/EnergyScale/EnergyScaleMedia_Comparison.%s", outputdir.Data(),fileType.Data()));
    legend->Clear();

    // Width of JES
    TH1D *dummyWidth   = (TH1D*)vecWidth0.at(0)->GetHistogram();
    dummyWidth->GetYaxis()->SetRangeUser(0.15,0.28);
    dummyWidth->GetXaxis()->SetRangeUser(20,240);
    SetStyleHistoTH1ForGraphs(dummyWidth,"","p_{T}^{part} [GeV/c]","#sigma((p_{T}^{det}-p_{T}^{part})/p_{T}^{part})",0.03,0.04,0.03,0.04,1,1.2);
    dummyWidth->Draw();

    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==2 || radius==3 || radius==4) vecWidth0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,240);
        if(radius==5) vecWidth0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,160);
        if(radius==6) vecWidth0.at(radius-minradius)->GetXaxis()->SetRangeUser(20,130);

        vecWidth0.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecWidth0.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecWidth0.at(radius-minradius)->SetMarkerStyle(styles_empty[radius-minradius]);
        vecWidth0.at(radius-minradius)->Draw("p,e1,same");
        legendwidth->AddEntry(vecWidth0.at(radius-minradius), Form("8TeV,  R=0.%i",radius), "p");

        vecWidth1.at(radius-minradius)->SetLineColor(colors[(vecWidth0.size() + (radius-minradius))]);
        vecWidth1.at(radius-minradius)->SetMarkerColor(colors[(vecWidth0.size() + (radius-minradius))]);
        vecWidth1.at(radius-minradius)->SetMarkerStyle(styles_filled[radius-minradius]);
        vecWidth1.at(radius-minradius)->Draw("p,e1,same");
        legendwidth->AddEntry(vecWidth1.at(radius-minradius), Form("13TeV, R=0.%i",radius), "p");
    }
    legendwidth->Draw();
    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.85, textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/EnergyScale/EnergyScaleWidth_Comparison.%s", outputdir.Data(),fileType.Data()));

}
