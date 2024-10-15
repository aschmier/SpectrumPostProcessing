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

void constructRpPb(TString ppRootFile, TString pPbRootFile, TString scaleFactorRootFile, TString sysRootFile, TString output, TString fileType, int minradius = 2, int maxradius = 4)
{
    Double_t textSize     = 0.03;

    int scale[5] = {1,3,10,30,100};
    int regnumBayes = 6;
    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.1,1.,1.5,1.2,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    double alphasval = 0.25;
    double nucfac = 208.;

    Color_t colorpp  = kGreen+2;
    Color_t colorpPb = kOrange+1;

    // Open root files
    TFile *fpp = TFile::Open(ppRootFile);
    if(!fpp || fpp->IsZombie()){
        cout << "pp file not found!" << endl;
        return;
    }

    TFile *fpPb = TFile::Open(pPbRootFile);
    if(!fpPb || fpPb->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    TFile *fscale = TFile::Open(scaleFactorRootFile);
    if(!fscale || fscale->IsZombie()){
        cout << "Scale factor file not found!" << endl;
        return;
    }

    // Set up canvases and legends for plotting
    TCanvas *cRatio   = new TCanvas("c2", "", 800, 800);
    DrawPaperCanvasSettings(cRatio,0.09,0.02,0.02,0.08);

    gStyle->SetOptStat(0);

    //TLegend *legendRatio;

    // Set up dummy histo for plotting
    TH1D *dummy = new TH1D("dummy","",260,0,260);
    dummy->GetXaxis()->SetRangeUser(0,340);
    SetStyleHistoTH1ForGraphs(dummy,"","#it{p}_{T} (GeV/#it{c})","#it{R}_{pPb}",textSize,0.035,textSize,0.035,1,1.2);

    //TLegend *legendErrorKey =  GetAndSetLegend2(0.19,0.11,0.39,0.11+(2)*textSize,textSize);
    //legendErrorKey->AddEntry(vecSpectrumSyspp.at(0), "Systematic Uncertainty", "f");
    //legendErrorKey->AddEntry(vecSpectrumpp.at(0), "Statistical Uncertainty", "e");

    //TLegend *legendErrorKeyRatio =  GetAndSetLegend2(0.6,0.11,0.8,0.11+(2)*textSize,textSize);
    //legendErrorKeyRatio->AddEntry(vecRatioSyspp.at(0), "Systematic Uncertainty", "f");
    //legendErrorKeyRatio->AddEntry(vecRatiopp.at(0), "Statistical Uncertainty", "e");

    ///////////////
    // Plot RpPb //
    ///////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==5) dummy->GetYaxis()->SetRangeUser(0,1.9);
        else dummy->GetYaxis()->SetRangeUser(0,1.6);
        //legendRatio       = GetAndSetLegend2(0.63,0.94-(2)*textSize,0.83,0.94,textSize);

        TDirectory *scalefacdir = (TDirectory*)fscale->Get(Form("R0%i",radius));
        TH1D *scalefac = (TH1D*)scalefacdir->Get(Form("PythiaRatio_R0%i",radius));

        TDirectory *dRadOrigpp = (TDirectory*)fpp->Get(Form("R0%i",radius));
        TDirectory *dRegOrigpp    = (TDirectory*)dRadOrigpp->Get(Form("reg%i",regnumBayes));
        TH1D *normUnfoldedOrigpp = (TH1D*)dRegOrigpp->Get(Form("normalized_reg%i",regnumBayes));
        normUnfoldedOrigpp->Multiply(normUnfoldedOrigpp,scalefac);

        TDirectory *dRadOrigpPb = (TDirectory*)fpPb->Get(Form("R0%i",radius));
        TDirectory *dRegOrigpPb    = (TDirectory*)dRadOrigpPb->Get(Form("reg%i",regnumBayes));
        TH1D *normUnfoldedOrigpPb = (TH1D*)dRegOrigpPb->Get(Form("normalized_reg%i",regnumBayes));

        TString sysfile = Form("%s/systematics_R0%i.root", sysRootFile.Data(), radius);
        TFile *fSys = TFile::Open(sysfile);
        if(!fSys || fSys->IsZombie()){
            cout << "pp systematics file not found! --> R=0." << radius << endl;
            return;
        }
        TDirectory *dRadSys = (TDirectory*)fSys->Get(Form("R0%i",radius));
        TH1D *systematics = (TH1D*)dRadSys->Get("hTotal");
        systematics->SetName(Form("systematics_R0%i",radius));

        
        TH1D *RpA = (TH1D*)normUnfoldedOrigpPb->Clone("RpA");
        RpA->Divide(normUnfoldedOrigpPb,normUnfoldedOrigpp);
        RpA->Sumw2();
        RpA->Scale(1/nucfac);
        //RpA->Divide(scalefac);
        if(radius == 4) RpA->GetXaxis()->SetRangeUser(20,120);
        else RpA->GetXaxis()->SetRangeUser(20,240);
        //else if(radius == 3) RpA->GetXaxis()->SetRangeUser(25,240);
        //else if(radius == 4) RpA->GetXaxis()->SetRangeUser(30,120);
        TH1D *RpASys = (TH1D*)RpA->Clone(Form("RpASys_R0%i", radius));

        for(int bin = 1; bin <= RpASys->GetNbinsX(); bin++){
            double sysError          = systematics->GetBinContent(bin)/100;
            Double_t scaledError     = sysError*RpASys->GetBinContent(bin);
            RpASys->SetBinError(bin, scaledError);
        }

        for(int bin = 1; bin <= RpA->GetNbinsX(); bin++){
            if(RpA->GetBinLowEdge(bin) < 20 || (RpA->GetBinLowEdge(bin) + RpA->GetBinWidth(bin)) > 240) continue;
            double val = RpA->GetBinContent(bin);
            double staterror = RpA->GetBinError(bin);
            double systerror = RpASys->GetBinError(bin);
            cout << "pT: " << RpA->GetBinCenter(bin) << ", nSigma from 1: " << abs((val-1.0))/sqrt(pow(staterror,2) + pow(systerror,2)) << endl;
        }

        //legendRatio->AddEntry(RpA, Form("R = 0.%i", radius), "p");

        RpA->SetMarkerColor(colorpp);
        RpA->SetLineColor(colorpp);

        RpA->SetMarkerStyle(stylesfilled[radius-minradius]);
        RpA->SetMarkerSize(sizesfilled[radius-minradius]);
        RpA->SetLineWidth(3);
        RpA->SetLineStyle(1);

        RpASys->SetFillColorAlpha(colorpp,alphasval);
        RpASys->SetLineColor(colorpp);
        RpASys->SetFillStyle(1);

        dummy->Draw("axis");
        DrawGammaLines(0.,260.,1.,1.,8.,16,9);
        RpA->Draw("p,e1,same");
        RpASys->Draw("e2,same");

        //legendRatio->Draw("same");
        //legendErrorKeyRatio->Draw("same");

        //drawLatexAdd("ALICE Preliminary",0.17,0.92, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.%i", radius),0.14,0.2, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.14,0.16, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.14,0.12, textSize,kFALSE, kFALSE, false);

        cRatio->SaveAs(Form("%s/RpPb/RpPb_R0%i.%s",output.Data(),radius,fileType.Data()));
    }

}
