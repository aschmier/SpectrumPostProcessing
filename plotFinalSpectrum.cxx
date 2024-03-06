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

void plotFinalSpectrum(TString spectrumFile, TString fPYTHIA, TString fPOWHEG, TString systematicsDir, TString type, TString output, TString fileType)
{
    Double_t textSize     = 0.03;
    Int_t regnum          = 6;
    int minradius = 2;
    int maxradius = 5;
    double alphasval = 0.25;


    //TH1D *rffit;
    //TH1D *rffitRatio;

    int scale[5] = {1,3,10,30,100};
    //const int nOutlier = 4;
    const int nOutlier = 1;

    int stylesempty[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    //TString outliers[nOutlier] = {"nooutlier","outlier30","outlier50","outlier70"};
    TString outliers[nOutlier] = {"nooutlier"};

    vector<double> trueRebin = getJetPtBinningNonLinTrue8TeV();

    vector<TH1D*> vecSpectrum;
    vector<TH1D*> vecSpectrumSys;
    vector<TH1D*> vecSystematics;
    vector<TH1D*> vecSystematicsRatios;
    vector<TH1D*> vecRatio;
    vector<TH1D*> vecRatioSys;
    vector<TH1D*> vecSpectrumSimPythia[nOutlier];
    vector<TH1D*> vecSpectrumSimPowheg;
    vector<TH1D*> vecAllUnfolding;
    vector<TH1D*> vecTriggerSwap;
    vector<TGraphErrors*> vecSpectrumGraph;
    vector<TGraphErrors*> vecSpectrumSysGraph;
    vector<TGraphErrors*> vecRatioGraph;
    vector<TGraphErrors*> vecRatioSysGraph;

    TString outputDirRootFile = "/home/austin/alice/FinalResultsRootFiles";
    gSystem->Exec("mkdir -p "+outputDirRootFile);

    gSystem->Exec("mkdir -p "+output+"/FinalResults");
    gSystem->Exec("mkdir -p "+output+"/MCGen");
    gSystem->Exec("mkdir -p "+output+"/MCGen/ratioDataMC");

    const char* nameOutput = Form("%s/FinalResults_pp8TeV.root",outputDirRootFile.Data());
    TFile* fOutput = new TFile(nameOutput,"RECREATE");

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
        for(int outlier = 0; outlier < nOutlier; outlier++){
          TList *simBase = (TList*)fSimPythia->Get(Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers[outlier].Data()));
          if(!simBase){
            cout << Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers[outlier].Data()) << endl;
            cout << "No sim base found!" << endl;
            return;
          }
          TH1D *hSpec = (TH1D*)simBase->FindObject("hJetPt");
          TH1D *hSim = (TH1D*)ProcessSimHisto( hSpec, trueRebin, radius, outliers[outlier], stylesempty[radius-minradius+1], colors[radius-minradius+1], sizesempty[radius-minradius+1] );
          vecSpectrumSimPythia[outlier].push_back(hSim);
        }
    }

    // Get generated powheg for comparisons
    for(int radius = minradius; radius <= maxradius; radius++){
        TH1D *hSpec = (TH1D*)fSimPowheg->Get(Form("InclusiveJetXSection_R%i",radius));
        TH1D *hEvent = (TH1D*)fSimPowheg->Get("hNEvent");
        hSpec->Scale(1./hEvent->GetBinContent(1));
        TH1D *hSim = (TH1D*)ProcessSimHisto( hSpec, trueRebin, radius, "na", stylesempty[radius-minradius+2], colors[radius-minradius+2], sizesempty[radius-minradius+2] );
        vecSpectrumSimPowheg.push_back(hSim);
    }

    // Set up graphs for spectra and ratios
    for(int radius = minradius; radius <= maxradius; radius++){
        // Set systematic bin error for spectra
        for(int bin = 1; bin <= vecSpectrumSys.at(radius-minradius)->GetNbinsX(); bin++){
            double sysError          = (vecSystematics.at(radius-minradius)->GetBinContent(bin))/100;
            //if(radius != 2) sysError = sqrt(pow(sysError,2) + pow((rffit->GetBinContent(bin))/100,2));
            Double_t scaledError     = sysError*vecSpectrumSys.at(radius-minradius)->GetBinContent(bin);
            vecSpectrumSys.at(radius-minradius)->SetBinError(bin, scaledError);
        }

        // Create TGraphErrors and remove unused points for spectra
        TGraphErrors *gSpectrum     = new TGraphErrors(vecSpectrum.at(radius-minradius));
        TGraphErrors *gSpectrumSys  = new TGraphErrors(vecSpectrumSys.at(radius-minradius));

        for(int point = 0; point < gSpectrum->GetN(); point++){
            double staterror = gSpectrum->GetErrorY(point);
            gSpectrum->SetPointError(point,0,staterror);
            double syserror = gSpectrumSys->GetErrorY(point);
            gSpectrumSys->SetPointError(point,gSpectrumSys->GetErrorX(point),syserror);
        }

        gSpectrum->SetMarkerStyle(stylesfilled[radius-minradius]);
        gSpectrum->SetMarkerSize(sizesfilled[radius-minradius]);
        gSpectrum->SetMarkerColor(colors[radius-minradius]);
        gSpectrum->SetLineColor(colors[radius-minradius]);
        gSpectrum->SetLineWidth(3);
        gSpectrum->SetLineStyle(1);

        gSpectrumSys->SetFillColorAlpha(colors[radius-minradius],alphasval);
        gSpectrumSys->SetLineColor(colors[radius-minradius]);
        gSpectrumSys->SetFillStyle(1);

        gSpectrum->RemovePoint((gSpectrum->GetN()-1));
        gSpectrum->RemovePoint(18);
        gSpectrum->RemovePoint(17);
        gSpectrumSys->RemovePoint((gSpectrumSys->GetN()-1));
        gSpectrumSys->RemovePoint(18);
        gSpectrumSys->RemovePoint(17);
        if(radius == 5 || radius == 6){
          gSpectrum->RemovePoint(16);
          gSpectrum->RemovePoint(15);
          gSpectrumSys->RemovePoint(16);
          gSpectrumSys->RemovePoint(15);
        }
        if(radius == 6){
          gSpectrum->RemovePoint(14);
          gSpectrumSys->RemovePoint(14);
        }

        gSpectrum->RemovePoint(3);
        gSpectrum->RemovePoint(2);
        gSpectrum->RemovePoint(1);
        gSpectrum->RemovePoint(0);
        gSpectrumSys->RemovePoint(3);
        gSpectrumSys->RemovePoint(2);
        gSpectrumSys->RemovePoint(1);
        gSpectrumSys->RemovePoint(0);

        // Save to vectors
        vecSpectrumGraph.push_back(gSpectrum);
        vecSpectrumSysGraph.push_back(gSpectrumSys);

        // Set systematic bin error for ratios
        if(radius != minradius){
            for(int bin = 1; bin <= vecRatioSys.at(radius-minradius-1)->GetNbinsX(); bin++){
                double sysError          = (vecSystematicsRatios.at(radius-minradius-1)->GetBinContent(bin))/100;
                //sysError = sqrt(pow(sysError,2) + pow((vecAllUnfolding.at(radius-minradius)->GetBinContent(bin))/100,2) + pow((vecAllUnfolding.at(0)->GetBinContent(bin))/100,2) + pow((vecTriggerSwap.at(radius-minradius)->GetBinContent(bin))/100,2) + pow((vecTriggerSwap.at(0)->GetBinContent(bin))/100,2));
                Double_t scaledError     = sysError*vecRatioSys.at(radius-minradius-1)->GetBinContent(bin);
                vecRatioSys.at(radius-minradius-1)->SetBinError(bin, scaledError);
            }

            if(radius != minradius){
                // Create TGraphErrors and remove unused points for spectra
                TGraphErrors *gRatio        = new TGraphErrors(vecRatio.at(radius-minradius-1));
                TGraphErrors *gRatioSys     = new TGraphErrors(vecRatioSys.at(radius-minradius-1));

                for(int point = 0; point < gRatio->GetN(); point++){
                    double staterror = gRatio->GetErrorY(point);
                    gRatio->SetPointError(point,0,staterror);
                    double syserror = gRatioSys->GetErrorY(point);
                    gRatioSys->SetPointError(point,gRatioSys->GetErrorX(point),syserror);
                }

                gRatio->SetMarkerStyle(stylesfilled[radius-(minradius+1)+1]);
                gRatio->SetMarkerSize(2);
                gRatio->SetMarkerColor(colors[radius-(minradius+1)+1]);
                gRatio->SetLineColor(colors[radius-(minradius+1)+1]);
                gRatio->SetLineWidth(3);
                gRatio->SetLineStyle(1);

                gRatioSys->SetFillColorAlpha(colors[radius-(minradius+1)+1],alphasval);
                gRatioSys->SetLineColor(colors[radius-(minradius+1)+1]);
                gRatioSys->SetFillStyle(1);

                gRatio->RemovePoint((gRatio->GetN()-1));
                gRatio->RemovePoint(18);
                gRatio->RemovePoint(17);
                gRatioSys->RemovePoint((gRatioSys->GetN()-1));
                gRatioSys->RemovePoint(18);
                gRatioSys->RemovePoint(17);
                if(radius == 5 || radius == 6){
                    gRatio->RemovePoint(16);
                    gRatio->RemovePoint(15);
                    gRatioSys->RemovePoint(16);
                    gRatioSys->RemovePoint(15);
                }
                if(radius == 6){
                    gRatio->RemovePoint(14);
                    gRatioSys->RemovePoint(14);
                }

                gRatio->RemovePoint(3);
                gRatio->RemovePoint(2);
                gRatio->RemovePoint(1);
                gRatio->RemovePoint(0);
                gRatioSys->RemovePoint(3);
                gRatioSys->RemovePoint(2);
                gRatioSys->RemovePoint(1);
                gRatioSys->RemovePoint(0);
                // Save to vectors
                vecRatioGraph.push_back(gRatio);
                vecRatioSysGraph.push_back(gRatioSys);
            }

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
    SetStyleHistoTH1ForGraphs(dummyRatio,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}^{#it{R}=0.2}}{d#it{p}_{T}d#it{#eta}}/#frac{d^{2}#it{#sigma}^{#it{R}=0.#it{X}}}{d#it{p}_{T}d#it{#eta}}",textSize,0.035,textSize,0.035,1,1.7);

    TH1D *dummyRatioSimData    = new TH1D("dummyRatioSimData","",300,0,300);
    dummyRatioSimData->GetXaxis()->SetRangeUser(0,260);
    dummyRatioSimData->GetYaxis()->SetRangeUser(0.96,2.4);
    SetStyleHistoTH1ForGraphs(dummyRatioSimData,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}^{Sim}}{d#it{p}_{T}d#it{#eta}}/#frac{d^{2}#it{#sigma}^{Data}}{d#it{p}_{T}d#it{#eta}}",textSize,0.035,textSize,0.035,1,1.7);

    TLine * line = new TLine (0,1,260,1);
    line->SetLineColor(14);
    line->SetLineWidth(3);
    line->SetLineStyle(7);

    cSpectrum->cd();
    ////////////////////////////////
    // Plot scaled cross-sections //
    ////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *finalSpectrum = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("finalSpectrum_R0%i", radius));
        finalSpectrum->Scale((double)scale[radius-minradius]);

        TGraphErrors *finalSpectrumSys = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("finalSpectrumSys_R0%i", radius));
        finalSpectrumSys->Scale((double)scale[radius-minradius]);

        legendSpectrum->AddEntry(finalSpectrum, Form("#it{R} = 0.%i x %i",radius,scale[radius-minradius]), "p");

        if(radius==minradius) dummySpectrum->Draw("axis");
        finalSpectrumSys->Draw("e2,same");
        finalSpectrum->Draw("p,e1,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("ALICE Preliminary",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.84, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/%s_reg%i.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    cSpectrum->SetLogx(1);

    /////////////////////////////////////
    // Plot scaled cross-sections logx //
    /////////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *finalSpectrum = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("finalSpectrum_R0%i", radius));
        finalSpectrum->Scale((double)scale[radius-minradius]);

        TGraphErrors *finalSpectrumSys = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("finalSpectrumSys_R0%i", radius));
        finalSpectrumSys->Scale((double)scale[radius-minradius]);

        if(radius==minradius) dummySpectrumLogX->Draw("axis");
        finalSpectrumSys->Draw("e2,same");
        finalSpectrum->Draw("p,e1,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/%s_reg%i_logx.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    cSpectrum->SetLogx(0);

    legendSpectrum->Clear();

    //////////////////////////////////
    // Plot unscaled cross-sections //
    //////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *finalSpectrum = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("finalSpectrum_R0%i", radius));

        TGraphErrors *finalSpectrumSys = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("finalSpectrumSys_R0%i", radius));

        legendSpectrum->AddEntry(finalSpectrum, Form("#it{R} = 0.%i",radius), "p");

        if(radius==minradius) dummySpectrumUnscaled->Draw("axis");
        finalSpectrum->Draw("p,e1,same");
        finalSpectrumSys->Draw("e2,same");

        fOutput->mkdir(Form("R0%i",radius));
        fOutput->cd(Form("R0%i",radius));
        finalSpectrumSys->Write();
        finalSpectrum->Write();
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/%s_reg%i_unscaled.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    cSpectrum->SetLogx(1);

    ///////////////////////////////////////
    // Plot unscaled cross-sections logx //
    ///////////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *finalSpectrum = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("finalSpectrum_R0%i", radius));

        TGraphErrors *finalSpectrumSys = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("finalSpectrumSys_R0%i", radius));

        if(radius==minradius) dummySpectrumLogXUnscaled->Draw("axis");
        finalSpectrumSys->Draw("e2,same");
        finalSpectrum->Draw("p,e1,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/%s_reg%i_logx_unscaled.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    cSpectrum->SetLogx(0);

    legendSpectrum->Clear();

    ///////////////////////////////
    // Plot cross-section ratios //
    ///////////////////////////////
    cRatio->cd();
    for(int radius = minradius+1; radius <= maxradius; radius++){
        TGraphErrors *finalRatio = (TGraphErrors*)vecRatioGraph.at(radius-minradius-1)->Clone(Form("finalRatio_R0%i", radius));

        TGraphErrors *finalRatioSys = (TGraphErrors*)vecRatioSysGraph.at(radius-minradius-1)->Clone(Form("finalRatioSys_R0%i", radius));

        legendRatio->AddEntry(finalRatio, Form("#it{R}=0.2/#it{R}=0.%i",radius), "p");

        if(radius==(minradius+1)) dummyRatio->Draw("axis");
        finalRatioSys->Draw("e2,same");
        finalRatio->Draw("p,e1,same");

        fOutput->cd(Form("R0%i",radius));
        finalRatio->Write();
        finalRatioSys->Write();
    }

    line->Draw("same");
    legendRatio->Draw("same");
    legendErrorKeyRatio->Draw("same");

    drawLatexAdd("ALICE Preliminary",0.17,0.92, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.17,0.88, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.17,0.84, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.17,0.8, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.17,0.76, textSize,kFALSE, kFALSE, false);

    cRatio->SaveAs(Form("%s/FinalResults/%s_reg%i_Ratio.%s",output.Data(),type.Data(),regnum,fileType.Data()));

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
        for(int outlier = 0; outlier < nOutlier; outlier++){
            // Define root objects
            canvas         = new TCanvas(Form("canvasSimR0%i_%s", radius, outliers[outlier].Data()),"",800,1000);
            upperPad       = new TPad(Form("upperPadR0%i_%s", radius, outliers[outlier].Data()), "", arrayBoundariesX[0], arrayBoundariesY[2], arrayBoundariesX[1], arrayBoundariesY[0],-1, -1, -2);
            lowerPad       = new TPad(Form("lowerPadR0%i_%s",  radius, outliers[outlier].Data()), "", arrayBoundariesX[0], arrayBoundariesY[3], arrayBoundariesX[1], arrayBoundariesY[2],-1, -1, -2);
            topPad         = new TPad(Form("topPadR0%i_%s", radius, outliers[outlier].Data()), "", 0.13, 0.32, 0.52, 0.52,-1, -1, -2);
            dummyHistUpper = new TH2F(Form("dummyHistUpper_R0%i_%s", radius, outliers[outlier].Data()),Form("dummyHistUpper_R0%i_%s", radius, outliers[outlier].Data()), 1000, minPt,maxPt ,1000., 3e-8,2e-2);
            if(radius == 2 || radius == 3) dummyHistLower = new TH2F(Form("dummyHistLower_R0%i_%s", radius, outliers[outlier].Data()),Form("dummyHistLower_R0%i_%s", radius, outliers[outlier].Data()), 1000, minPt,maxPt, 1000., 0.9,1.9);
            else if(radius == 4) dummyHistLower = new TH2F(Form("dummyHistLower_R0%i_%s", radius, outliers[outlier].Data()),Form("dummyHistLower_R0%i_%s", radius, outliers[outlier].Data()), 1000, minPt,maxPt, 1000., 0.9,2.1);
            else dummyHistLower = new TH2F(Form("dummyHistLower_R0%i_%s", radius, outliers[outlier].Data()),Form("dummyHistLower_R0%i_%s", radius, outliers[outlier].Data()), 1000, minPt,maxPt, 1000., 0.9,2.7);

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

            TGraphErrors *finalSpectrum = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("finalSpectrum_R0%i_%s", radius, outliers[outlier].Data()));
            TGraphErrors *finalSpectrumSys = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("finalSpectrumSys_R0%i_%s", radius, outliers[outlier].Data()));
            TGraphErrors *graphSimPythia  = new TGraphErrors(vecSpectrumSimPythia[outlier].at(radius-minradius));
            TGraphErrors *graphSimPowheg  = new TGraphErrors(vecSpectrumSimPowheg.at(radius-minradius));


            graphSimPythia->SetMarkerColor(colors[11]);
            graphSimPythia->SetLineColor(colors[11]);

            graphSimPowheg->SetMarkerColor(colors[12]);
            graphSimPowheg->SetLineColor(colors[12]);

            TH1D *ratiosimdataPythia = (TH1D*)vecSpectrum.at(radius-minradius)->Clone(Form("ratiosimdatalowerPythia_%i_%i",radius,outlier));
            ratiosimdataPythia->Divide(vecSpectrumSimPythia[outlier].at(radius-minradius), vecSpectrum.at(radius-minradius),1,1);
            SetStyleHistoTH1ForGraphs(ratiosimdataPythia,"","#it{p}_{T} (GeV/#it{c})","Ratio",textSize,0.04,textSize,0.04,1,1.5);
            ratiosimdataPythia->GetYaxis()->SetRangeUser(0.96,2.4);
            TGraphErrors *ratiosimdataPythia_graph = new TGraphErrors(ratiosimdataPythia);

            TH1D *ratiosimdataPowheg = (TH1D*)vecSpectrum.at(radius-minradius)->Clone(Form("ratiosimdatalowerPowheg_%i_%i",radius,outlier));
            ratiosimdataPowheg->Divide(vecSpectrumSimPowheg.at(radius-minradius), vecSpectrum.at(radius-minradius),1,1);
            SetStyleHistoTH1ForGraphs(ratiosimdataPowheg,"","#it{p}_{T} (GeV/#it{c})","Ratio",textSize,0.04,textSize,0.04,1,1.5);
            ratiosimdataPowheg->GetYaxis()->SetRangeUser(0.96,2.4);
            TGraphErrors *ratiosimdataPowheg_graph = new TGraphErrors(ratiosimdataPowheg);

            ratiosimdataPythia_graph->SetFillColor(colors[11]);
            ratiosimdataPythia_graph->SetLineColor(colors[11]);
            ratiosimdataPythia_graph->SetMarkerStyle(graphSimPythia->GetMarkerStyle());
            ratiosimdataPythia_graph->SetMarkerSize(graphSimPythia->GetMarkerSize());
            ratiosimdataPythia_graph->SetMarkerColor(colors[11]);

            ratiosimdataPowheg_graph->SetFillColor(colors[12]);
            ratiosimdataPowheg_graph->SetLineColor(colors[12]);
            ratiosimdataPowheg_graph->SetMarkerStyle(graphSimPowheg->GetMarkerStyle());
            ratiosimdataPowheg_graph->SetMarkerSize(graphSimPowheg->GetMarkerSize());
            ratiosimdataPowheg_graph->SetMarkerColor(colors[12]);

            TH1D *ratiosimdataPythiaSys = (TH1D*)vecSpectrumSys.at(radius-minradius)->Clone(Form("ratiosimdatalowerPythiasys_%i_%i",radius,outlier));
            ratiosimdataPythiaSys->Divide(vecSpectrumSimPythia[outlier].at(radius-minradius), vecSpectrumSys.at(radius-minradius),1,1);
            SetStyleHistoTH1ForGraphs(ratiosimdataPythiaSys,"","#it{p}_{T} (GeV/#it{c})","Ratio",textSize,0.04,textSize,0.04,1,1.5);
            ratiosimdataPythiaSys->GetYaxis()->SetRangeUser(0.96,2.4);
            TGraphErrors *ratiosimdataPythiasys_graph = new TGraphErrors(ratiosimdataPythiaSys);

            TH1D *ratiosimdataPowhegSys = (TH1D*)vecSpectrumSys.at(radius-minradius)->Clone(Form("ratiosimdatalowerPowhegsys_%i_%i",radius,outlier));
            ratiosimdataPowhegSys->Divide(vecSpectrumSimPowheg.at(radius-minradius), vecSpectrumSys.at(radius-minradius),1,1);
            SetStyleHistoTH1ForGraphs(ratiosimdataPowhegSys,"","#it{p}_{T} (GeV/#it{c})","Ratio",textSize,0.04,textSize,0.04,1,1.5);
            ratiosimdataPowhegSys->GetYaxis()->SetRangeUser(0.96,2.4);
            TGraphErrors *ratiosimdataPowhegsys_graph = new TGraphErrors(ratiosimdataPowhegSys);

            ratiosimdataPythiasys_graph->SetFillColor(colors[11]);
            ratiosimdataPythiasys_graph->SetLineColor(colors[11]);
            ratiosimdataPythiasys_graph->SetFillStyle(1);

            ratiosimdataPowhegsys_graph->SetFillColor(colors[12]);
            ratiosimdataPowhegsys_graph->SetLineColor(colors[12]);
            ratiosimdataPowhegsys_graph->SetFillStyle(1);

            for(int point = (ratiosimdataPythiasys_graph->GetN())-1; point >= 0; point--){
                ratiosimdataPythiasys_graph->SetPointError(point, ratiosimdataPythiasys_graph->GetErrorX(point), ((vecSpectrumSys.at(radius-minradius)->GetBinError(point))/(vecSpectrumSys.at(radius-minradius)->GetBinContent(point)))*ratiosimdataPythiasys_graph->GetPointY(point));
            }

            for(int point = (ratiosimdataPowhegsys_graph->GetN())-1; point >= 0; point--){
                ratiosimdataPowhegsys_graph->SetPointError(point, ratiosimdataPowhegsys_graph->GetErrorX(point), ((vecSpectrumSys.at(radius-minradius)->GetBinError(point))/(vecSpectrumSys.at(radius-minradius)->GetBinContent(point)))*ratiosimdataPowhegsys_graph->GetPointY(point));
            }

            for(int point = (graphSimPythia->GetN())-1; point >= 0; point--){
                if(graphSimPythia->GetPointX(point) > 240) graphSimPythia->RemovePoint(point);
                if(graphSimPythia->GetPointX(point) < 20) graphSimPythia->RemovePoint(point);
            }

            for(int point = (graphSimPowheg->GetN())-1; point >= 0; point--){
                if(graphSimPowheg->GetPointX(point) > 240) graphSimPowheg->RemovePoint(point);
                if(graphSimPowheg->GetPointX(point) < 20) graphSimPowheg->RemovePoint(point);
            }

            for(int point = (ratiosimdataPythia_graph->GetN())-1; point >= 0; point--){
                if(ratiosimdataPythia_graph->GetPointX(point) > 240){
                    ratiosimdataPythia_graph->RemovePoint(point);
                    ratiosimdataPythiasys_graph->RemovePoint(point);
                }
                if(ratiosimdataPythia_graph->GetPointX(point) < 20){
                    ratiosimdataPythia_graph->RemovePoint(point);
                    ratiosimdataPythiasys_graph->RemovePoint(point);
                }
            }

            for(int point = (ratiosimdataPowheg_graph->GetN())-1; point >= 0; point--){
                if(ratiosimdataPowheg_graph->GetPointX(point) > 240){
                    ratiosimdataPowheg_graph->RemovePoint(point);
                    ratiosimdataPowhegsys_graph->RemovePoint(point);
                }
                if(ratiosimdataPowheg_graph->GetPointX(point) < 20){
                    ratiosimdataPowheg_graph->RemovePoint(point);
                    ratiosimdataPowhegsys_graph->RemovePoint(point);
                }
            }

            legendSimNew = GetAndSetLegend2(0.63,0.68-3*textsizeLabelsWidth*0.8,0.88,0.68,textsizeLabelsWidth*0.8);
            legendErrorKeyNew =  GetAndSetLegend2(0.19,0.04,0.39,0.04+2*textsizeLabelsWidth*0.8,textsizeLabelsWidth*0.8);

            legendErrorKeyNew->AddEntry(vecSpectrumSysGraph.at(0), "Systematic Uncertainty", "f");
            legendErrorKeyNew->AddEntry(vecSpectrumGraph.at(0), "Statistical Uncertainty", "e");

            legendSimNew->AddEntry(finalSpectrum, "ALICE Data", "p");
            legendSimNew->AddEntry(graphSimPythia, "PYTHIA8 Monash", "p");
            legendSimNew->AddEntry(graphSimPowheg, "POWHEG+PYTHIA8", "p");

            finalSpectrumSys->Draw("e2,same");
            finalSpectrum->Draw("p,e1,same");
            graphSimPythia->Draw("p,e1,same");
            graphSimPowheg->Draw("p,e1,same");
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

            ratiosimdataPythiasys_graph->Draw("e2,same");
            ratiosimdataPythia_graph->Draw("p,e1,same");
            ratiosimdataPowhegsys_graph->Draw("e2,same");
            ratiosimdataPowheg_graph->Draw("p,e1,same");
            DrawGammaLines(0.,260.,1.,1.,8.,16,9);

            canvas->Update();

            canvas->SaveAs(Form("%s/MCGen/MCComp_R0%i_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileType.Data()));
            legendSimNew->Clear();
            legendErrorKeyNew->Clear();

            dummyHistUpper = NULL;
            delete dummyHistUpper;
            upperPad->Clear();
            lowerPad->Clear();
            topPad->Clear();
            canvas->Clear();
      }
    }

    ///////////////////////////////////////
    // Plot ratio simulation comparisons //
    ///////////////////////////////////////
    cRatio->cd();
    for(int radius = minradius+1; radius <= maxradius; radius++){
      for(int outlier = 0; outlier < nOutlier; outlier++){
          TGraphErrors *finalRatio = (TGraphErrors*)vecRatioGraph.at(radius-minradius-1)->Clone(Form("finalSpectrum_R0%i_%s", radius, outliers[outlier].Data()));
          TGraphErrors *finalRatioSys = (TGraphErrors*)vecRatioSysGraph.at(radius-minradius-1)->Clone(Form("finalSpectrumSys_R0%i_%s", radius, outliers[outlier].Data()));
          
          TH1D *dummyratiosimPythia = (TH1D*)vecSpectrumSimPythia[outlier].at(0)->Clone(Form("dummyratiosimPythia_%i_%i",radius,outlier));
          dummyratiosimPythia->Divide(vecSpectrumSimPythia[outlier].at(0), vecSpectrumSimPythia[outlier].at(radius-minradius),1,1);
          TGraphErrors *dummyratiosimPythia_graph = new TGraphErrors(dummyratiosimPythia);

          TH1D *dummyratiosimPowheg = (TH1D*)vecSpectrumSimPowheg.at(0)->Clone(Form("dummyratiosimPowheg_%i_%i",radius,outlier)); 
          dummyratiosimPowheg->Divide(vecSpectrumSimPowheg.at(0), vecSpectrumSimPowheg.at(radius-minradius),1,1);
          TGraphErrors *dummyratiosimPowheg_graph = new TGraphErrors(dummyratiosimPowheg);

          for(int point = (dummyratiosimPythia_graph->GetN())-1; point >= 0; point--){
              if(dummyratiosimPythia_graph->GetPointX(point) > 240) dummyratiosimPythia_graph->RemovePoint(point);
              if(dummyratiosimPythia_graph->GetPointX(point) < 20) dummyratiosimPythia_graph->RemovePoint(point);
          }

            for(int point = (dummyratiosimPowheg_graph->GetN())-1; point >= 0; point--){
                if(dummyratiosimPowheg_graph->GetPointX(point) > 240) dummyratiosimPowheg_graph->RemovePoint(point);
                if(dummyratiosimPowheg_graph->GetPointX(point) < 20) dummyratiosimPowheg_graph->RemovePoint(point);
            }

          dummyRatio->Draw("axis");
          finalRatioSys->Draw("e2,same");
          finalRatio->Draw("p,e1,same");

          legendSimRatio->AddEntry(finalRatio, "ALICE Data", "p");
          legendSimRatio->AddEntry(dummyratiosimPythia_graph, "PYTHIA8 Monash", "p");
          legendSimRatio->AddEntry(dummyratiosimPowheg_graph, "POWHEG+PYTHIA8", "p");
          dummyratiosimPythia_graph->SetMarkerColor(colors[11]);
          dummyratiosimPythia_graph->SetLineColor(colors[11]);
          dummyratiosimPythia_graph->Draw("p,e1,same");
          dummyratiosimPowheg_graph->SetMarkerColor(colors[12]);
          dummyratiosimPowheg_graph->SetLineColor(colors[12]);
          dummyratiosimPowheg_graph->Draw("p,e1,same");
          line->Draw("same");
          legendSimRatio->Draw();
          legendErrorKeyRatio->Draw();

          //drawLatexAdd("ALICE Preliminary",0.17,0.92, textSize,kFALSE, kFALSE, false);
          drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.17,0.88, textSize,kFALSE, kFALSE, false);
          drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.2/#it{R}=0.%i",radius),0.17,0.84, textSize,kFALSE, kFALSE, false);
          drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.17,0.8, textSize,kFALSE, kFALSE, false);
          drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.17,0.76, textSize,kFALSE, kFALSE, false);

          cRatio->SaveAs(Form("%s/MCGen/MCComp_Ratio_R0%i02_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileType.Data()));
          legendSimRatio->Clear();
      }
    }

    /////////////////////////////////////////////////////////////
    // Plot combined ratio simulation comparisons (HP request) //
    /////////////////////////////////////////////////////////////
    cRatio->cd();
    for(int outlier = 0; outlier < nOutlier; outlier++){

    dummyRatio->Draw("axis");
    for(int radius = minradius+1; radius <= maxradius; radius++){
          TGraphErrors *finalRatio = (TGraphErrors*)vecRatioGraph.at(radius-minradius-1)->Clone(Form("finalSpectrum_R0%i_%s", radius, outliers[outlier].Data()));
          TGraphErrors *finalRatioSys = (TGraphErrors*)vecRatioSysGraph.at(radius-minradius-1)->Clone(Form("finalSpectrumSys_R0%i_%s", radius, outliers[outlier].Data()));
          
          TH1D *dummyratiosimPythia = (TH1D*)vecSpectrumSimPythia[outlier].at(0)->Clone(Form("dummyratiosimPythia_%i_%i",radius,outlier));
          dummyratiosimPythia->Divide(vecSpectrumSimPythia[outlier].at(0), vecSpectrumSimPythia[outlier].at(radius-minradius),1,1);
          TGraphErrors *dummyratiosimPythia_graph = new TGraphErrors(dummyratiosimPythia);
          dummyratiosimPythia_graph->SetFillColorAlpha(colors[14-radius],0.35);
          dummyratiosimPythia_graph->SetLineColor(colors[14-radius]);

          TH1D *dummyratiosimPowheg = (TH1D*)vecSpectrumSimPowheg.at(0)->Clone(Form("dummyratiosimPowheg_%i_%i",radius,outlier));
          dummyratiosimPowheg->Divide(vecSpectrumSimPowheg.at(0), vecSpectrumSimPowheg.at(radius-minradius),1,1);
          TGraphErrors *dummyratiosimPowheg_graph = new TGraphErrors(dummyratiosimPowheg);
          dummyratiosimPowheg_graph->SetFillColorAlpha(colors[10-radius],0.35);
          dummyratiosimPowheg_graph->SetLineColor(colors[10-radius]);

          for(int point = (dummyratiosimPythia_graph->GetN())-1; point >= 0; point--){
              if(radius==5){
                if(dummyratiosimPythia_graph->GetPointX(point) > 160) dummyratiosimPythia_graph->RemovePoint(point);
              }else{
                if(dummyratiosimPythia_graph->GetPointX(point) > 240) dummyratiosimPythia_graph->RemovePoint(point);
              }
              if(dummyratiosimPythia_graph->GetPointX(point) < 20) dummyratiosimPythia_graph->RemovePoint(point);
          }

          for(int point = (dummyratiosimPowheg_graph->GetN())-1; point >= 0; point--){
              if(radius==5){
                if(dummyratiosimPowheg_graph->GetPointX(point) > 160) dummyratiosimPowheg_graph->RemovePoint(point);
              }else{
                if(dummyratiosimPowheg_graph->GetPointX(point) > 240) dummyratiosimPowheg_graph->RemovePoint(point);
              }
              if(dummyratiosimPowheg_graph->GetPointX(point) < 20) dummyratiosimPowheg_graph->RemovePoint(point);
          }

          finalRatioSys->Draw("e2,same");
          finalRatio->Draw("p,e1,same");

          dummyratiosimPythia_graph->SetLineWidth(3);
          dummyratiosimPowheg_graph->SetLineWidth(3);

          legendSimRatioCombined->AddEntry(finalRatio, Form("R = 0.2/0.%i |", radius), "");
          legendSimRatioCombined->AddEntry(finalRatio, "          |   ", "p");
          legendSimRatioCombined->AddEntry(dummyratiosimPythia_graph, "        |    ", "l");
          legendSimRatioCombined->AddEntry(dummyratiosimPowheg_graph, "    ", "l");
          dummyratiosimPythia_graph->Draw("l,e3,same");
          dummyratiosimPowheg_graph->Draw("l,e3,same");

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

    cRatio->SaveAs(Form("%s/MCGen/MCComp_RatioCombined_%s.%s",output.Data(),outliers[outlier].Data(),fileType.Data()));
    legendRatio->Clear();
}

    ///////////////////////////////////////
    // Plot ratio of sim to data //
    ///////////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
      for(int outlier = 0; outlier < nOutlier; outlier++){
        TH1D *ratiosimdataPythia = (TH1D*)vecSpectrum.at(radius-minradius)->Clone(Form("ratiosimdataPythia_%i_%i",radius,outlier));
        ratiosimdataPythia->Divide(vecSpectrumSimPythia[outlier].at(radius-minradius), vecSpectrum.at(radius-minradius),1,1);
        SetStyleHistoTH1ForGraphs(ratiosimdataPythia,"","#it{p}_{T} (GeV/#it{c})","#frac{d#it{#sigma}^{sim}}{d#it{p}_{T}d#it{#eta}} / #frac{d#it{#sigma}^{data}}{d#it{p}_{T}d#it{#eta}}",textSize,0.04,textSize,0.04,1,1.5);
        ratiosimdataPythia->GetYaxis()->SetRangeUser(0.96,2.4);
        TGraphErrors *ratiosimdataPythia_graph = new TGraphErrors(ratiosimdataPythia);

        TH1D *ratiosimdataPowheg = (TH1D*)vecSpectrum.at(radius-minradius)->Clone(Form("ratiosimdataPowheg_%i_%i",radius,outlier));
        ratiosimdataPowheg->Divide(vecSpectrumSimPowheg.at(radius-minradius), vecSpectrum.at(radius-minradius),1,1);
        SetStyleHistoTH1ForGraphs(ratiosimdataPowheg,"","#it{p}_{T} (GeV/#it{c})","#frac{d#it{#sigma}^{sim}}{d#it{p}_{T}d#it{#eta}} / #frac{d#it{#sigma}^{data}}{d#it{p}_{T}d#it{#eta}}",textSize,0.04,textSize,0.04,1,1.5);
        ratiosimdataPowheg->GetYaxis()->SetRangeUser(0.96,2.4);
        TGraphErrors *ratiosimdataPowheg_graph = new TGraphErrors(ratiosimdataPowheg);

        dummyRatioSimData->Draw("axis");
        ratiosimdataPythia_graph->Draw("p,e,same");
        ratiosimdataPowheg_graph->Draw("p,e,same");
        line->Draw("same");

        drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.19,0.91, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.%i",radius),0.19,0.87, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.19,0.83, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.19,0.79, textSize,kFALSE, kFALSE, false);

        cRatio->SaveAs(Form("%s/MCGen/ratioDataMC/ratio_simdata_R0%i_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileType.Data()));
      }
    }

    fOutput->Close();
    delete fOutput;
}
