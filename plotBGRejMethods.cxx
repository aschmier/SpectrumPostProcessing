// Macro for comparing rejection factors in pp 2012 jet data

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
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

void plotBGRejMethods(TString NoSub, TString PerpCone, TString RhoSparse, TString output, int radius, TString fileType)
{
    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Setup ////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    Double_t textSize     = 0.04;

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    vector<double> detlevelbin = getJetPtBinningNonLinSmear8TeV();

    vector<TString> triggers{"EJ2","EJ1"};
    vector<TString> fileNames{NoSub,PerpCone,RhoSparse};
    vector<TString> rejTypes{"NoSub", "PerpCone", "RhoSparse"};
    vector<TString> typesPlot{"No Subtraction", "Perpendicular Cones", "Rho Sparse"};
    TH1D *plots[3][3];
    TString jetType = "Full";

    // Plotting stuff
    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.075,0.01,0.01,0.1);
    gStyle->SetOptStat(0);

    TH1D *dummy = new TH1D("dummy","",350,0,350);
    dummy->GetXaxis()->SetRangeUser(0,260);
    dummy->GetYaxis()->SetRangeUser(0.6,1.21);
    SetStyleHistoTH1ForGraphs(dummy,"","#it{p}_{T} (GeV/c)","Ratio",textSize,textSize*(4/3),textSize,textSize*(4/3),1.1,0.9);

    TLine * l1 = new TLine (0,1,260,1);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);

    TLine * l2 = new TLine (20,0.6,20,1.21);
    l2->SetLineColor(14);
    l2->SetLineWidth(3);
    l2->SetLineStyle(7);

    TLine * l3 = new TLine (240,0.6,240,1.21);
    l3->SetLineColor(14);
    l3->SetLineWidth(3);
    l3->SetLineStyle(7);

    TLegend *legendFoldRaw =  GetAndSetLegend2(0.09,0.79-(2)*textSize,0.29,0.79,textSize);

    ////////////////////////////////////////////////////////////////////////////
    ///////////////////////////// Get the histograms ///////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    for(int t = 0; t < triggers.size(); t++){
        for(int f = 0; f < fileNames.size(); f++){
            TFile *file = TFile::Open(fileNames.at(f), "read");
            if(!file || file->IsZombie()) return;

            TString dirname     = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers.at(t).Data());
            TDirectory *dir = (TDirectory*)file->Get(dirname.Data());
            if(!dir){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers.at(t).Data());
                dir = (TDirectory*)file->Get(dirname.Data());
            }

            if(!dir){
                cout << "Could not find directory " << dirname << endl;
                cout << "File: " << fileNames.at(f) << endl;
                cout << "Trigger: " << triggers.at(t) << endl;
                return;
            }

            TList *list     = (TList*)dir->Get(dirname.Data());
            TH1D *events    = (TH1D*)list->FindObject("hClusterCounter");
            TH2D *spec2d    = (TH2D*)list->FindObject("hJetSpectrum");
            TH1D *spec1d    = (TH1D*)spec2d->ProjectionY(Form("dataspec1d_R0%i_%s",radius,triggers.at(t).Data()),1,1);
            TH1D *rspec     = (TH1D*)spec1d->Rebin(detlevelbin.size()-1, Form("spec_%s_%s",rejTypes.at(f).Data(),triggers.at(t).Data()), detlevelbin.data());
            rspec->Scale(1.,"width");
            rspec->Scale(1/events->GetBinContent(1));
            plots[f][t] = rspec;
        }

        TH1D *PerpConeToNoSub = (TH1D*)plots[0][0]->Clone(Form("PerpConeToNoSub_%s",triggers.at(t).Data()));
        PerpConeToNoSub->Divide(plots[1][t], plots[0][t]);
        PerpConeToNoSub->Sumw2();
        PerpConeToNoSub->SetMarkerStyle(4);
        PerpConeToNoSub->SetLineColor(kBlack);
        PerpConeToNoSub->SetMarkerColor(kBlack);
        PerpConeToNoSub->SetMarkerSize(2.5);

        dummy->Draw("axis");
        l1->Draw("same");
        l2->Draw("same");
        l3->Draw("same");
        PerpConeToNoSub->Draw("p,e1,same");

        drawLatexAdd("#sqrt{#it{s}_{NN}} = 8.16 TeV",0.11,0.30, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.11,0.26, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Trigger: %s",triggers.at(t).Data()),0.11,0.22, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("Statistical Errors Only",0.11,0.18, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("Perpendicular Cones / No Subtraction",0.11,0.14, textSize,kFALSE, kFALSE, false);

        c->SaveAs(Form("%s/PerpConeToNoSub_R0%i_%s.%s",output.Data(),radius,triggers.at(t).Data(),fileType.Data()));

        TH1D *RhoSparseToNoSub = (TH1D*)plots[0][0]->Clone(Form("RhoSparseToNoSub_%s",triggers.at(t).Data()));
        RhoSparseToNoSub->Divide(plots[2][t], plots[0][t]);
        RhoSparseToNoSub->Sumw2();
        RhoSparseToNoSub->SetMarkerStyle(4);
        RhoSparseToNoSub->SetLineColor(kBlack);
        RhoSparseToNoSub->SetMarkerColor(kBlack);
        RhoSparseToNoSub->SetMarkerSize(2.5);

        dummy->Draw("axis");
        l1->Draw("same");
        l2->Draw("same");
        l3->Draw("same");
        RhoSparseToNoSub->Draw("p,e1,same");

        drawLatexAdd("#sqrt{#it{s}_{NN}} = 8.16 TeV",0.11,0.3, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.11,0.26, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Trigger: %s",triggers.at(t).Data()),0.11,0.22, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("Statistical Errors Only",0.11,0.18, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("Rho Sparse / No Subtraction",0.11,0.14, textSize,kFALSE, kFALSE, false);

        c->SaveAs(Form("%s/RhoSparseToNoSub_R0%i_%s.%s",output.Data(),radius,triggers.at(t).Data(),fileType.Data()));

        TH1D *PerpConeToRhoSparse = (TH1D*)plots[0][0]->Clone(Form("PerpConeToRhoSparse_%s",triggers.at(t).Data()));
        PerpConeToRhoSparse->Divide(plots[1][t], plots[2][t]);
        PerpConeToRhoSparse->Sumw2();
        PerpConeToRhoSparse->SetMarkerStyle(4);
        PerpConeToRhoSparse->SetLineColor(kBlack);
        PerpConeToRhoSparse->SetMarkerColor(kBlack);
        PerpConeToRhoSparse->SetMarkerSize(2.5);

        dummy->Draw("axis");
        l1->Draw("same");
        l2->Draw("same");
        l3->Draw("same");
        PerpConeToRhoSparse->Draw("p,e1,same");

        drawLatexAdd("#sqrt{#it{s}_{NN}} = 8.16 TeV",0.11,0.3, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, #it{R} = 0.%i",radius),0.11,0.26, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Trigger: %s",triggers.at(t).Data()),0.11,0.22, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("Statistical Errors Only",0.11,0.18, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("Perpendicular Cones / Rho Sparse",0.11,0.14, textSize,kFALSE, kFALSE, false);

        c->SaveAs(Form("%s/PerpConeToRhoSparse_R0%i_%s.%s",output.Data(),radius,triggers.at(t).Data(),fileType.Data()));
    }

}
