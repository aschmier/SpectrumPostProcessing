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

TH1D *ProcessSimHisto(TH1D *spectrum, vector<double> vecBins, int r, TString outlier, int style, Color_t color){
    TH1D *hRebinned = (TH1D*)spectrum->Rebin(vecBins.size()-1, Form("hSim_R0%i_%s",r,outlier.Data()), vecBins.data());
    double radius = (0.1)*((double)r);
    hRebinned->Scale(1.,"width");
    hRebinned->Scale(1./(1.4 - 2*radius));
    hRebinned->SetMarkerStyle(style);
    hRebinned->SetMarkerSize(2.5);
    hRebinned->SetMarkerColor(color);
    hRebinned->SetLineColor(color);
    return hRebinned;
}

void plotFinalSpectrum(TString spectrumFile, TString simFile, TString systematicsDir, TString type, TString output, TString fileType)
{
    Double_t textSize     = 0.03;
    Int_t regnum          = 6;
    int minradius = 2;
    int maxradius = 6;

    //TH1D *rffit;
    //TH1D *rffitRatio;

    int scale[5] = {1,3,10,30,100};
    const int nOutlier = 4;

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};

    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    TString outliers[nOutlier] = {"nooutlier","outlier30","outlier50","outlier70"};

    vector<double> trueRebin = getJetPtBinningNonLinTrue8TeV();

    vector<TH1D*> vecSpectrum;
    vector<TH1D*> vecSpectrumSys;
    vector<TH1D*> vecSystematics;
    vector<TH1D*> vecSystematicsRatios;
    vector<TH1D*> vecRatio;
    vector<TH1D*> vecRatioSys;
    vector<TH1D*> vecSpectrumSim[nOutlier];
    vector<TH1D*> vecAllUnfolding;
    vector<TH1D*> vecTriggerSwap;
    vector<TGraphErrors*> vecSpectrumGraph;
    vector<TGraphErrors*> vecSpectrumSysGraph;
    vector<TGraphErrors*> vecRatioGraph;
    vector<TGraphErrors*> vecRatioSysGraph;

    gSystem->Exec("mkdir -p "+output+"/FinalResults");
    gSystem->Exec("mkdir -p "+output+"/MCGen");
    gSystem->Exec("mkdir -p "+output+"/MCGen/ratioDataMC");

    // Open root files
    TFile *f = TFile::Open(spectrumFile);
    if(!f || f->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    TFile *fSim = TFile::Open(simFile);
    if(!fSim || fSim->IsZombie()){
        cout << "Simulation file not found!" << endl;
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
        TString sysfile = Form("%s/ratio/systematics_R0%i.root", systematicsDir.Data(), radius);
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
          TList *simBase = (TList*)fSim->Get(Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers[outlier].Data()));
          if(!simBase){
            cout << Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers[outlier].Data()) << endl;
            cout << "No sim base found!" << endl;
            return;
          }
          TH1D *hSpec = (TH1D*)simBase->FindObject("hJetPt");
          TH1D *hSim = (TH1D*)ProcessSimHisto( hSpec, trueRebin, radius, outliers[outlier], styles[radius-minradius+1], colors[radius-minradius+1] );
          vecSpectrumSim[outlier].push_back(hSim);
        }
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

        gSpectrum->SetMarkerStyle(styles[radius-minradius]);
        gSpectrum->SetMarkerSize(2.5);
        gSpectrum->SetMarkerColor(colors[radius-minradius]);
        gSpectrum->SetLineColor(colors[radius-minradius]);
        gSpectrum->SetFillColor(colors[radius-minradius]);
        gSpectrum->SetFillStyle(3001);

        gSpectrumSys->SetFillColor(colors[radius-minradius]);
        gSpectrumSys->SetLineColor(colors[radius-minradius]);
        gSpectrumSys->SetFillStyle(0);

        gSpectrum->RemovePoint(18);
        gSpectrum->RemovePoint(17);
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
                sysError = sqrt(pow(sysError,2) + pow((vecAllUnfolding.at(radius-minradius)->GetBinContent(bin))/100,2) + pow((vecAllUnfolding.at(0)->GetBinContent(bin))/100,2) + pow((vecTriggerSwap.at(radius-minradius)->GetBinContent(bin))/100,2) + pow((vecTriggerSwap.at(0)->GetBinContent(bin))/100,2));
                Double_t scaledError     = sysError*vecRatioSys.at(radius-minradius-1)->GetBinContent(bin);
                vecRatioSys.at(radius-minradius-1)->SetBinError(bin, scaledError);
            }

            if(radius != minradius){
                // Create TGraphErrors and remove unused points for spectra
                TGraphErrors *gRatio        = new TGraphErrors(vecRatio.at(radius-minradius-1));
                TGraphErrors *gRatioSys     = new TGraphErrors(vecRatioSys.at(radius-minradius-1));

                gRatio->SetMarkerStyle(stylesfilled[radius-(minradius+1)]);
                gRatio->SetMarkerSize(2.5);
                gRatio->SetMarkerColor(colors[radius-(minradius+1)]);
                gRatio->SetLineColor(colors[radius-(minradius+1)]);
                gRatio->SetFillColor(colors[radius-(minradius+1)]);
                gRatio->SetFillStyle(3001);

                gRatioSys->SetFillColor(colors[radius-(minradius+1)]);
                gRatioSys->SetLineColor(colors[radius-(minradius+1)]);
                gRatioSys->SetFillStyle(0);

                gRatio->RemovePoint(18);
                gRatio->RemovePoint(17);
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
    DrawPaperCanvasSettings(cSpectrum,0.1,0.025,0.025,0.1);
    cSpectrum->SetLogy();

    TCanvas *cRatio   = new TCanvas("c2", "", 800, 800);
    DrawPaperCanvasSettings(cRatio,0.15,0.025,0.025,0.1);

    gStyle->SetOptStat(0);

    TLegend *legendSpectrum    =  GetAndSetLegend2(0.53,0.68,0.93,0.68+(maxradius-minradius)*textSize,textSize,2);
    TLegend *legendRatio       = GetAndSetLegend2(0.47,0.15,0.95,0.15+((maxradius-minradius+1)*textSize*1.5)/2,textSize,2);
    TLegend *legendSim     =  GetAndSetLegend2(0.55,0.67,0.95,0.67+((2)*textSize*1.5),textSize);
    TLegend *legendSimRatio = GetAndSetLegend2(0.65,0.15,0.95,0.15+((2)*textSize*1.5),textSize);

    TLegend *legendErrorKey =  GetAndSetLegend2(0.15,0.15,0.35,0.15+((2)*textSize*1.5)/2,textSize);
    legendErrorKey->AddEntry(vecSpectrumSysGraph.at(0), "Systematic Uncertainty", "f");
    legendErrorKey->AddEntry(vecSpectrumGraph.at(0), "Statistical Uncertainty", "f");

    TLegend *legendErrorKeyRatio =  GetAndSetLegend2(0.15,0.15,0.35,0.15+((2)*textSize*1.5)/2,textSize);
    legendErrorKeyRatio->AddEntry(vecRatioSysGraph.at(0), "Systematic Uncertainty", "f");
    legendErrorKeyRatio->AddEntry(vecRatioGraph.at(0), "Statistical Uncertainty", "f");

    // Set up dummy histos for plotting
    TH1D *dummySpectrum = (TH1D*)vecSpectrum.at(0)->Clone("dummySpectrum");
    dummySpectrum->GetXaxis()->SetRangeUser(0,280);
    dummySpectrum->GetYaxis()->SetRangeUser(1e-8,10);
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{p}_{T} (GeV/#it{c})","d#it{#sigma} / d#it{p}_{T}d#it{#eta} (mb/(GeV/#it{c}))",textSize,0.04,textSize,0.04,1,1.2);

    // Set up dummy histos for plotting
    TH1D *dummySpectrumLogX = (TH1D*)vecSpectrum.at(0)->Clone("dummySpectrum");
    dummySpectrumLogX->GetXaxis()->SetRangeUser(15,300);
    dummySpectrumLogX->GetYaxis()->SetRangeUser(1e-8,10);
    SetStyleHistoTH1ForGraphs(dummySpectrumLogX,"","#it{p}_{T} (GeV/#it{c})","d#it{#sigma} / d#it{p}_{T}d#it{#eta} (mb/(GeV/#it{c}))",textSize,0.04,textSize,0.04,1,1.2);

    TH1D *dummyRatio    = (TH1D*)vecRatio.at(0)->Clone("dummyRatio");
    dummyRatio->GetXaxis()->SetRangeUser(0,280);
    dummyRatio->GetYaxis()->SetRangeUser(0,1.4);
    SetStyleHistoTH1ForGraphs(dummyRatio,"","#it{p}_{T} (GeV/#it{c})","d#it{#sigma} / d#it{p}_{T}d#it{#eta} (mb/(GeV/#it{c}))",textSize,0.04,textSize,0.04,1,1.2);

    TH1D *dummyRatioSimData    = (TH1D*)vecRatio.at(0)->Clone("dummyRatio");
    dummyRatioSimData->GetXaxis()->SetRangeUser(0,280);
    dummyRatioSimData->GetYaxis()->SetRangeUser(0.96,2.4);
    SetStyleHistoTH1ForGraphs(dummyRatioSimData,"","#it{p}_{T} (GeV/#it{c})","d#it{#sigma} / d#it{p}_{T}d#it{#eta} (mb/(GeV/#it{c}))",textSize,0.04,textSize,0.04,1,1.2);

    TLine * line = new TLine (0,1,280,1);
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
        finalSpectrum->Draw("p,e2,same");
        finalSpectrumSys->Draw("e2,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.85, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

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

        //legendSpectrum->AddEntry(finalSpectrum, Form("#it{R} = 0.%i x %i",radius,scale[radius-minradius]), "p");

        if(radius==minradius) dummySpectrumLogX->Draw("axis");
        finalSpectrum->Draw("p,e2,same");
        finalSpectrumSys->Draw("e2,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.85, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

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

        if(radius==minradius) dummySpectrum->Draw("axis");
        finalSpectrum->Draw("p,e2,same");
        finalSpectrumSys->Draw("e2,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.85, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/FinalResults/%s_reg%i_unscaled.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    cSpectrum->SetLogx(1);

    //////////////////////////////////
    // Plot unscaled cross-sections //
    //////////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        TGraphErrors *finalSpectrum = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("finalSpectrum_R0%i", radius));

        TGraphErrors *finalSpectrumSys = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("finalSpectrumSys_R0%i", radius));

        //legendSpectrum->AddEntry(finalSpectrum, Form("#it{R} = 0.%i",radius), "p");

        if(radius==minradius) dummySpectrumLogX->Draw("axis");
        finalSpectrum->Draw("p,e2,same");
        finalSpectrumSys->Draw("e2,same");
    }

    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.85, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

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

        legendRatio->AddEntry(finalRatio, Form("#it{R}=0.%i/#it{R}=0.2",radius), "p");

        if(radius==(minradius+1)) dummyRatio->Draw("axis");
        finalRatio->Draw("p,e2,same");
        finalRatioSys->Draw("e2,same");

    }

    line->Draw("same");
    legendRatio->Draw("same");
    legendErrorKeyRatio->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.19,0.91, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.19,0.87, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.19,0.83, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.19,0.79, textSize,kFALSE, kFALSE, false);

    cRatio->SaveAs(Form("%s/FinalResults/%s_reg%i_Ratio.%s",output.Data(),type.Data(),regnum,fileType.Data()));

    legendRatio->Clear();

    /////////////////////////////////
    // Plot simulation comparisons //
    /////////////////////////////////
    cSpectrum->cd();
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int outlier = 0; outlier < nOutlier; outlier++){
            TGraphErrors *finalSpectrum = (TGraphErrors*)vecSpectrumGraph.at(radius-minradius)->Clone(Form("finalSpectrum_R0%i_%s", radius, outliers[outlier].Data()));
            TGraphErrors *finalSpectrumSys = (TGraphErrors*)vecSpectrumSysGraph.at(radius-minradius)->Clone(Form("finalSpectrumSys_R0%i_%s", radius, outliers[outlier].Data()));
            TGraphErrors *graphSim  = new TGraphErrors(vecSpectrumSim[outlier].at(radius-minradius));

            legendSim->AddEntry(vecSpectrum.at(radius-minradius), "ALICE Data", "p");
            legendSim->AddEntry(vecSpectrumSim[outlier].at(radius-minradius), "PYTHIA8 Monash", "p");

            dummySpectrum->Draw("axis");
            finalSpectrum->Draw("p,e2,same");
            finalSpectrumSys->Draw("e2,same");
            graphSim->Draw("p,e,same");
            legendSim->Draw();

            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.92,0.91, textSize,kFALSE, kFALSE, true);
            drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.%i",radius),0.92,0.87, textSize,kFALSE, kFALSE, true);
            drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.92,0.83, textSize,kFALSE, kFALSE, true);
            drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.92,0.79, textSize,kFALSE, kFALSE, true);

            cSpectrum->SaveAs(Form("%s/MCGen/MCComp_R0%i_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileType.Data()));
            legendSim->Clear();
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
          TH1D *dummyratiosim = (TH1D*)vecRatio.at(0)->Clone(Form("dummyratiosim_%i_%i",radius,outlier));
          dummyratiosim->Divide(vecSpectrumSim[outlier].at(0), vecSpectrumSim[outlier].at(radius-minradius),1,1,"b");
          TGraphErrors *dummyratiosim_graph = new TGraphErrors(dummyratiosim);

          dummyRatio->Draw("axis");
          finalRatio->Draw("p,e2,same");
          finalRatioSys->Draw("e2,same");

          legendSimRatio->AddEntry(vecRatio.at(radius-(minradius+1)), "ALICE Data", "p");
          legendSimRatio->AddEntry(dummyratiosim, "PYTHIA Monash", "p");
          dummyratiosim_graph->Draw("p,e,same");
          line->Draw("same");
          legendSimRatio->Draw();

          drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.19,0.91, textSize,kFALSE, kFALSE, false);
          drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.2/#it{R}=0.%i",radius),0.19,0.87, textSize,kFALSE, kFALSE, false);
          drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.19,0.83, textSize,kFALSE, kFALSE, false);
          drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.19,0.79, textSize,kFALSE, kFALSE, false);

          cRatio->SaveAs(Form("%s/MCGen/MCComp_Ratio_R0%i02_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileType.Data()));
          legendSimRatio->Clear();
      }
    }

    for(int radius = minradius; radius <= maxradius; radius++){
      for(int outlier = 0; outlier < nOutlier; outlier++){
        TH1D *ratiosimdata = (TH1D*)vecSpectrum.at(radius-minradius)->Clone(Form("ratiosimdata_%i_%i",radius,outlier));
        ratiosimdata->Divide(vecSpectrumSim[outlier].at(radius-minradius), vecSpectrum.at(radius-minradius),1,1,"b");
        SetStyleHistoTH1ForGraphs(ratiosimdata,"","#it{p}_{T} (GeV/#it{c})","#frac{d#it{#sigma}^{sim}}{d#it{p}_{T}d#it{#eta}} / #frac{d#it{#sigma}^{data}}{d#it{p}_{T}d#it{#eta}}",textSize,0.04,textSize,0.04,1,1.5);
        ratiosimdata->GetYaxis()->SetRangeUser(0.96,2.4);
        TGraphErrors *ratiosimdata_graph = new TGraphErrors(ratiosimdata);

        dummyRatioSimData->Draw("axis");
        ratiosimdata_graph->Draw("p,e,same");
        line->Draw("same");

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.19,0.3, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.%i",radius),0.19,0.26, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.19,0.22, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.19,0.18, textSize,kFALSE, kFALSE, false);

        cRatio->SaveAs(Form("%s/MCGen/ratioDataMC/ratio_simdata_R0%i_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileType.Data()));
      }
    }
}
