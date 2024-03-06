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

void compareRequireL0(TString defaultfile, TString requirel0file, TString output, TString fileType, int minradius = 2, int maxradius = 4)
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
    TFile *fdef = TFile::Open(defaultfile);
    if(!fdef || fdef->IsZombie()){
        cout << "default file not found!" << endl;
        return;
    }

    TFile *fL0 = TFile::Open(requirel0file);
    if(!fL0 || fL0->IsZombie()){
        cout << "L0 file not found!" << endl;
        cout << requirel0file << endl;
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
    SetStyleHistoTH1ForGraphs(dummy,"","#it{p}_{T} (GeV/#it{c})","(Requre L0 for L1) / (No Requirement)",textSize,0.035,textSize,0.035,1,1.2);

    //TLegend *legendErrorKey =  GetAndSetLegend2(0.19,0.11,0.39,0.11+(2)*textSize,textSize);
    //legendErrorKey->AddEntry(vecSpectrumSyspp.at(0), "Systematic Uncertainty", "f");
    //legendErrorKey->AddEntry(vecSpectrumpp.at(0), "Statistical Uncertainty", "e");

    //TLegend *legendErrorKeyRatio =  GetAndSetLegend2(0.6,0.11,0.8,0.11+(2)*textSize,textSize);
    //legendErrorKeyRatio->AddEntry(vecRatioSyspp.at(0), "Systematic Uncertainty", "f");
    //legendErrorKeyRatio->AddEntry(vecRatiopp.at(0), "Statistical Uncertainty", "e");

    for(int radius = minradius; radius <= maxradius; radius++){
        dummy->GetYaxis()->SetRangeUser(0.9,1.05);
        //legendRatio       = GetAndSetLegend2(0.63,0.94-(2)*textSize,0.83,0.94,textSize);

        TDirectory *dRadOrigDef= (TDirectory*)fdef->Get(Form("R0%i",radius));
        TDirectory *dRegOrigDef   = (TDirectory*)dRadOrigDef->Get(Form("reg%i",regnumBayes));
        TH1D *normUnfoldedOrigDef = (TH1D*)dRegOrigDef->Get(Form("normalized_reg%i",regnumBayes));

        TDirectory *dRadOrigL0 = (TDirectory*)fL0->Get(Form("R0%i",radius));
        TDirectory *dRegOrigL0    = (TDirectory*)dRadOrigL0->Get(Form("reg%i",regnumBayes));
        TH1D *normUnfoldedOrigL0 = (TH1D*)dRegOrigL0->Get(Form("normalized_reg%i",regnumBayes));

        TH1D *L0test = (TH1D*)normUnfoldedOrigL0->Clone(Form("L0test_R0%i", radius));
        L0test->Divide(normUnfoldedOrigL0,normUnfoldedOrigDef,1,1,"b");
        L0test->GetXaxis()->SetRangeUser(20,240);

        //legendRatio->AddEntry(L0test, Form("R = 0.%i", radius), "p");

        L0test->SetMarkerColor(colorpp);
        L0test->SetLineColor(colorpp);

        L0test->SetMarkerStyle(stylesfilled[radius-minradius]);
        L0test->SetMarkerSize(sizesfilled[radius-minradius]);
        //L0test->SetMarkerColor(colors[radius-minradius]);
        //L0test->SetLineColor(colors[radius-minradius]);
        L0test->SetLineWidth(3);
        L0test->SetLineStyle(1);

        dummy->Draw("axis");
        DrawGammaLines(0.,260.,1.,1.,8.,16,9);
        L0test->Draw("p,e1,same");

        //legendRatio->Draw("same");
        //legendErrorKeyRatio->Draw("same");

        //drawLatexAdd("ALICE Preliminary",0.17,0.92, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.14,0.24, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.%i", radius),0.14,0.2, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.14,0.16, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.14,0.12, textSize,kFALSE, kFALSE, false);

        cRatio->SaveAs(Form("%s/RequireL0/RequireL0_R0%i.%s",output.Data(),radius,fileType.Data()));
    }

}
