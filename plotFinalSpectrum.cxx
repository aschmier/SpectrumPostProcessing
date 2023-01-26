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

TH1D *ProcessSimHisto(TH1D *spectrum, TH2D *etaphi, vector<double> vecBins, int r, TString outlier, int style, Color_t color){
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
    TH1D *maxtrackpt_R02;
    TH1D *maxtrackpt_R04;
    TH1D *rfbinvar;
    TH1D *rffit;
    TH1D *ratiodummy;
    TH1D *ratiodummySysE;

    int scale[5] = {1,3,10,30,100};
    const int nOutlier = 4;

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    TString outliers[nOutlier] = {"nooutlier","outlier30","outlier50","outlier70"};

    vector<double> trueRebin = getJetPtBinningNonLinTruePoor();

    vector<TH1D*> vecSpectra;
    vector<TH1D*> vecSpectraSysE;
    vector<TH1D*> vecSystematics;
    vector<TH1D*> vecRatio;
    vector<TH1D*> vecSpectraSim[nOutlier];
    vector<TGraphErrors*> vecSpectraSysE_graph;
    vector<TGraphErrors*> vecRatioSysE;

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

    for(int radius = minradius; radius <= maxradius; radius++){
        TString sysfile = Form("%s/systematics_R0%i.root", systematicsDir.Data(), radius);
        TFile *fSys = TFile::Open(sysfile);
        if(!fSys || fSys->IsZombie()){
            cout << "Systematics file not found! --> R=0." << radius << endl;
            return;
        }
        vecSystematics.push_back((TH1D*)fSys->Get("hTotal"));
        if(radius == 2){
            maxtrackpt_R02 = (TH1D*)fSys->Get("smooth_maxtrackpt");
            rfbinvar = (TH1D*)fSys->Get("smooth_rfbinvar");
            rffit = (TH1D*)fSys->Get("smooth_rffit");
        }
        if(radius == 4) maxtrackpt_R04 = (TH1D*)fSys->Get("smooth_maxtrackpt");
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *regDir = (TDirectory*)f->Get(Form("R0%i",radius));
        TDirectory *reg    = (TDirectory*)regDir->Get(Form("reg%i",regnum));
        TH1D *normUnfolded = (TH1D*)reg->Get(Form("normalized_reg%i",regnum));
        TH1D *normUnfoldedClone = (TH1D*)normUnfolded->Clone(Form("normUnfoldedClone_%i",radius));
        vecSpectra.push_back(normUnfolded);
        vecSpectraSysE.push_back(normUnfoldedClone);
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int outlier = 0; outlier < nOutlier; outlier++){
          TList *simBase = (TList*)fSim->Get(Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers[outlier].Data()));
          if(!simBase){
            cout << Form("PartLevelJetResultsFullJetR0%i_%s",radius,outliers[outlier].Data()) << endl;
            cout << "No sim base found!" << endl;
            return;
          }
          TH1D *hSpec = (TH1D*)simBase->FindObject("hJetPt");
          TH2D *hEtaPhi = (TH2D*)simBase->FindObject("hJetEtaPhi");
          TH1D *hSim = (TH1D*)ProcessSimHisto( hSpec, hEtaPhi, trueRebin, radius, outliers[outlier], styles[radius-minradius+1], colors[radius-minradius+1] );
          vecSpectraSim[outlier].push_back(hSim);
        }
    }

    TCanvas *c   = new TCanvas("c", "", 800, 1000);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.53,0.68,0.93,0.68+(maxradius-minradius)*textSize,textSize,2);
    TLegend *legendSysE =  GetAndSetLegend2(0.15,0.15,0.35,0.15+((2)*textSize*1.5)/2,textSize);

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int bin = 1; bin <= vecSpectraSysE.at(radius-minradius)->GetNbinsX(); bin++){
            double sysError                          = (vecSystematics.at(radius-minradius)->GetBinContent(bin))/100;
            if(radius == 3)                sysError = sqrt(pow(sysError,2) + pow((maxtrackpt_R02->GetBinContent(bin))/100,2));
            if(radius == 5 || radius == 6) sysError = sqrt(pow(sysError,2) + pow((maxtrackpt_R04->GetBinError(bin))/100,2));
            if(radius != 2) sysError = sqrt(pow(sysError,2) + pow((rfbinvar->GetBinContent(bin))/100,2) + pow((rfbinvar->GetBinContent(bin))/100,2));
            Double_t scaledError = sysError*vecSpectraSysE.at(radius-minradius)->GetBinContent(bin);
            vecSpectraSysE.at(radius-minradius)->SetBinError(bin, scaledError);
        }

        vecSpectra.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecSpectra.at(radius-minradius)->SetMarkerSize(2.5);
        vecSpectra.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecSpectra.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecSpectra.at(radius-minradius)->SetFillColor(colors[radius-minradius]);
        vecSpectra.at(radius-minradius)->SetFillStyle(3001);

        // Ratio plots
        if(radius != minradius){
          ratiodummy = (TH1D*)vecSpectra.at(radius-minradius)->Clone(Form("dummy_R0%i",radius));
          ratiodummy->Divide(vecSpectra.at(0),ratiodummy,1,1,"b");
          vecRatio.push_back(ratiodummy);
          ratiodummySysE = (TH1D*)vecSpectraSysE.at(radius-minradius)->Clone(Form("dummySysE_R0%i",radius));
          ratiodummySysE->Divide(vecSpectraSysE.at(0),ratiodummySysE,1,1,"b");
          vecRatioSysE.push_back(new TGraphErrors(ratiodummySysE));
        }

        vecSpectra.at(radius-minradius)->Scale((double)scale[radius-minradius]);
        vecSpectra.at(radius-minradius)->GetXaxis()->SetRangeUser(20,320);
        vecSpectra.at(radius-minradius)->GetYaxis()->SetRangeUser(1e-8,10);
        SetStyleHistoTH1ForGraphs(vecSpectra.at(radius-minradius),"","#it{p}_{T} (GeV/#it{c})","d#it{#sigma} / d#it{p}_{T}d#it{#eta} (mb/(GeV/#it{c}))",textSize,0.04,textSize,0.04,1,1.2);
        vecSpectraSysE.at(radius-minradius)->Scale((double)scale[radius-minradius]);
        vecSpectraSysE.at(radius-minradius)->GetXaxis()->SetRangeUser(20,320);
        vecSpectraSysE.at(radius-minradius)->GetYaxis()->SetRangeUser(1e-8,10);
        TGraphErrors *graphSysE = new TGraphErrors(vecSpectraSysE.at(radius-minradius));
        graphSysE->SetFillStyle(0);
        graphSysE->SetFillColor(colors[radius-minradius]);
        graphSysE->SetLineColor(colors[radius-minradius]);
        vecSpectraSysE_graph.push_back(graphSysE);

        legend->AddEntry(vecSpectra.at(radius-minradius), Form("#it{R} = 0.%i x %i",radius,scale[radius-minradius]), "p");
        if(radius==minradius){
          vecSpectra.at(radius-minradius)->Draw("p,e2");
          graphSysE->Draw("e2,same");
        }else{
          vecSpectra.at(radius-minradius)->Draw("p,e2,same");
          graphSysE->Draw("e2,same");
        }
    }

    legend->Draw("same");

    legendSysE->AddEntry(vecSpectraSysE.at(0), "Systematic Uncertainty", "f");
    legendSysE->AddEntry(vecSpectra.at(0), "Statistical Uncertainty", "f");
    legendSysE->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.85, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

    c->SaveAs(Form("%s/FinalResults/%s_reg%i.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    c->SetLogx(1);
    c->SaveAs(Form("%s/FinalResults/%s_reg%i_logx.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    c->SetLogx(0);

    legend->Clear();
    legendSysE->Clear();

    for(int radius = minradius; radius <= maxradius; radius++){
        vecSpectra.at(radius-minradius)->Scale(1/(double)scale[radius-minradius]);
        vecSpectraSysE.at(radius-minradius)->Scale(1/(double)scale[radius-minradius]);
        vecSpectraSysE_graph.at(radius-minradius)->Scale(1./((double)scale[radius-minradius]));

        legend->AddEntry(vecSpectra.at(radius-minradius), Form("#it{R} = 0.%i",radius), "p");
        if(radius==minradius){
          vecSpectra.at(radius-minradius)->Draw("p,e2");
          vecSpectraSysE_graph.at(radius-minradius)->Draw("e2,same");
        }else{
          vecSpectra.at(radius-minradius)->Draw("p,e2,same");
          vecSpectraSysE_graph.at(radius-minradius)->Draw("e2,same");
        }
    }

    legend->Draw("same");

    legendSysE->AddEntry(vecSpectraSysE.at(0), "Systematic Uncertainty", "f");
    legendSysE->AddEntry(vecSpectra.at(0), "Statistical Uncertainty", "f");
    legendSysE->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.85, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

    c->SaveAs(Form("%s/FinalResults/%s_reg%i_unscaled.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    c->SetLogx(1);
    c->SaveAs(Form("%s/FinalResults/%s_reg%i_logx_unscaled.%s",output.Data(),type.Data(),regnum,fileType.Data()));
    c->SetLogx(0);

    TCanvas *c2   = new TCanvas("c2", "", 800, 800);
    DrawPaperCanvasSettings(c2,0.15,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    //c2->SetLogx();

    TLegend *legend2     =  GetAndSetLegend2(0.47,0.15,0.95,0.15+((maxradius-minradius+1)*textSize*1.5)/2,textSize,2);
    TLegend *legendSysE2 =  GetAndSetLegend2(0.47,0.27,0.95,0.27+((2)*textSize*1.5),textSize);

    for(int radius = minradius+1; radius <= maxradius; radius++){
      //vecRatio.at(radius-(minradius+1))->GetYaxis()->SetRangeUser(0,1.4);
      vecRatio.at(radius-(minradius+1))->GetYaxis()->SetRangeUser(0,1.4);
      vecRatio.at(radius-(minradius+1))->GetXaxis()->SetRangeUser(20,320);
      SetStyleHistoTH1ForGraphs(vecRatio.at(radius-(minradius+1)),"","#it{p}_{T} (GeV/#it{c})","#frac{d#it{#sigma}^{#it{R}=0.2}}{d#it{p}_{T}d#it{#eta}} / #frac{d#it{#sigma}^{#it{R}=0.#it{X}}}{d#it{p}_{T}d#it{#eta}}",textSize,0.04,textSize,0.04,1,1.5);
      vecRatio.at(radius-(minradius+1))->SetFillColor(colors[radius-(minradius+1)]);
      vecRatio.at(radius-(minradius+1))->SetMarkerStyle(styles[radius-(minradius+1)]);
      vecRatio.at(radius-(minradius+1))->SetMarkerSize(2.5);
      vecRatio.at(radius-(minradius+1))->SetMarkerColor(colors[radius-(minradius+1)]);
      vecRatio.at(radius-(minradius+1))->SetLineColor(colors[radius-(minradius+1)]);
      vecRatio.at(radius-(minradius+1))->SetFillColor(colors[radius-(minradius+1)]);
      vecRatio.at(radius-(minradius+1))->SetFillStyle(3001);

      vecRatioSysE.at(radius-(minradius+1))->SetFillStyle(0);
      vecRatioSysE.at(radius-(minradius+1))->SetFillColor(colors[radius-(minradius+1)]);
      vecRatioSysE.at(radius-(minradius+1))->SetLineColor(colors[radius-(minradius+1)]);

      legend2->AddEntry(vecRatio.at(radius-(minradius+1)), Form("#it{R}=0.%i/#it{R}=0.2",radius), "p");
      if(radius==(minradius+1)){
        vecRatio.at(radius-(minradius+1))->Draw("p,e2");
        vecRatioSysE.at(radius-(minradius+1))->Draw("e2,same");
      }else{
        vecRatio.at(radius-(minradius+1))->Draw("p,e2,same");
        vecRatioSysE.at(radius-(minradius+1))->Draw("e2,same");
      }
    }

    TLine * l1 = new TLine (20,1,320,1);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);
    l1->Draw("same");

    legend2->Draw("same");

    legendSysE2->AddEntry(vecRatioSysE.at(0), "Systematic Uncertainty", "f");
    legendSysE2->AddEntry(vecRatio.at(0), "Statistical Uncertainty", "f");
    legendSysE2->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.19,0.91, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.19,0.87, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.19,0.83, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.19,0.79, textSize,kFALSE, kFALSE, false);

    c2->SaveAs(Form("%s/FinalResults/%s_reg%i_Ratio.%s",output.Data(),type.Data(),regnum,fileType.Data()));

    TCanvas *c3   = new TCanvas("c3", "", 800, 1000);
    DrawPaperCanvasSettings(c3,0.15,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c3->SetLogy();

    TLegend *legendsim     =  GetAndSetLegend2(0.55,0.67,0.95,0.67+((2)*textSize*1.5),textSize);

    for(int radius = minradius; radius <= maxradius; radius++){
      //vecSpectra.at(radius-minradius)->Scale(1./((double)scale[radius-minradius]));
      //vecSpectraSysE_graph.at(radius-minradius)->Scale(1./((double)scale[radius-minradius]));
      SetStyleHistoTH1ForGraphs(vecSpectra.at(radius-minradius),"","#it{p}_{T} (GeV/#it{c})","#frac{d#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb/(GeV/#it{c}))",textSize,0.04,textSize,0.04,1,1.6);
      for(int outlier = 0; outlier < nOutlier; outlier++){
        vecSpectra.at(radius-minradius)->Draw("p,e2");
        vecSpectraSysE_graph.at(radius-minradius)->Draw("e2,same");
        vecSpectraSim[outlier].at(radius-minradius)->Draw("p,e,same");
        legendsim->AddEntry(vecSpectra.at(radius-minradius), "ALICE Data", "p");
        legendsim->AddEntry(vecSpectraSim[outlier].at(radius-minradius), "PYTHIA8 Monash", "p");
        legendsim->Draw();
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.92,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.%i",radius),0.92,0.87, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.92,0.83, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.92,0.79, textSize,kFALSE, kFALSE, true);
        c3->SaveAs(Form("%s/MCGen/MCComp_R0%i_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileType.Data()));
        legendsim->Clear();
      }
    }

    TCanvas *c4   = new TCanvas("c4", "", 800, 800);
    DrawPaperCanvasSettings(c4,0.15,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

    TLine * l2 = new TLine (20,1,320,1);
    l2->SetLineColor(14);
    l2->SetLineWidth(3);
    l2->SetLineStyle(7);

    TLegend *legendsimratio = GetAndSetLegend2(0.65,0.15,0.95,0.15+((2)*textSize*1.5),textSize);

    for(int radius = minradius; radius < maxradius; radius++){
      SetStyleHistoTH1ForGraphs(vecRatio.at(radius-minradius),"","#it{p}_{T} (GeV/#it{c})","#frac{d#it{#sigma}^{#it{R}=0.2}}{d#it{p}_{T}d#it{#eta}} / #frac{d#it{#sigma}^{#it{R}=0.#it{X}}}{d#it{p}_{T}d#it{#eta}}",textSize,0.04,textSize,0.04,1,1.5);
      for(int outlier = 0; outlier < nOutlier; outlier++){
        vecRatio.at(radius-minradius)->Draw("p,e2");
        vecRatioSysE.at(radius-minradius)->Draw("e2,same");
        TH1D *dummyratiosim = (TH1D*)vecRatio.at(0)->Clone(Form("dummyratiosim_%i_%i",radius,outlier));
        dummyratiosim->Divide(vecSpectraSim[outlier].at(0), vecSpectraSim[outlier].at(radius-minradius+1),1,1,"b");
        legendsimratio->AddEntry(vecRatio.at(radius-minradius), "ALICE Data", "p");
        legendsimratio->AddEntry(dummyratiosim, "PYTHIA Monash", "p");
        dummyratiosim->Draw("p,e,same");
        l2->Draw("same");
        legendsimratio->Draw();

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.19,0.91, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.2/#it{R}=0.%i",radius+1),0.19,0.87, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.19,0.83, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.19,0.79, textSize,kFALSE, kFALSE, false);

        c4->SaveAs(Form("%s/MCGen/MCComp_Ratio_R0%i_%s.%s",output.Data(),radius+1,outliers[outlier].Data(),fileType.Data()));
        legendsimratio->Clear();
      }
    }

    TCanvas *c5   = new TCanvas("c5", "", 800, 800);
    DrawPaperCanvasSettings(c5,0.15,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

    TLine * l3 = new TLine (20,1,320,1);
    l3->SetLineColor(14);
    l3->SetLineWidth(3);
    l3->SetLineStyle(7);

    for(int radius = minradius; radius <= maxradius; radius++){
      for(int outlier = 0; outlier < nOutlier; outlier++){
        TH1D *ratiosimdata = (TH1D*)vecSpectra.at(radius-minradius)->Clone(Form("ratiosimdata_%i_%i",radius,outlier));
        ratiosimdata->Divide(vecSpectraSim[outlier].at(radius-minradius), vecSpectra.at(radius-minradius),1,1,"b");
        SetStyleHistoTH1ForGraphs(ratiosimdata,"","#it{p}_{T} (GeV/#it{c})","#frac{d#it{#sigma}^{sim}}{d#it{p}_{T}d#it{#eta}} / #frac{d#it{#sigma}^{data}}{d#it{p}_{T}d#it{#eta}}",textSize,0.04,textSize,0.04,1,1.5);
        ratiosimdata->GetYaxis()->SetRangeUser(0.96,2.4);
        ratiosimdata->Draw("p,e");
        l2->Draw("same");

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.19,0.3, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R}=0.%i",radius),0.19,0.26, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.19,0.22, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.19,0.18, textSize,kFALSE, kFALSE, false);

        c5->SaveAs(Form("%s/MCGen/ratioDataMC/ratio_simdata_R0%i_%s.%s",output.Data(),radius,outliers[outlier].Data(),fileType.Data()));
      }
    }
}
