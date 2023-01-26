#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

void plotRpARejComp(TString ppMC, TString pA1, TString pA2, TString pA3, TString pA4, TString outputdir)
{
    double textSize  = 0.04;
    int radius    = 2;
    TString jetType  = "Full";
    TString fileType = "png";
    std::vector<TString> pAMC{pA1, pA2, pA3, pA4};
    std::vector<TString> pAnames{"HeaderFilterNew", "HeaderFilter", "EPOSCorrected", "No Rejection"};

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    const int nFiles_pA = pAMC.size();
    vector<TH1D*> vecSpec_pA[nFiles_pA];
    vector<TH1D*> vecClus_pA[nFiles_pA];
    vector<TH1D*> vecSpec_pp;
    vector<TH1D*> vecClus_pp;
    vector<double> detlevelbin = getJetPtBinningNonLinSmearPoor();
    double xsec_pp   = 55.8;
    double xsec_pPb  = 2095.;
    double nucfactor = 208.;
    vector<TString> triggers_pp{"INT7","EMC7","EJE"};
    vector<TString> triggers_pA{"INT7","EJ2","EJ1"};
    vector<TString> triggerNames{"Min. Bias","EMCal L0","EMCal L1-Jet"};

    double RpA_scale = (xsec_pPb)/(xsec_pp*nucfactor);

    TFile *fpp = TFile::Open(ppMC);
    if(!fpp || fpp->IsZombie()){
        cout << "Sim pp file not found!" << endl;
        return;
    }
    for(int tpp = 0; tpp < triggers_pp.size(); tpp++){
        TString dirname_pp = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers_pp.at(tpp).Data());

        TDirectory *dir_pp = (TDirectory*)fpp->Get(dirname_pp.Data());
        if(!dir_pp){
            dirname_pp = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers_pp.at(tpp).Data());
            dir_pp = (TDirectory*)fpp->Get(dirname_pp.Data());
        }
        TList *list_pp     = (TList*)dir_pp->Get(dirname_pp.Data());
        TH1D *events_pp    = (TH1D*)list_pp->FindObject("hClusterCounter");
        TH2D *spec2d_pp    = (TH2D*)list_pp->FindObject("hJetSpectrum");
        TH1D *clus1d_pp    = (TH1D*)list_pp->FindObject("hClusterEnergy1D");
        TH1D *spec1d_pp    = (TH1D*)spec2d_pp->ProjectionY(Form("spec1d_R0%i",radius),1,1);
        TH1D *rspec_pp     = (TH1D*)spec1d_pp->Rebin(detlevelbin.size()-1, Form("rspec_pp_%i_%s",radius,triggers_pp.at(tpp).Data()), detlevelbin.data());
        TH1D *rclus_pp     = (TH1D*)clus1d_pp->Rebin(detlevelbin.size()-1, Form("rclus_pp_%i_%s",radius,triggers_pp.at(tpp).Data()), detlevelbin.data());
        rspec_pp->Scale(1.,"width");
        rspec_pp->Scale(1/events_pp->GetBinContent(1));
        rclus_pp->Scale(1.,"width");
        rclus_pp->Scale(1/events_pp->GetBinContent(1));
        vecSpec_pp.push_back(rspec_pp);
        vecClus_pp.push_back(rclus_pp);
    }

    for(int ifile = 0; ifile < pAMC.size(); ifile++){
        TFile *f = TFile::Open(pAMC.at(ifile));
        if(!f || f->IsZombie()){
            cout << Form("Sim pPb file %i of %i not found!", ifile+1, 3) << endl;
            return;
        }
        for(int t = 0; t < triggers_pA.size(); t++){
            TString dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers_pA.at(t).Data());

            TDirectory *dir = (TDirectory*)f->Get(dirname.Data());
            if(!dir){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers_pA.at(t).Data());
                dir = (TDirectory*)f->Get(dirname.Data());
            }
            TList *list     = (TList*)dir->Get(dirname.Data());
            TH1D *events    = (TH1D*)list->FindObject("hClusterCounter");
            TH2D *spec2d    = (TH2D*)list->FindObject("hJetSpectrum");
            TH1D *clus1d    = (TH1D*)list->FindObject("hClusterEnergy1D");
            TH1D *spec1d    = (TH1D*)spec2d->ProjectionY(Form("spec1d_R0%i",radius),1,1);
            TH1D *rspec     = (TH1D*)spec1d->Rebin(detlevelbin.size()-1, Form("rspec_pA_%i_%s_file%i",radius,triggers_pA.at(t).Data(),ifile), detlevelbin.data());
            TH1D *rclus     = (TH1D*)clus1d->Rebin(detlevelbin.size()-1, Form("rclus_pA_%i_%s_file%i",radius,triggers_pA.at(t).Data(),ifile), detlevelbin.data());
            rspec->Scale(1.,"width");
            rspec->Scale(1/events->GetBinContent(1));
            rclus->Scale(1.,"width");
            rclus->Scale(1/events->GetBinContent(1));
            vecSpec_pA[ifile].push_back(rspec);
            vecClus_pA[ifile].push_back(rclus);
        }
    }

    TCanvas* canvas       = new TCanvas("canvas","",10,10,750,500);
    double leftMargin   = 0.1;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.13,(0.28-(5)*textSize),0.43,0.28,textSize,2);

    for(int t=0; t<triggers_pA.size(); t++){
        // Plot simulation jets for all pp triggers in one plot
        for(int ifile = 0; ifile < pAMC.size(); ifile++){
            vecSpec_pA[ifile].at(t)->SetMarkerStyle(styles[ifile]);
            vecSpec_pA[ifile].at(t)->SetMarkerColor(colors[ifile]);
            vecSpec_pA[ifile].at(t)->SetLineColor(colors[ifile]);
            vecSpec_pA[ifile].at(t)->GetXaxis()->SetRangeUser(20,240);
            if(t==0) vecSpec_pA[ifile].at(t)->GetYaxis()->SetRangeUser(1e-12,1e-3);
            else vecSpec_pA[ifile].at(t)->GetYaxis()->SetRangeUser(1e-10,1e-1);
            SetStyleHistoTH1ForGraphs(vecSpec_pA[ifile].at(t),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
            if(ifile==0) vecSpec_pA[ifile].at(t)->Draw("p,e");
            else vecSpec_pA[ifile].at(t)->Draw("p,e,same");
            legend->AddEntry(vecSpec_pA[ifile].at(t), Form("%s", pAnames.at(ifile).Data()), "p");
        }

        vecSpec_pp.at(t)->SetMarkerStyle(styles[pAMC.size()]);
        vecSpec_pp.at(t)->SetMarkerColor(colors[pAMC.size()]);
        vecSpec_pp.at(t)->SetLineColor(colors[pAMC.size()]);
        vecSpec_pp.at(t)->GetXaxis()->SetRangeUser(20,240);
        //vecSpec_pp.at(t)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpec_pp.at(t),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpec_pp.at(t)->Draw("p,e,same");
        legend->AddEntry(vecSpec_pp.at(t), "pp sim", "p");

        legend->Draw();
        drawLatexAdd("pp [pPb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation",0.93,0.82, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("%s",triggerNames.at(t).Data()),0.93,0.78, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Scaled by x-secs and 1/208 (p--Pb)",0.93,0.74, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/RejectionComparison_R0%i_%s_%s.%s", outputdir.Data(), radius, triggers_pp.at(t).Data(), triggers_pA.at(t).Data(), fileType.Data()));
        legend->Clear();
    }
}
