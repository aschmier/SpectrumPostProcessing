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

void plotExperimentComparison(TString alicefiles, TString atlasfiles, TString output, TString fileType, int minradius, int maxradius)
{
    Double_t textSize     = 0.03;
    int radius = 4;

    int scale[5] = {1,3,10,30,100};

    int styles[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizes[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    double alphasval = 0.25;

    Color_t coloralice   = kGreen+2;
    Color_t coloratlas05 = kOrange+1;
    Color_t coloratlas51 = kBlack;

    vector<TGraphErrors*> vecSpectrum8TeV;
    vector<TGraphErrors*> vecSpectrumSys8TeV;
    vector<TGraphErrors*> vecSpectrumAtlas05;
    vector<TGraphErrors*> vecSpectrumAtlas51;

    gSystem->Exec("mkdir -p "+output+"/ExperimentComparison");

    // Open root files
    TFile *fresults = TFile::Open(alicefiles);
    if(!fresults || fresults->IsZombie()){
        cout << "Default alice file not found!" << endl;
        return;
    }

    // Open root files
    TFile *fresultsatlas = TFile::Open(atlasfiles);
    if(!fresultsatlas || fresultsatlas->IsZombie()){
        cout << "Default atlas file not found!" << endl;
        return;
    }

    // ALICE
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *rdir = (TDirectory*)fresults->Get(Form("R0%i",radius));
        TGraphErrors *spectrum     = (TGraphErrors*)rdir->Get(Form("finalSpectrum_R0%i",radius));
        TGraphErrors *spectrumSys  = (TGraphErrors*)rdir->Get(Form("finalSpectrumSys_R0%i",radius));

        for(int point = 0; point < spectrum->GetN(); point++){
            double staterror = spectrum->GetErrorY(point);
            spectrum->SetPointError(point,0,staterror);
            double syserror = spectrumSys->GetErrorY(point);
            spectrumSys->SetPointError(point,spectrumSys->GetErrorX(point),syserror);
        }

        // Remove points
        for(int point = (spectrum->GetN())-1; point >= 0; point--){
            double x,y;
            spectrum->GetPoint(point,x,y);
            if(x < 20 || x > 240){
                spectrum->RemovePoint(point);
                spectrumSys->RemovePoint(point);
                point--;
            }
        }

        spectrum->SetMarkerStyle(styles[radius-minradius]);
        spectrum->SetMarkerSize(sizes[radius-minradius]);
        spectrum->SetMarkerColor(coloralice);
        spectrum->SetLineColor(coloralice);

        spectrumSys->SetFillColorAlpha(coloralice,alphasval);
        spectrumSys->SetLineColor(coloralice);
        spectrumSys->SetFillStyle(1000);

        //spectrum->Scale(1./spectrum->Integral());
        //spectrumSys->Scale(1./spectrumSys->Integral());

        vecSpectrum8TeV.push_back(spectrum);
        vecSpectrumSys8TeV.push_back(spectrumSys);
    }

    // ATLAS
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *etadir05 = (TDirectory*)fresultsatlas->Get("atlas8TeVeta05");
        TDirectory *etadir51 = (TDirectory*)fresultsatlas->Get("atlas8TeVeta51");
        TDirectory *rdir05 = (TDirectory*)etadir05->Get(Form("R0%i",radius));
        TDirectory *rdir51 = (TDirectory*)etadir51->Get(Form("R0%i",radius));
        TGraphErrors *spectrum05 = (TGraphErrors*)rdir05->Get("spectrumStat");
        TGraphErrors *spectrum51 = (TGraphErrors*)rdir51->Get("spectrumStat");

        //for(int point = 0; point < spectrum05->GetN(); point++){
         //   double staterror05 = spectrum05->GetErrorY(point);
        //    spectrum05->SetPointError(point,0,staterror05);
        //}

        //for(int point = 0; point < spectrum51->GetN(); point++){
        //    double staterror51 = spectrum51->GetErrorY(point);
        //    spectrum51->SetPointError(point,0,staterror51);
        //}

        // Remove points
        for(int point = (spectrum05->GetN())-1; point >= 0; point--){
            double x,y;
            spectrum05->GetPoint(point,x,y);
            if(x < 20 || x > 240){
                spectrum05->RemovePoint(point);
                point--;
            }
        }

        // Remove points
        for(int point = (spectrum51->GetN())-1; point >= 0; point--){
            double x,y;
            spectrum51->GetPoint(point,x,y);
            if(x < 20 || x > 240){
                spectrum51->RemovePoint(point);
                point--;
            }
        }

        spectrum05->SetMarkerStyle(styles[radius-minradius]);
        spectrum05->SetMarkerSize(sizes[radius-minradius]);
        spectrum05->SetMarkerColor(coloratlas05);
        spectrum05->SetLineColor(coloratlas05);
        spectrum51->SetMarkerStyle(styles[radius-minradius]);
        spectrum51->SetMarkerSize(sizes[radius-minradius]);
        spectrum51->SetMarkerColor(coloratlas51);
        spectrum51->SetLineColor(coloratlas51);

        spectrum05->Scale(1./spectrum05->Integral());
        spectrum51->Scale(1./spectrum51->Integral());

        vecSpectrumAtlas05.push_back(spectrum05);
        vecSpectrumAtlas51.push_back(spectrum51);
    }

    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("cSpectrum", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();

    gStyle->SetOptStat(0);

    TLegend *legendSpectrum;

    TLegend *legendErrorKey =  GetAndSetLegend2(0.19,0.11,0.39,0.11+(2)*textSize,textSize);
    legendErrorKey->AddEntry(vecSpectrumSys8TeV.at(0), "Systematic Uncertainty", "f");
    legendErrorKey->AddEntry(vecSpectrum8TeV.at(0), "Statistical Uncertainty", "e");

    // Set up dummy histos for plotting
    TH1D *dummySpectrum = new TH1D("dummySpectrum","",350,0,350);
    dummySpectrum->GetXaxis()->SetRangeUser(0,350);
    dummySpectrum->GetYaxis()->SetRangeUser(2e-7,1e9);
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    TLine * line = new TLine (0,1,280,1);
    line->SetLineColor(14);
    line->SetLineWidth(3);
    line->SetLineStyle(7);

    cSpectrum->cd();
    ///////////////////////////////
    // Plot spectrum comparisons //
    ///////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        legendSpectrum    =  GetAndSetLegend2(0.71,0.82-(3)*textSize,0.91,0.82,textSize);

        TGraphErrors *finalSpectrum8TeV = (TGraphErrors*)vecSpectrum8TeV.at(radius-minradius)->Clone(Form("finalSpectrum8_R0%i", radius));
        TGraphErrors *finalSpectrumSys8TeV = (TGraphErrors*)vecSpectrumSys8TeV.at(radius-minradius)->Clone(Form("finalSpectrumSys8_R0%i", radius));
        legendSpectrum->AddEntry(finalSpectrum8TeV, "ALICE", "p");


        TGraphErrors *finalSpectrumAtlas05 = (TGraphErrors*)vecSpectrumAtlas05.at(radius-minradius)->Clone(Form("finalSpectrumAtlas05_R0%i", radius));
        TGraphErrors *finalSpectrumAtlas51 = (TGraphErrors*)vecSpectrumAtlas51.at(radius-minradius)->Clone(Form("finalSpectrumAtlas51_R0%i", radius));
        legendSpectrum->AddEntry(finalSpectrumAtlas05, "ATLAS: 0.0 < #abs{#eta} < 0.5", "p");
        legendSpectrum->AddEntry(finalSpectrumAtlas51, "ATLAS: 0.5 < #abs{#eta} < 1.0", "p");

        dummySpectrum->Draw("axis");
        finalSpectrum8TeV->Draw("p,e1,same");
        finalSpectrumSys8TeV->Draw("e2,same");
        finalSpectrumAtlas05->Draw("p,e1,same");
        finalSpectrumAtlas51->Draw("p,e1,same");

        legendSpectrum->Draw("same");
        legendErrorKey->Draw("same");

        drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.%i", radius),0.93,0.9, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

        cSpectrum->SaveAs(Form("%s/SpectrumComparison_R0%i.%s",output.Data(),radius,fileType.Data()));

        legendSpectrum->Clear();
    }
}
