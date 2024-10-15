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
//#include "/home/austin/alice/AnalysisSoftware/CommonHeaders/PlottingGammaConversionHistos.h"
//#include "/home/austin/alice/AnalysisSoftware/CommonHeaders/PlottingGammaConversionAdditional.h"

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

TGraphErrors *FormatSpectrum(TGraphErrors *g, int radius, bool sys, bool pythia = false, bool powheg = false, int minradius = 2, int maxradius = 5){

    int stylesempty[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    Double_t textSize     = 0.03;
    Int_t regnum          = 6;
    double alphasval = 0.25;

    Color_t colors[12] = {kBlack, kRed+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+9, kTeal+2, kAzure-4, kViolet+5, kPink-4};
    int linestyles[4] = {7,3,4,10};

    for(int point = 0; point < g->GetN(); point++){
        double e = g->GetErrorY(point);
        if(sys) g->SetPointError(point,g->GetErrorX(point),e);
        else g->SetPointError(point,0,e);
    }

    if(sys || pythia || powheg){
        if(pythia){
            g->SetFillColorAlpha(colors[radius-minradius+maxradius-1],alphasval);
            g->SetLineColor(colors[radius-minradius+maxradius-1]);
            g->SetMarkerColor(colors[radius-minradius+maxradius-1]);
        }else if(powheg){
            g->SetFillColorAlpha(colors[radius-minradius+2*maxradius-2],alphasval);
            g->SetLineColor(colors[radius-minradius+2*maxradius-2]);
            g->SetMarkerColor(colors[radius-minradius+2*maxradius-2]);
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

void plotFinalSpectrum(TString spectrumFile, TString fPYTHIA, TString fPOWHEG, TString systematicsDir, TString type, TString output, TString fileType, int minradius = 2, int maxradius = 5, TString binning = "fine")
{
    Double_t textSize     = 0.03;
    Int_t regnum          = 6;
    double alphasval = 0.25;
    int linestyles[4] = {7,3,4,10};


    int scale[5] = {1,3,10,30,100};
    //const int nOutlier = 4;
    const int nOutlier = 1;

    int stylesempty[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    TString outliers = "nooutlier";

    vector<double> trueRebin;
    if(binning == "fine")   trueRebin = getJetPtBinningNonLinTrue8TeV();
    if(binning == "coarse") trueRebin = getJetPtBinningNonLinTrue8TeVCourse();

    vector<TH1D*> vecSpectrum;
    vector<TH1D*> vecSpectrumSys;
    vector<TH1D*> vecSystematics;
    vector<TH1D*> vecSystematicsRatios;
    vector<TH1D*> vecRatio;
    vector<TH1D*> vecRatioSys;
    vector<TH1D*> vecPythia;
    vector<TH1D*> vecPythiaRatio;
    vector<TH1D*> vecPowhegStat;
    vector<TH1D*> vecPowhegSyst;
    vector<TH1D*> vecPowhegStatRatio;
    vector<TH1D*> vecPowhegSystRatio;
    vector<TH1D*> vecAllUnfolding;
    vector<TH1D*> vecTriggerSwap;
    vector<TGraphErrors*> vecSpectrumGraph;
    vector<TGraphErrors*> vecSpectrumSysGraph;
    vector<TGraphErrors*> vecRatioGraph;
    vector<TGraphErrors*> vecRatioSysGraph;
    vector<TGraphErrors*> vecPythiaGraph;
    vector<TGraphErrors*> vecPythiaRatioGraph;
    vector<TGraphErrors*> vecPowhegStatGraph;
    vector<TGraphErrors*> vecPowhegSystGraph;
    vector<TGraphErrors*> vecPowhegStatRatioGraph;
    vector<TGraphErrors*> vecPowhegSystRatioGraph;

    TString outputDirRootFile = "/home/austin/alice/FinalResultsRootFiles";
    gSystem->Exec("mkdir -p "+outputDirRootFile);

    gSystem->Exec("mkdir -p "+output+"/FinalResults");
    gSystem->Exec("mkdir -p "+output+"/FinalResults/SingleSpectra");
    gSystem->Exec("mkdir -p "+output+"/FinalResults/SingleRatios");
    gSystem->Exec("mkdir -p "+output+"/FinalResults/RatioDataMC");

    const char* nameOutput = Form("%s/FinalResults_pp8TeV.root",outputDirRootFile.Data());
    const char* nameOutputCoarse = Form("%s/FinalResults_pp8TeV_Coarse.root",outputDirRootFile.Data());
    TFile* fOutput;
    if(binning == "fine") fOutput = new TFile(nameOutput,"RECREATE");
    if(binning == "coarse") fOutput = new TFile(nameOutputCoarse,"RECREATE");

    // Open root files
    TFile *f = TFile::Open(spectrumFile);
    if(!f || f->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    TFile *fSimPythia = TFile::Open(fPYTHIA);
    if(!fSimPythia || fSimPythia->IsZombie()){
        cout << "Generated PYTHIA file not found!" << endl;
        return;
    }

    TFile *fSimPowheg = TFile::Open(fPOWHEG);
    if(!fSimPowheg || fSimPowheg->IsZombie()){
        cout << "Generated POWHEG file not found!" << endl;
        return;
    }

    // Get systematics file
    for(int radius = minradius; radius <= maxradius; radius++){
        TString sysfile = Form("%s/systematics_R0%i.root", systematicsDir.Data(), radius);
        TFile *fSys = TFile::Open(sysfile);
        if(!fSys || fSys->IsZombie()){
            cout << "Systematics file not found! --> R=0." << radius << endl;
            return;
        }
        vecSystematics.push_back((TH1D*)fSys->Get("hTotal"));
        vecAllUnfolding.push_back((TH1D*)fSys->Get("smooth_allunfolding"));
        vecTriggerSwap.push_back((TH1D*)fSys->Get("smooth_triggerswap"));
    }

    // Get systematics file for ratios
    for(int radius = minradius+1; radius <= maxradius; radius++){
        TString sysfile = Form("%s/ratio/systematics_R02R0%i.root", systematicsDir.Data(), radius);
        TFile *fSys = TFile::Open(sysfile);
        if(!fSys || fSys->IsZombie()){
            cout << "Systematics file not found! --> R=0." << radius << endl;
            return;
        }
        vecSystematicsRatios.push_back((TH1D*)fSys->Get("hTotal"));
    }

    // Get spectra and ratios for results and systematics
    TH1D *spectrum_R02;
    TH1D *spectrumSys_R02;
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *regDir = (TDirectory*)f->Get(Form("R0%i",radius));
        TDirectory *reg    = (TDirectory*)regDir->Get(Form("reg%i",regnum));
        TH1D *spectrum     = (TH1D*)reg->Get(Form("normalized_reg%i",regnum));
        TH1D *spectrumSys  = (TH1D*)spectrum->Clone(Form("spectrumSys_%i",radius));
        vecSpectrum.push_back(spectrum);
        vecSpectrumSys.push_back(spectrumSys);

        if(radius==minradius){
            spectrum_R02    = (TH1D*)spectrum->Clone("spectrum_R02");
            spectrumSys_R02 = (TH1D*)spectrumSys->Clone("spectrumSys_R02");
        }
        if(radius != minradius){
            TH1D *ratio    = (TH1D*)spectrum->Clone(Form("ratio_R0%i",radius));
            ratio->Divide(spectrum_R02,spectrum);
            TH1D *ratioSys = (TH1D*)spectrum->Clone(Form("ratioSys_R0%i",radius));
            ratioSys->Divide(spectrumSys_R02,spectrumSys);
            vecRatio.push_back(ratio);
            vecRatioSys.push_back(ratioSys);
        }
    }

    // Get generated pythia for comparisons
    for(int radius = minradius; radius <= maxradius; radius++){
        TList *simBase = (TList*)fSimPythia->Get(Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers.Data()));
        if(!simBase){
            cout << Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers.Data()) << endl;
            cout << "No sim base found!" << endl;
            return;
        }
        TH1D *hSpec = (TH1D*)simBase->FindObject("hJetPt");
        TH1D *hSim = (TH1D*)ProcessSimHisto( hSpec, trueRebin, radius, outliers, stylesempty[radius-minradius+1], colors[radius-minradius+1], sizesempty[radius-minradius+1] );
        vecPythia.push_back(hSim);

        TH1D *hSimR02;
        if(radius == minradius) hSimR02 = (TH1D*)hSim->Clone("hSimR02"); 

        if(radius != 2){
            TH1D *hSimRatio = (TH1D*)hSim->Clone(Form("hSimRatio_R0%i",radius));
            hSimRatio->Divide(hSimR02,hSim);
            vecPythiaRatio.push_back(hSimRatio);
        }
    }

    // Get generated powheg for comparisons
    for(int radius = minradius; radius <= maxradius; radius++){
        TH1D *hSpecStat = (TH1D*)fSimPowheg->Get(Form("hStat_R0%i",radius));
        TH1D *hSpecMufMur = (TH1D*)fSimPowheg->Get(Form("hMufMur_R0%i",radius));
        TH1D *hSpecPDF = (TH1D*)fSimPowheg->Get(Form("hPDF_R0%i",radius));
        // Add bin errors for MufMur and PDF together
        TH1D *hSpecSyst = (TH1D*)hSpecMufMur->Clone(Form("hSyst_R0%i",radius));
        for(int bin = 1; bin <= hSpecSyst->GetNbinsX(); bin++){
            double error = sqrt(pow(hSpecMufMur->GetBinError(bin),2) + pow(hSpecPDF->GetBinError(bin),2));
            hSpecSyst->SetBinError(bin,error);
        }

        if(radius != 2){
            TH1D *hSpecStatRatio = (TH1D*)fSimPowheg->Get(Form("hStat_Ratio_R0%i",radius));
            TH1D *hSpecMufMurRatio = (TH1D*)fSimPowheg->Get(Form("hMufMur_Ratio_R0%i",radius));
            TH1D *hSpecPDFRatio = (TH1D*)fSimPowheg->Get(Form("hPDF_Ratio_R0%i",radius));
            TH1D *hSpecSystRatio = (TH1D*)hSpecMufMurRatio->Clone(Form("hSyst_Ratio_R0%i",radius));
            for(int bin = 1; bin <= hSpecSystRatio->GetNbinsX(); bin++){
                double error = sqrt(pow(hSpecMufMurRatio->GetBinError(bin),2) + pow(hSpecPDFRatio->GetBinError(bin),2));
                hSpecSystRatio->SetBinError(bin,error);
            }

            vecPowhegStatRatio.push_back(hSpecStatRatio);
            vecPowhegSystRatio.push_back(hSpecSystRatio);
        }

        vecPowhegStat.push_back(hSpecStat);
        vecPowhegSyst.push_back(hSpecSyst);
    }

    // Set up graphs for spectra and ratios
    for(int radius = minradius; radius <= maxradius; radius++){
        // Set systematic bin error for spectra
        for(int bin = 1; bin <= vecSpectrumSys.at(radius-minradius)->GetNbinsX(); bin++){
            double sysError          = (vecSystematics.at(radius-minradius)->GetBinContent(bin))/100;
            Double_t scaledError     = sysError*vecSpectrumSys.at(radius-minradius)->GetBinContent(bin);
            vecSpectrumSys.at(radius-minradius)->SetBinError(bin, scaledError);
        }

        // Create TGraphErrors and remove unused points for spectra
        TGraphErrors *gSpectrum     = new TGraphErrors(vecSpectrum.at(radius-minradius));
        TGraphErrors *gSpectrumSys  = new TGraphErrors(vecSpectrumSys.at(radius-minradius));

        gSpectrum = FormatSpectrum(gSpectrum,radius,false,false,false,minradius,maxradius);
        gSpectrumSys = FormatSpectrum(gSpectrumSys,radius,true,false,false,minradius,maxradius);

        // Save to vectors
        vecSpectrumGraph.push_back(gSpectrum);
        vecSpectrumSysGraph.push_back(gSpectrumSys);

        // Set systematic bin error for ratios
        if(radius != minradius){
            for(int bin = 1; bin <= vecRatioSys.at(radius-minradius-1)->GetNbinsX(); bin++){
                double sysError          = (vecSystematicsRatios.at(radius-minradius-1)->GetBinContent(bin))/100;
                Double_t scaledError     = sysError*vecRatioSys.at(radius-minradius-1)->GetBinContent(bin);
                vecRatioSys.at(radius-minradius-1)->SetBinError(bin, scaledError);
            }

            // Create TGraphErrors and remove unused points for spectra
            TGraphErrors *gRatio        = new TGraphErrors(vecRatio.at(radius-minradius-1));
            TGraphErrors *gRatioSys     = new TGraphErrors(vecRatioSys.at(radius-minradius-1));
           
            gRatio = FormatSpectrum(gRatio,radius,false,false,false,minradius,maxradius);
            gRatioSys = FormatSpectrum(gRatioSys,radius,true,false,false,minradius,maxradius);

            // Save to vectors
            vecRatioGraph.push_back(gRatio);
            vecRatioSysGraph.push_back(gRatioSys);
        }
    }

    //Set up graphs for POWHEG
    for(int radius = minradius; radius <= maxradius; radius++){
        // Create TGraphErrors and remove unused points for spectra
        TGraphErrors *gSpectrumStat = new TGraphErrors(vecPowhegStat.at(radius-minradius));
        TGraphErrors *gSpectrumSyst = new TGraphErrors(vecPowhegSyst.at(radius-minradius));

        gSpectrumStat = FormatSpectrum(gSpectrumStat,radius,false,false,true,minradius,maxradius);
        gSpectrumSyst = FormatSpectrum(gSpectrumSyst,radius,true,false,true,minradius,maxradius);

        // Save to vectors
        vecPowhegStatGraph.push_back(gSpectrumStat);
        vecPowhegSystGraph.push_back(gSpectrumSyst);

        // Create TGraphErrors and remove unused points for spectra
        if(radius != 2){
            TGraphErrors *gSpectrumStatRatio = new TGraphErrors(vecPowhegStatRatio.at(radius-minradius-1));
            TGraphErrors *gSpectrumSystRatio = new TGraphErrors(vecPowhegSystRatio.at(radius-minradius-1));

            gSpectrumStatRatio = FormatSpectrum(gSpectrumStatRatio,radius,false,false,true,minradius,maxradius);
            gSpectrumSystRatio = FormatSpectrum(gSpectrumSystRatio,radius,true,false,true,minradius,maxradius);

            // Save to vectors
            vecPowhegStatRatioGraph.push_back(gSpectrumStatRatio);
            vecPowhegSystRatioGraph.push_back(gSpectrumSystRatio);
        }
    }

    // Set up graphs for PYTHIA
    for(int radius = minradius; radius <= maxradius; radius++){
            // Create TGraphErrors and remove unused points for spectra
            TGraphErrors *gSpectrum = new TGraphErrors(vecPythia.at(radius-minradius));
            gSpectrum = FormatSpectrum(gSpectrum,radius,false,true,false,minradius,maxradius);

            // Save to vectors
            vecPythiaGraph.push_back(gSpectrum);

            // Create TGraphErrors and remove unused points for spectra
            if(radius != 2){
                TGraphErrors *gSpectrumRatio = new TGraphErrors(vecPythiaRatio.at(radius-minradius-1));
                gSpectrumRatio = FormatSpectrum(gSpectrumRatio,radius,false,true,false,minradius,maxradius);

                // Save to vectors
                vecPythiaRatioGraph.push_back(gSpectrumRatio);
            }
    }

    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("c", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();

    TCanvas *cRatio   = new TCanvas("c2", "", 800, 800);
    DrawPaperCanvasSettings(cRatio,0.14,0.025,0.025,0.08);

    gStyle->SetOptStat(0);

    TLegend *legendSpectrum =  GetAndSetLegend2(0.73,0.79-(maxradius-minradius+1)*textSize,0.93,0.79,textSize);
    TLegend *legendRatio    = GetAndSetLegend2(0.73,0.94-(maxradius-minradius)*textSize,0.93,0.94,textSize);
    TLegend *legendSim      =  GetAndSetLegend2(0.63,0.79-3*textSize,0.88,0.79,textSize);
    TLegend *legendSimNewer =  GetAndSetLegend2(0.19,0.2,0.39,0.2+2*textSize,textSize);

    TLegend *legendSimRatio = GetAndSetLegend2(0.65,0.94-3*textSize,0.9,0.94,textSize);
    TLegend *legendSimRatioCombined = GetAndSetLegend2(0.165,0.11,0.925,0.11+((9)*textSize)/3,textSize,4);

    TLegend *legendErrorKey =  GetAndSetLegend2(0.19,0.11,0.39,0.11+2*textSize,textSize);
    legendErrorKey->AddEntry(vecSpectrumSysGraph.at(0), "Systematic Uncertainty", "f");
    legendErrorKey->AddEntry(vecSpectrumGraph.at(0), "Statistical Uncertainty", "e");

    TLegend *legendErrorKeyRatio =  GetAndSetLegend2(0.6,0.11,0.8,0.11+2*textSize,textSize);
    legendErrorKeyRatio->AddEntry(vecSpectrumSysGraph.at(0), "Systematic Uncertainty", "f");
    legendErrorKeyRatio->AddEntry(vecSpectrumGraph.at(0), "Statistical Uncertainty", "e");

    TLegend *legendErrorKeyRatioCombined = GetAndSetLegend2(0.58,0.94-(2)*textSize,0.93,0.94,textSize);
    legendErrorKeyRatioCombined->AddEntry(vecSpectrumSysGraph.at(0), "Systematic Uncertainty", "f");
    legendErrorKeyRatioCombined->AddEntry(vecSpectrumGraph.at(0), "Statistical Uncertainty", "e");

    // Set up dummy histos for plotting
    //TH1D *dummySpectrum = (TH1D*)vecSpectrum.at(0)->Clone("dummySpectrum");
    TH1D *dummySpectrum = new TH1D("dummySpectrum","",300,0,300);
    dummySpectrum->GetXaxis()->SetRangeUser(0,270);
    dummySpectrum->GetYaxis()->SetRangeUser(3e-8,0.6);
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    TH1D *dummySpectrumUnscaled = new TH1D("dummySpectrumUnscaled","",300,0,300);
    dummySpectrumUnscaled->GetXaxis()->SetRangeUser(0,270);
    dummySpectrumUnscaled->GetYaxis()->SetRangeUser(3e-8,2e-2);
    SetStyleHistoTH1ForGraphs(dummySpectrumUnscaled,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    // Set up dummy histos for plotting
    TH1D *dummySpectrumLogX = new TH1D("dummySpectrumLogX","",300,0,300);
    dummySpectrumLogX->GetXaxis()->SetRangeUser(18,260);
    dummySpectrumLogX->GetYaxis()->SetRangeUser(3e-8,0.6);
    SetStyleHistoTH1ForGraphs(dummySpectrumLogX,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    TH1D *dummySpectrumLogXUnscaled = new TH1D("dummySpectrumLogXUnscaled","",300,0,300);
    dummySpectrumLogXUnscaled->GetXaxis()->SetRangeUser(18,260);
    dummySpectrumLogXUnscaled->GetYaxis()->SetRangeUser(3e-8,2e-2);
    SetStyleHistoTH1ForGraphs(dummySpectrumLogXUnscaled,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    TH1D *dummyRatio    = new TH1D("dummyRatio","",300,0,300);
    dummyRatio->GetXaxis()->SetRangeUser(0,260);
    dummyRatio->GetYaxis()->SetRangeUser(0,1.4);
    SetStyleHistoTH1ForGraphs(dummyRatio,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}^{#it{R}=0.2}}{d^{2}#it{#sigma}^{#it{R}=0.#it{X}}}",textSize,0.035,textSize,0.035,1,1.7);

    TLine * line = new TLine (0,1,260,1);
    line->SetLineColor(14);
    line->SetLineWidth(3);
    line->SetLineStyle(7);

    cSpectrum->cd();
    ////////////////////////////////
    // Plot scaled cross-sections //
    ////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *spectrumStat = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("spectrumStat_R0%i", radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("spectrumSyst_R0%i", radius));
        TGraphErrors *powhegStat   = (TGraphErrors*)vecPowhegStatGraph.at(radius-minradius)->Clone(Form("powhegStat_R0%i", radius));
        TGraphErrors *powhegSyst   = (TGraphErrors*)vecPowhegSystGraph.at(radius-minradius)->Clone(Form("powhegSyst_R0%i", radius));
        TGraphErrors *pythiaStat   = (TGraphErrors*)vecPythiaGraph.at(radius-minradius)->Clone(Form("pythiaStat_R0%i", radius));

        spectrumStat->Scale((double)scale[radius-minradius]);
        spectrumSyst->Scale((double)scale[radius-minradius]);
        powhegStat->Scale((double)scale[radius-minradius]);
        powhegSyst->Scale((double)scale[radius-minradius]);
        pythiaStat->Scale((double)scale[radius-minradius]);

        legendSpectrum->AddEntry(spectrumStat, Form("#it{R} = 0.%i x %i",radius,scale[radius-minradius]), "p");
        if(radius==minradius){
            legendSimNewer->AddEntry(pythiaStat, "PYTHIA8", "l");
            legendSimNewer->AddEntry(powhegStat, "POWHEG+PYTHIA8", "l");
        }

        if(radius==minradius) dummySpectrum->Draw("axis");
        spectrumSyst->Draw("e2,same");
        spectrumStat->Draw("p,e1,same");
        powhegSyst->Draw("l,e3,same");
        powhegStat->Draw("l,e3,same");
        pythiaStat->Draw("l,e3,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");
    legendSimNewer->Draw("same");

    drawLatexAdd("ALICE Preliminary",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.84, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/jetspectrum_scaled.%s",output.Data(),fileType.Data()));
    cSpectrum->SetLogx(1);

    /////////////////////////////////////
    // Plot scaled cross-sections logx //
    /////////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *spectrumStat = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("spectrumStat_R0%i", radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("spectrumSyst_R0%i", radius));
        TGraphErrors *powhegStat   = (TGraphErrors*)vecPowhegStatGraph.at(radius-minradius)->Clone(Form("powhegStat_R0%i", radius));
        TGraphErrors *powhegSyst   = (TGraphErrors*)vecPowhegSystGraph.at(radius-minradius)->Clone(Form("powhegSyst_R0%i", radius));
        TGraphErrors *pythiaStat   = (TGraphErrors*)vecPythiaGraph.at(radius-minradius)->Clone(Form("pythiaStat_R0%i", radius));

        spectrumStat->Scale((double)scale[radius-minradius]);
        spectrumSyst->Scale((double)scale[radius-minradius]);
        powhegStat->Scale((double)scale[radius-minradius]);
        powhegSyst->Scale((double)scale[radius-minradius]);
        pythiaStat->Scale((double)scale[radius-minradius]);

        if(radius==minradius) dummySpectrumLogX->Draw("axis");
        spectrumSyst->Draw("e2,same");
        spectrumStat->Draw("p,e1,same");
        powhegSyst->Draw("l,e3,same");
        powhegStat->Draw("l,e3,same");
        pythiaStat->Draw("l,e3,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");
    legendSimNewer->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/jetspectrum_logx_scaled.%s",output.Data(),fileType.Data()));
    cSpectrum->SetLogx(0);

    legendSpectrum->Clear();

    //////////////////////////////////
    // Plot unscaled cross-sections //
    //////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *spectrumStat = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("spectrumStat_R0%i", radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("spectrumSyst_R0%i", radius));
        TGraphErrors *powhegStat  = (TGraphErrors*)vecPowhegStatGraph.at(radius-minradius)->Clone(Form("powhegStat_R0%i", radius));
        TGraphErrors *powhegSyst  = (TGraphErrors*)vecPowhegSystGraph.at(radius-minradius)->Clone(Form("powhegSyst_R0%i", radius));
        TGraphErrors *pythiaStat = (TGraphErrors*)vecPythiaGraph.at(radius-minradius)->Clone(Form("pythiaStat_R0%i", radius));


        legendSpectrum->AddEntry(spectrumStat, Form("#it{R} = 0.%i",radius), "p");

        if(radius==minradius) dummySpectrumUnscaled->Draw("axis");
        spectrumSyst->Draw("e2,same");
        powhegSyst->Draw("l,e3,same");
        spectrumStat->Draw("p,e1,same");
        powhegStat->Draw("l,e3,same");
        pythiaStat->Draw("l,e3,same");



        fOutput->mkdir(Form("R0%i",radius));
        fOutput->cd(Form("R0%i",radius));
        spectrumSyst->Write();
        spectrumStat->Write();
        powhegSyst->Write();
        powhegStat->Write();
        pythiaStat->Write();
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");
    legendSimNewer->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/jetspectrum.%s",output.Data(),fileType.Data()));
    cSpectrum->SetLogx(1);

    ///////////////////////////////////////
    // Plot unscaled cross-sections logx //
    ///////////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *spectrumStat = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("spectrumStat_R0%i", radius));
        TGraphErrors *spectrumSyst = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("spectrumSyst_R0%i", radius));
        TGraphErrors *powhegStat  = (TGraphErrors*)vecPowhegStatGraph.at(radius-minradius)->Clone(Form("powhegStat_R0%i", radius));
        TGraphErrors *powhegSyst  = (TGraphErrors*)vecPowhegSystGraph.at(radius-minradius)->Clone(Form("powhegSyst_R0%i", radius));
        TGraphErrors *pythiaStat = (TGraphErrors*)vecPythiaGraph.at(radius-minradius)->Clone(Form("pythiaStat_R0%i", radius));

        if(radius==minradius) dummySpectrumLogXUnscaled->Draw("axis");
        spectrumSyst->Draw("e2,same");
        spectrumStat->Draw("p,e1,same");
        powhegSyst->Draw("l,e3,same");
        powhegStat->Draw("l,e3,same");
        pythiaStat->Draw("l,e3,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");
    legendSimNewer->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/jetspectrum_logx.%s",output.Data(),fileType.Data()));
    cSpectrum->SetLogx(0);

    legendSpectrum->Clear();

    /////////////////////////////////////////////////////////////
    // Plot combined ratio simulation comparisons (HP request) //
    /////////////////////////////////////////////////////////////
    cRatio->cd();

    dummyRatio->Draw("axis");
    for(int radius = minradius+1; radius <= maxradius; radius++){
        TGraphErrors *ratioStat = (TGraphErrors*)vecRatioGraph.at(radius-minradius-1)->Clone(Form("ratioStat_R0%i", radius));
        TGraphErrors *ratioSyst = (TGraphErrors*)vecRatioSysGraph.at(radius-minradius-1)->Clone(Form("ratioSyst_R0%i", radius));

        TGraphErrors *ratioPythia = (TGraphErrors*)vecPythiaRatioGraph.at(radius-minradius-1)->Clone(Form("ratioPythiaStat_%i",radius));
        TGraphErrors *ratioPowhegStat = (TGraphErrors*)vecPowhegStatRatioGraph.at(radius-minradius-1)->Clone(Form("ratioPowhegStat_%i",radius));
        TGraphErrors *ratioPowhegSyst = (TGraphErrors*)vecPowhegSystRatioGraph.at(radius-minradius-1)->Clone(Form("ratioPowhegSyst_%i",radius));

        legendSimRatioCombined->AddEntry(ratioStat, Form("R = 0.2/0.%i |", radius), "");
        legendSimRatioCombined->AddEntry(ratioStat, "          |   ", "p");
        legendSimRatioCombined->AddEntry(ratioPythia, "        |    ", "l");
        legendSimRatioCombined->AddEntry(ratioPowhegStat, "    ", "l");

        ratioSyst->Draw("e2,same");
        ratioStat->Draw("p,e1,same");
        ratioPythia->Draw("l,e3,same");
        ratioPowhegStat->Draw("l,e3,same");
        ratioPowhegSyst->Draw("l,e3,same");

        fOutput->cd(Form("R0%i",radius));
        ratioStat->Write();
        ratioSyst->Write();
        ratioPythia->Write();
        ratioPowhegStat->Write();
        ratioPowhegSyst->Write();
    }

    line->Draw("same");
    legendSimRatioCombined->Draw("same");
    legendErrorKeyRatioCombined->Draw("same");

    drawLatexAdd("ALICE Data       | PYTHIA8 Monash | POWHEG+PYTHIA8",0.4,0.21, textSize/1.3,kFALSE, kFALSE, false);

    //drawLatexAdd("ALICE Preliminary",0.17,0.92, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.17,0.88, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.17,0.84, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.17,0.8, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.17,0.76, textSize,kFALSE, kFALSE, false);

    cRatio->SaveAs(Form("%s/FinalResults/xsectionratio.%s",output.Data(),fileType.Data()));
    legendRatio->Clear();

    /////////////////////////////////
    // Plot simulation comparisons //
    /////////////////////////////////
    Double_t minPt               = 0.;
    Double_t maxPt               = 260.;
    Double_t textsizeLabelsWidth = 0;
    Double_t textsizeFacWidth    = 0;
    Double_t textsizeLabelsComp  = 0;
    Double_t textsizeFacComp     = 0;
    Double_t textSizeLabelsPixel = 30;

    Double_t arrayBoundariesX[2];
    Double_t arrayBoundariesY[4];
    Double_t relativeMarginsX[3];
    Double_t relativeMarginsY[4];

    ReturnCorrectValuesForCanvasScaling(800,1000, 1, 3,0.15, 0.025, 0.005,0.07,arrayBoundariesX,arrayBoundariesY,relativeMarginsX,relativeMarginsY);
    Double_t margin = relativeMarginsX[0]*2.7*800;

    // Declare root objects
    TCanvas *canvas;
    TCanvas *ratioCanvas;
    TPad    *upperPad;
    TPad    *lowerPad;
    TPad    *topPad;
    TH2F    *dummyHistUpper;
    TH2F    *dummyHistLower;

    gStyle->SetOptStat(0);

    TLegend *legendSimNew;
    TLegend *legendErrorKeyNew;


    //canvas->cd();
    for(int radius = minradius; radius <= maxradius; radius++){
            // Define root objects
            canvas         = new TCanvas(Form("canvasSimR0%i", radius),"",800,1000);
            upperPad       = new TPad(Form("upperPadR0%i", radius), "", arrayBoundariesX[0], arrayBoundariesY[2], arrayBoundariesX[1], arrayBoundariesY[0],-1, -1, -2);
            lowerPad       = new TPad(Form("lowerPadR0%i",  radius), "", arrayBoundariesX[0], arrayBoundariesY[3], arrayBoundariesX[1], arrayBoundariesY[2],-1, -1, -2);
            topPad         = new TPad(Form("topPadR0%i", radius), "", 0.13, 0.32, 0.52, 0.52,-1, -1, -2);
            dummyHistUpper = new TH2F(Form("dummyHistUpper_R0%i", radius),Form("dummyHistUpper_R0%i", radius), 1000, minPt,maxPt ,1000., 3e-8,2e-2);
            if(radius == 2 || radius == 3) dummyHistLower = new TH2F(Form("dummyHistLower_R0%i", radius),Form("dummyHistLower_R0%i", radius), 1000, minPt,maxPt, 1000., 0.9,1.9);
            else if(radius == 4) dummyHistLower = new TH2F(Form("dummyHistLower_R0%i", radius),Form("dummyHistLower_R0%i", radius), 1000, minPt,maxPt, 1000., 0.9,2.1);
            else dummyHistLower = new TH2F(Form("dummyHistLower_R0%i", radius),Form("dummyHistLower_R0%i", radius), 1000, minPt,maxPt, 1000., 0.9,2.7);

            DrawGammaCanvasSettings( canvas,  0.15, 0.025, 0.025, 0.08);
            DrawGammaPadSettings( upperPad, relativeMarginsX[0], relativeMarginsX[2], relativeMarginsY[0], relativeMarginsY[1]);
            DrawGammaPadSettings( lowerPad, relativeMarginsX[0], relativeMarginsX[2], relativeMarginsY[1], relativeMarginsY[2]);
            DrawGammaPadSettings( topPad, 0., 0., 0., 0.);
            topPad->SetFillStyle(0);

            canvas->cd();
            upperPad->Draw();
            lowerPad->Draw();
            topPad->Draw();

            upperPad->cd();
            upperPad->SetLogy();

            if (upperPad->XtoPixel(upperPad->GetX2()) < upperPad->YtoPixel(upperPad->GetY1())){
                textsizeLabelsWidth         = (Double_t)textSizeLabelsPixel/upperPad->XtoPixel(upperPad->GetX2()) ;
                textsizeFacWidth            = (Double_t)1./upperPad->XtoPixel(upperPad->GetX2()) ;
            } else {
                textsizeLabelsWidth         = (Double_t)textSizeLabelsPixel/upperPad->YtoPixel(upperPad->GetY1());
                textsizeFacWidth            = (Double_t)1./upperPad->YtoPixel(upperPad->GetY1());
            }

            SetStyleHistoTH2ForGraphs(dummyHistUpper, "#it{p}_{T} (GeV/#it{c})", "#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})", 0.85*textsizeLabelsWidth, textsizeLabelsWidth,
                                      0.85*textsizeLabelsWidth, textsizeLabelsWidth, 0.8,0.76/(textsizeFacWidth*margin), 512, 505,42,42);
            dummyHistUpper->DrawCopy();

            TGraphErrors *finalSpectrumStat = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("finalSpectrumStat_R0%i", radius));
            TGraphErrors *finalSpectrumSyst = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("finalSpectrumSyst_R0%i", radius));
            TGraphErrors *pythiaStat  = (TGraphErrors*)vecPythiaGraph.at(radius-minradius)->Clone(Form("pythiaStat_R0%i", radius));
            TGraphErrors *powhegStat  = (TGraphErrors*)vecPowhegStatGraph.at(radius-minradius)->Clone(Form("powhegStat_R0%i", radius));
            TGraphErrors *powhegSyst  = (TGraphErrors*)vecPowhegSystGraph.at(radius-minradius)->Clone(Form("powhegSyst_R0%i", radius));
            TH1D *dataRatioStat = (TH1D*)vecSpectrum.at(radius-minradius)->Clone(Form("dataRatioStat_R0%i", radius));
            TH1D *zeroErrorStat = (TH1D*)vecSpectrum.at(radius-minradius)->Clone(Form("zeroErrorStat_R0%i", radius));
            TH1D *dataRatioSyst = (TH1D*)vecSpectrumSys.at(radius-minradius)->Clone(Form("dataRatioSyst_R0%i", radius));
            TH1D *zeroErrorSyst = (TH1D*)vecSpectrumSys.at(radius-minradius)->Clone(Form("zeroErrorSyst_R0%i", radius));

            for(int i = 1; i < zeroErrorStat->GetNbinsX()+1; i++){
                zeroErrorStat->SetBinError(i,0);
                zeroErrorSyst->SetBinError(i,0);
            }

            dataRatioStat->Divide(zeroErrorStat);
            dataRatioSyst->Divide(zeroErrorSyst);

            TGraphErrors *dataRatioStatGraph = new TGraphErrors(dataRatioStat);
            TGraphErrors *dataRatioSystGraph = new TGraphErrors(dataRatioSyst);
            dataRatioStatGraph = FormatSpectrum(dataRatioStatGraph,radius,false,false,false,minradius,maxradius);
            dataRatioSystGraph = FormatSpectrum(dataRatioSystGraph,radius,true,false,false,minradius,maxradius);

            TH1D *ratiosimdataPythiaStat = (TH1D*)vecPythia.at(radius-minradius)->Clone(Form("ratiosimdatalowerPythiaStat_%i",radius));
            
            ratiosimdataPythiaStat->Divide(zeroErrorStat);
            ratiosimdataPythiaStat->GetYaxis()->SetRangeUser(0.96,2.4);

            TGraphErrors *ratiosimdataPythiaStat_graph = new TGraphErrors(ratiosimdataPythiaStat);

            TH1D *ratiosimdataPowhegStat = (TH1D*)vecPowhegStat.at(radius-minradius)->Clone(Form("ratiosimdatalowerPowhegStat_%i",radius));
            TH1D *ratiosimdataPowhegSyst = (TH1D*)vecPowhegSyst.at(radius-minradius)->Clone(Form("ratiosimdatalowerPowhegSyst_%i",radius));
            ratiosimdataPowhegStat->Divide(zeroErrorStat);
            ratiosimdataPowhegSyst->Divide(zeroErrorSyst);
            ratiosimdataPowhegStat->GetYaxis()->SetRangeUser(0.96,2.4);
            ratiosimdataPowhegSyst->GetYaxis()->SetRangeUser(0.96,2.4);
            TGraphErrors *ratiosimdataPowhegStat_graph = new TGraphErrors(ratiosimdataPowhegStat);
            TGraphErrors *ratiosimdataPowhegSyst_graph = new TGraphErrors(ratiosimdataPowhegSyst);

            ratiosimdataPowhegStat_graph = FormatSpectrum(ratiosimdataPowhegStat_graph,radius,false,false,true,minradius,maxradius);
            ratiosimdataPowhegSyst_graph = FormatSpectrum(ratiosimdataPowhegSyst_graph,radius,true,false,true,minradius,maxradius);
            ratiosimdataPythiaStat_graph = FormatSpectrum(ratiosimdataPythiaStat_graph,radius,false,true,false,minradius,maxradius);

            legendSimNew = GetAndSetLegend2(0.61,0.68-3*textsizeLabelsWidth*0.8,0.86,0.68,textsizeLabelsWidth*0.8);
            legendErrorKeyNew =  GetAndSetLegend2(0.19,0.04,0.39,0.04+2*textsizeLabelsWidth*0.8,textsizeLabelsWidth*0.8);

            legendErrorKeyNew->AddEntry(vecSpectrumSysGraph.at(0), "Systematic Uncertainty", "f");
            legendErrorKeyNew->AddEntry(vecSpectrumGraph.at(0), "Statistical Uncertainty", "e");

            legendSimNew->AddEntry(finalSpectrumStat, "ALICE Data", "p");
            legendSimNew->AddEntry(pythiaStat, "PYTHIA8 Monash", "p");
            legendSimNew->AddEntry(powhegStat, "POWHEG+PYTHIA8", "p");

            finalSpectrumSyst->Draw("e2,same");
            powhegSyst->Draw("e2,same");
            finalSpectrumStat->Draw("p,e1,same");
            pythiaStat->Draw("p,e1,same");
            powhegStat->Draw("p,e1,same");
            legendSimNew->Draw();
            legendErrorKeyNew->Draw();

            //drawLatexAdd("ALICE Preliminary",0.93,0.92, 0.85*textsizeLabelsWidth,kFALSE, kFALSE, true);
            drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.87, 0.85*textsizeLabelsWidth,kFALSE, kFALSE, true);
            drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.%i",radius),0.93,0.82, 0.85*textsizeLabelsWidth,kFALSE, kFALSE, true);
            drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.77, 0.85*textsizeLabelsWidth,kFALSE, kFALSE, true);
            drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.72, 0.85*textsizeLabelsWidth,kFALSE, kFALSE, true);

            lowerPad->cd();

          	if (lowerPad->XtoPixel(lowerPad->GetX2()) <lowerPad->YtoPixel(lowerPad->GetY1()) ){
          	    textsizeLabelsComp              = (Double_t)textSizeLabelsPixel/lowerPad->XtoPixel(lowerPad->GetX2()) ;
          	    textsizeFacComp                 = (Double_t)1./lowerPad->XtoPixel(lowerPad->GetX2()) ;
          	} else {
          	    textsizeLabelsComp              = (Double_t)textSizeLabelsPixel/lowerPad->YtoPixel(lowerPad->GetY1());
          	    textsizeFacComp                 = (Double_t)1./lowerPad->YtoPixel(lowerPad->GetY1());
          	}

            SetStyleHistoTH2ForGraphs(dummyHistLower, "#it{p}_{T} (GeV/#it{c})", "MC/Data     ", 0.85*textsizeLabelsComp, textsizeLabelsComp, 0.85*textsizeLabelsComp,
                                      textsizeLabelsComp, 1, 0.65/(textsizeFacComp*margin),512,505,42,42);
            dummyHistLower->DrawCopy();

            ratiosimdataPythiaStat_graph->Draw("p,e1,same");
            ratiosimdataPowhegSyst_graph->Draw("e2,same");
            ratiosimdataPowhegStat_graph->Draw("p,e1,same");

            dataRatioSystGraph->Draw("e2,same");
            dataRatioStatGraph->Draw("e1,same");
            DrawGammaLines(0.,260.,1.,1.,8.,16,9);

            canvas->Update();

            canvas->SaveAs(Form("%s/FinalResults/SingleSpectra/jetspectrum_R0%i.%s",output.Data(),radius,fileType.Data()));
            legendSimNew->Clear();
            legendErrorKeyNew->Clear();

            dummyHistUpper = NULL;
            delete dummyHistUpper;
            upperPad->Clear();
            lowerPad->Clear();
            topPad->Clear();
            canvas->Clear();
    }

    ///////////////////////////////////////
    // Plot ratio simulation comparisons //
    ///////////////////////////////////////
    cRatio->cd();
    for(int radius = minradius+1; radius <= maxradius; radius++){
            TGraphErrors *finalRatio = (TGraphErrors*)vecRatioGraph.at(radius-minradius-1)->Clone(Form("finalSpectrum_R0%i", radius));
            TGraphErrors *finalRatioSys = (TGraphErrors*)vecRatioSysGraph.at(radius-minradius-1)->Clone(Form("finalSpectrumSys_R0%i", radius));
            
            TGraphErrors *ratioPythia = (TGraphErrors*)vecPythiaRatioGraph.at(radius-minradius-1)->Clone(Form("dummyratiosimPythia_%i",radius));

            TGraphErrors *ratioPowhegStat = (TGraphErrors*)vecPowhegStatRatioGraph.at(radius-minradius-1)->Clone(Form("dummyratiosimPowhegStat_%i",radius));
            TGraphErrors *ratioPowhegSyst = (TGraphErrors*)vecPowhegSystRatioGraph.at(radius-minradius-1)->Clone(Form("dummyratiosimPowhegSyst_%i",radius));
            
            finalRatioSys->SetFillStyle(0);

            dummyRatio->Draw("axis");
            finalRatioSys->Draw("e2,same");
            finalRatio->Draw("p,e1,same");

            legendSimRatio->AddEntry(finalRatio, "ALICE Data", "p");
            legendSimRatio->AddEntry(ratioPythia, "PYTHIA8 Monash", "p");
            legendSimRatio->AddEntry(ratioPowhegStat, "POWHEG+PYTHIA8", "p");
            ratioPythia->Draw("l,e3,same");
            ratioPowhegSyst->Draw("l,e3,same");
            ratioPowhegStat->Draw("l,e3,same");
            line->Draw("same");
            legendSimRatio->Draw();
            legendErrorKeyRatio->Draw();

            //drawLatexAdd("ALICE Preliminary",0.17,0.92, textSize,kFALSE, kFALSE, false);
            drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.17,0.88, textSize,kFALSE, kFALSE, false);
            drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.2/#it{R}=0.%i",radius),0.17,0.84, textSize,kFALSE, kFALSE, false);
            drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.17,0.8, textSize,kFALSE, kFALSE, false);
            drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.17,0.76, textSize,kFALSE, kFALSE, false);

            cRatio->SaveAs(Form("%s/FinalResults/SingleRatios/xsectionratio_R020%i.%s",output.Data(),radius,fileType.Data()));
            legendSimRatio->Clear();
    }

    fOutput->Close();
    delete fOutput;
}
