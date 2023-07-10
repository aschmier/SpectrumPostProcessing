#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"

void plotDpT(TString filename, TString outputdir, TString fileType, TString suffix, int minradius, int maxradius)
{
    double textSize  = 0.03;
    TString jetType  = "Full";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    TString triggers[3] = {"INT7","EJ2","EJ1"};
    double nEvt[3];

    gSystem->Exec("mkdir -p "+outputdir);

    TFile *f = TFile::Open(filename.Data());
    if(!f || f->IsZombie()){
        cout << "File not found!" << endl;
        return;
    }


    TCanvas* canvas       = new TCanvas("canvas", "", 1000, 1000);
    double leftMargin   = 0.1;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.83,0.75-(3)*textSize,0.98,0.75,textSize);
    // loop over radii
    for(int radius = minradius; radius <= maxradius; radius++){
    for(int i = 0; i < 3; i++){
        TString dirname     = Form("JetSpectrum_%sJets_R0%i_%s_%s", jetType.Data(), radius, triggers[i].Data(), suffix.Data());
        TString listname     = Form("JetSpectrum_%sJets_R0%i_%s_%s", jetType.Data(), radius, triggers[i].Data(), suffix.Data());
        TDirectory *dir = (TDirectory*)f->Get(dirname.Data());
        TList *list     = (TList*)dir->Get(listname.Data());
        TH2D *hDpT2D    = (TH2D*)list->FindObject("hRhoVsDeltaPtRC");
        TH1D *hDpT      = (TH1D*)hDpT2D->ProjectionX(Form("hDpT_%s",triggers[i].Data()),1,100);
        hDpT->Scale(1./hDpT->Integral());
        SetStyleHistoTH1ForGraphs(hDpT,"","#delta#it{p}_{T}^{RC} (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);

        hDpT->SetMarkerStyle(styles[i]);
        hDpT->SetMarkerColor(colors[i]);
        hDpT->SetLineColor(colors[i]);
        hDpT->SetMarkerSize(1.5);

        legend->AddEntry(hDpT,triggers[i].Data(),"p");
        if(i==0) hDpT->Draw("p,e");
        else hDpT->Draw("p,e,same");
    }
    legend->Draw("same");
    // draw text
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.87, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.84, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);
    drawLatexAdd(Form("R = 0.%i", radius),0.93,0.78,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotDpT_R0%i.%s",outputdir.Data(),radius,fileType.Data()));
    }
}
