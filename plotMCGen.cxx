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

TH1D *ProcessSimHisto(TH1D *spectrum, vector<double> vecBins, int r, TString outlier, int style, Color_t color, double size){
    TH1D *hRebinned = (TH1D*)spectrum->Rebin(vecBins.size()-1, Form("hSim_R0%i_%s",r,outlier.Data()), vecBins.data());
    double radius = (0.1)*((double)r);
    hRebinned->Scale(1.,"width");
    hRebinned->Scale(1./(1.4 - 2*radius));
    hRebinned->SetMarkerStyle(style);
    hRebinned->SetMarkerSize(2.5);
    hRebinned->SetMarkerColor(color);
    hRebinned->SetLineColor(color);
    hRebinned->SetMarkerSize(size);
    return hRebinned;
}

void plotMCGen(TString fPYTHIA, TString output, double lumi = -1, TString fileExt = "pdf", int minradius = 2, int maxradius = 6, TString system = "pp", double energy = 13.6)
{
    // Macro setup
    Double_t textSize     = 0.03;
    double alphasval = 0.25;
    int scale[5] = {1,3,10,30,100};
    const int nOutlier = 4;
    double minJetPt = 10.;
    double maxJetPt = 320.;

    int stylesempty[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};
    int linestyles[4] = {7,3,4,10};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    Color_t simcolors[5] = {kGray+2, kPink-9, kOrange+2, kTeal-1, kAzure-1};

    TString outliers[nOutlier] = {"nooutlier","outlier30","outlier50","outlier70"};
    vector<double> trueRebin = getJetPtBinningNonLinTrue8TeVCourse();
    vector<TH1D*> vecSpectrumSimPythia[nOutlier];   

    // Create output directory
    gSystem->Exec(Form("mkdir -p %s/MCGen",output.Data())); 

    // Get generated pythia file
    TFile *fSimPythia = TFile::Open(fPYTHIA);
    if(!fSimPythia || fSimPythia->IsZombie()){
        cout << "Generated pythia file not found!" << endl;
        return;
    }

    // Get generated pythia for comparisons
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int outlier = 0; outlier < nOutlier; outlier++){
          TList *simBase = (TList*)fSimPythia->Get(Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers[outlier].Data()));
          if(!simBase){
            cout << Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers[outlier].Data()) << endl;
            cout << "No sim base found!" << endl;
            return;
          }
          TH1D *hSpec = (TH1D*)simBase->FindObject("hJetPt");
          TH1D *hSim = (TH1D*)ProcessSimHisto( hSpec, trueRebin, radius, outliers[outlier], stylesempty[radius-minradius+1], colors[radius-minradius+1], sizesempty[radius-minradius+1] );
          if(lumi > 0) hSim->Scale(lumi);
          vecSpectrumSimPythia[outlier].push_back(hSim);
        }
    }

    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("c", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();
    gStyle->SetOptStat(0);
    TLegend *legendSim      =  GetAndSetLegend2(0.63,0.89-3*textSize,0.88,0.89,textSize);

    // Set up dummy histos for plotting
    TH1D *dummySpectrum = new TH1D("dummySpectrum","",330,0,330);
    dummySpectrum->GetXaxis()->SetRangeUser(0,320);
    if(system=="pPb"){
        if(lumi > 0) dummySpectrum->GetYaxis()->SetRangeUser(3e-6*lumi,400*lumi);
        else dummySpectrum->GetYaxis()->SetRangeUser(3e-6,400);
    }
    if(system=="pp"){
        if(lumi > 0) dummySpectrum->GetYaxis()->SetRangeUser(3e-8*lumi,0.6*lumi);
        else dummySpectrum->GetYaxis()->SetRangeUser(3e-8,0.6);
    }
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    // Iterate and draw
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int outlier = 0; outlier < nOutlier; outlier++){
            TGraphErrors *graphSimPythia  = new TGraphErrors(vecSpectrumSimPythia[outlier].at(radius-minradius));

            graphSimPythia->SetMarkerColor(colors[11]);
            graphSimPythia->SetLineColor(colors[11]);

            for(int point = (graphSimPythia->GetN())-1; point >= 0; point--){
                if(graphSimPythia->GetPointX(point) > maxJetPt) graphSimPythia->RemovePoint(point);
                if(graphSimPythia->GetPointX(point) < minJetPt) graphSimPythia->RemovePoint(point);
            }
            
            legendSim->AddEntry(graphSimPythia, "PYTHIA8 Monash", "p");

            dummySpectrum->Draw("axis");
            graphSimPythia->Draw("p,e1,same");
            legendSim->Draw();

            //drawLatexAdd("ALICE Preliminary",0.93,0.92, 0.85*textsizeLabelsWidth,kFALSE, kFALSE, true);
            if(system=="PbPb") drawLatexAdd(Form("Pb#minusPb #sqrt{#it{s}_{NN}} = %.1f TeV", energy),0.93,0.93, 0.85*textSize,kFALSE, kFALSE, true);
            if(system=="pPb") drawLatexAdd(Form("p#minusPb #sqrt{#it{s}_{NN}} = %.1f TeV", energy),0.93,0.93, 0.85*textSize,kFALSE, kFALSE, true);
            if(system=="pp") drawLatexAdd(Form("pp #sqrt{#it{s}} = %.1f TeV", energy),0.93,0.93, 0.85*textSize,kFALSE, kFALSE, true);
            drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.%i",radius),0.93,0.89, 0.85*textSize,kFALSE, kFALSE, true);
            
            cSpectrum->SaveAs(Form("%s/MCGen/MCComp_R0%i_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileExt.Data()));
            legendSim->Clear();
        }
    }

}