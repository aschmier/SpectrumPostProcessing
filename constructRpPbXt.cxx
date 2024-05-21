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

void constructRpPbXt(TString ppRootFile, TString pPbRootFile, TString sysRootFile, TString output, TString fileType, int minradius = 2, int maxradius = 4)
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

        TDirectory *rDirpp = (TDirectory*)fpp->Get(Form("R0%i",radius));
        TH1D *normUnfoldedOrigpp = (TH1D*)rDirpp->Get("scaledSpectrumOutHist");

        TDirectory *dRadOrigpPb = (TDirectory*)fpPb->Get(Form("R0%i",radius));
        TDirectory *dRegOrigpPb    = (TDirectory*)dRadOrigpPb->Get(Form("reg%i",regnumBayes));
        TH1D *normUnfoldedOrigpPb = (TH1D*)dRegOrigpPb->Get(Form("normalized_reg%i",regnumBayes));

        TString sysfile = Form("%s/systematics_R0%i.root", sysRootFile.Data(), radius);
        TFile *fSys = TFile::Open(sysfile);
        if(!fSys || fSys->IsZombie()){
            cout << "pp systematics file not found! --> R=0." << radius << endl;
            return;
        }
        TH1D *systematics = (TH1D*)fSys->Get("hTotal");
        systematics->SetName(Form("systematics_R0%i",radius));

        cout << "Radius: " << radius << endl;
        cout << "pp:" << endl;
        for(int bin = 1; bin <= normUnfoldedOrigpp->GetNbinsX(); bin++){
            cout << "Bin: " << bin << endl;
            if(normUnfoldedOrigpp->GetBinCenter(bin) > 20. && normUnfoldedOrigpp->GetBinCenter(bin) < 280.) cout << "Bin Center: " << normUnfoldedOrigpp->GetBinCenter(bin) << endl;
            if(normUnfoldedOrigpp->GetBinCenter(bin) > 20. && normUnfoldedOrigpp->GetBinCenter(bin) < 280.) cout << "Error: " << normUnfoldedOrigpp->GetBinError(bin) << endl;
        }
        cout << endl;

        cout << "pPb:" << endl;
        for(int bin = 1; bin <= normUnfoldedOrigpPb->GetNbinsX(); bin++){
            cout << "Bin: " << bin << endl;
            if(normUnfoldedOrigpPb->GetBinCenter(bin) > 20. && normUnfoldedOrigpPb->GetBinCenter(bin) < 280.) cout << "Bin Center: " << normUnfoldedOrigpPb->GetBinCenter(bin) << endl;
            if(normUnfoldedOrigpPb->GetBinCenter(bin) > 20. && normUnfoldedOrigpPb->GetBinCenter(bin) < 280.) cout << "Error: " << normUnfoldedOrigpPb->GetBinError(bin) << endl;
        }
        cout << endl;

        cout << "Systematics:" << endl;
        for(int bin = 1; bin <= systematics->GetNbinsX(); bin++){
            cout << "Bin: " << bin << endl;
            if(systematics->GetBinCenter(bin) > 20. && systematics->GetBinCenter(bin) < 280.) cout << "Bin Center: " << systematics->GetBinCenter(bin) << endl;
            if(systematics->GetBinCenter(bin) > 20. && systematics->GetBinCenter(bin) < 280.) cout << "Error: " << systematics->GetBinError(bin) << endl;
        }
        cout << endl;
/*
        // Create TGraphErrors and remove unused points for spectra
        TGraphErrors *gSpectrum     = new TGraphErrors(vecSpectrum.at(radius-minradius));
        TGraphErrors *gSpectrumSys  = new TGraphErrors(vecSpectrumSys.at(radius-minradius));

        for(int point = 0; point < gSpectrum->GetN(); point++){
            double staterror = gSpectrum->GetErrorY(point);
            gSpectrum->SetPointError(point,0,staterror);
            double syserror = gSpectrumSys->GetErrorY(point);
            gSpectrumSys->SetPointError(point,gSpectrumSys->GetErrorX(point),syserror);
        }
*/
        TH1D *RpA = (TH1D*)normUnfoldedOrigpPb->Clone("RpA");
        RpA->Divide(normUnfoldedOrigpPb,normUnfoldedOrigpp);
        RpA->Scale(1/nucfac);
        RpA->GetXaxis()->SetRangeUser(20,240);
        TH1D *RpASys = (TH1D*)RpA->Clone(Form("RpASys_R0%i", radius));

        cout << "RpA:" << endl;
        for(int bin = 1; bin <= RpASys->GetNbinsX(); bin++){
            cout << "Bin: " << bin << endl;
            if(RpA->GetBinCenter(bin) > 20. && RpA->GetBinCenter(bin) < 280.) cout << "Bin Center: " << RpA->GetBinCenter(bin) << endl;
            if(RpA->GetBinCenter(bin) > 20. && RpA->GetBinCenter(bin) < 280.) cout << "Error: " << RpA->GetBinError(bin) << endl;
            double sysError          = systematics->GetBinContent(bin)/100;
            Double_t scaledError     = sysError*RpASys->GetBinContent(bin);
            RpASys->SetBinError(bin, scaledError);
        }
cout << endl;
        //legendRatio->AddEntry(RpA, Form("R = 0.%i", radius), "p");

        RpA->SetMarkerColor(colorpp);
        RpA->SetLineColor(colorpp);

        RpA->SetMarkerStyle(stylesfilled[radius-minradius]);
        RpA->SetMarkerSize(sizesfilled[radius-minradius]);
        //RpA->SetMarkerColor(colors[radius-minradius]);
        //RpA->SetLineColor(colors[radius-minradius]);
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

        cRatio->SaveAs(Form("%s/RpPb/RpPbXt_R0%i.%s",output.Data(),radius,fileType.Data()));
    }

}
