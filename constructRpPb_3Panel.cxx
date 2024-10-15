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

void constructRpPb_3Panel(TString ppRootFile, TString pPbRootFile, TString scaleFactorRootFile, TString sysRootFile, TString output, TString fileType, int minradius = 2, int maxradius = 4)
{
    Double_t textSize     = 0.03;

    int scale[5] = {1,3,10,30,100};
    int regnumBayes = 6;
    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.1,1.,1.5,1.2,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};
    double minPt = 0.01;
    double maxPt = 259.;

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

    TLegend *legendErrorKey;

    Double_t textsizeLabels = 0;
    Double_t textsizeFac    = 0;
    Double_t textSizeLabelsPixel = 750*0.05;

    // Number of panels x+1
    Double_t arrayBoundariesX[4];
    // Number of panels y+1
    Double_t arrayBoundariesY[2];
    // Stays constant at 3
    Double_t relativeMarginsX[3];
    // Stays constant at 3
    Double_t relativeMarginsY[3];

    // [canvas size x], [canvas size y], [number of panels x], [number of panels y], [left margin], [right margin], [bottom margin], [top margin], [editable arrays (don't change)]...
    ReturnCorrectValuesForCanvasScaling(2250,750, 3, 1,0.043, 0.003, 0.01,0.111,arrayBoundariesX,arrayBoundariesY,relativeMarginsX,relativeMarginsY);
    Double_t margin = relativeMarginsX[0]*2.7*1350;

    // 0, 0, canvas size x, canvas size y
    TCanvas *canvas = new TCanvas("canvasSim","",0,0,2250,750);;
    TPad    *ratioPad[3];
    TH2F    *dummyHist;

    gStyle->SetOptStat(0);

    TLegend *legendErrorKeyRatioCombined;

    // canvas, left margin, right margin, bottom margin, top margin (this is for the base canvas underneath the pads)
    DrawGammaCanvasSettings( canvas,  0.05, 0.01, 0.01,0.095);
    canvas->cd();

    // 
    ratioPad[0] = new TPad("padR02", "", arrayBoundariesX[0], arrayBoundariesY[1], arrayBoundariesX[1], arrayBoundariesY[0],-1, -1, -2);
    ratioPad[1] = new TPad("padR03", "", arrayBoundariesX[1], arrayBoundariesY[1], arrayBoundariesX[2], arrayBoundariesY[0],-1, -1, -2);
    ratioPad[2] = new TPad("padR04", "", arrayBoundariesX[2], arrayBoundariesY[1], arrayBoundariesX[3], arrayBoundariesY[0],-1, -1, -2);

    DrawGammaPadSettings( ratioPad[0], relativeMarginsX[0], relativeMarginsX[1], relativeMarginsY[0], relativeMarginsY[2]);
    DrawGammaPadSettings( ratioPad[1], relativeMarginsX[1], relativeMarginsX[1], relativeMarginsY[0], relativeMarginsY[2]);
    DrawGammaPadSettings( ratioPad[2], relativeMarginsX[1], relativeMarginsX[2], relativeMarginsY[0], relativeMarginsY[2]);

    ratioPad[0]->Draw();
    ratioPad[1]->Draw();
    ratioPad[2]->Draw();


    if (ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) < ratioPad[0]->YtoPixel(ratioPad[0]->GetY1()) ){
      	textsizeLabels              = (Double_t)textSizeLabelsPixel/ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) ;
      	textsizeFac                 = (Double_t)1./ratioPad[0]->XtoPixel(ratioPad[0]->GetX2()) ;
    }else{
      	textsizeLabels              = (Double_t)textSizeLabelsPixel/ratioPad[0]->YtoPixel(ratioPad[0]->GetY1());
      	textsizeFac                 = (Double_t)1./ratioPad[0]->YtoPixel(ratioPad[0]->GetY1());
    }

    dummyHist = new TH2F("dummyHist","dummyHist", 1000, minPt,maxPt, 1000., 0.2,1.65);
    dummyHist->GetYaxis()->SetRangeUser(0.,1.6);
    SetStyleHistoTH2ForGraphs(dummyHist, "#it{p}_{T} (GeV/#it{c})", "#it{R}_{pPb}", 0.85*textsizeLabels, 0.95*textsizeLabels, 0.85*textsizeLabels, 0.95*textsizeLabels, 1, 1.1,510,505);

    ///////////////
    // Plot RpPb //
    ///////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        ratioPad[radius-minradius]->cd();
        dummyHist->DrawCopy();
        DrawGammaLines(minPt,maxPt,1.,1.,8.,16,9);

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
        if(radius == 4) RpA->GetXaxis()->SetRangeUser(20,120);
        else RpA->GetXaxis()->SetRangeUser(20,240);
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

        RpA->SetMarkerColor(colorpp);
        RpA->SetLineColor(colorpp);

        RpA->SetMarkerStyle(stylesfilled[radius-minradius]);
        RpA->SetMarkerSize(sizesfilled[radius-minradius]);
        RpA->SetLineWidth(3);
        RpA->SetLineStyle(1);

        RpASys->SetFillColorAlpha(colorpp,alphasval);
        RpASys->SetLineColor(colorpp);
        RpASys->SetFillStyle(1);

        DrawGammaLines(0.,260.,1.,1.,8.,16,9);
        RpA->Draw("p,e1,same");
        RpASys->Draw("e2,same");

        if(radius==minradius){
            legendErrorKey = GetAndSetLegend2(0.15,0.14,0.35,0.14+(2)*(0.85*textsizeLabels),0.85*textsizeLabels);
            legendErrorKey->AddEntry(RpASys, "Systematic Uncertainty", "f");
            legendErrorKey->AddEntry(RpA, "Statistical Uncertainty", "e");
            legendErrorKey->Draw("same");
        }

        drawLatexAdd(Form("#it{R} = 0.%i", radius),0.95,0.91, 0.85*textsizeLabels,kFALSE, kFALSE, true);

        if(radius==maxradius){
            drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.95,0.26, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.95,0.21, 0.85*textsizeLabels,kFALSE, kFALSE, true);
            drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.95,0.16, 0.85*textsizeLabels,kFALSE, kFALSE, true);
        }

        dummyHist->Draw("axis,same");
    }

    canvas->SaveAs(Form("%s/RpPb.%s",output.Data(),fileType.Data()));

}
