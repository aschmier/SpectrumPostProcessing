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

void compareScaledRhoSpectra_unfolded(TString nfull, TString ncharged, TString nhigh, TString nlow, TString output, TString filetype, TString system){
    double textSize = 0.04;
    int reg = 6;

    vector<double> binning = getJetPtBinningNonLinSmear8TeV();

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    int minradius = 2;
    int maxradius = 4;
    if(system=="pp") maxradius = 5;
    //else if(system=="pPb") maxradius = 4;
    //else{
    //    cout << "Invalid system" << endl;
    //    return;
    //}

    vector<TString> triggers;
    if(system=="pp") triggers = {"INT7", "EMC7", "EJE"};
    else if(system=="pPb") triggers = {"INT7", "EJ2", "EJ1"};
    else{
        cout << "Invalid system" << endl;
        return;
    }

    TFile *fFull = TFile::Open(nfull.Data(), "READ");
    TFile *fCharged = TFile::Open(ncharged.Data(), "READ");
    TFile *fHigh = TFile::Open(nhigh.Data(), "READ");
    TFile *fLow = TFile::Open(nlow.Data(), "READ");

    if(!fFull || !fCharged || !fHigh || !fLow){
        cout << "Invalid file" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        TString histname = Form("R0%i/reg%i/normalized_reg%i", radius, reg, reg);
        
        TH1D *spectrumFull = (TH1D*)fFull->Get(histname);
        TH1D *spectrumCharged = (TH1D*)fCharged->Get(histname);
        TH1D *spectrumHigh = (TH1D*)fHigh->Get(histname);
        TH1D *spectrumLow = (TH1D*)fLow->Get(histname);

        TH1D *rFullCharged = (TH1D*)spectrumFull->Clone(Form("rFullCharged_R0%i", radius));
        rFullCharged->Divide(spectrumCharged);
        rFullCharged->SetMarkerStyle(styles[0]);
        rFullCharged->SetMarkerColor(colors[0]);
        rFullCharged->SetLineColor(colors[0]);
        rFullCharged->SetMarkerSize(1.5);

        TH1D *rHighCharged = (TH1D*)spectrumHigh->Clone(Form("rHighCharged_R0%i", radius));
        rHighCharged->Divide(spectrumCharged);
        rHighCharged->SetMarkerStyle(styles[1]);
        rHighCharged->SetMarkerColor(colors[1]);
        rHighCharged->SetLineColor(colors[1]);
        rHighCharged->SetMarkerSize(1.5);

        TH1D *rLowCharged = (TH1D*)spectrumLow->Clone(Form("rLowCharged_R0%i", radius));
        rLowCharged->Divide(spectrumCharged);
        rLowCharged->SetMarkerStyle(styles[2]);
        rLowCharged->SetMarkerColor(colors[2]);
        rLowCharged->SetLineColor(colors[2]);
        rLowCharged->SetMarkerSize(2);

        TCanvas *c   = new TCanvas(Form("c_%i", radius), "", 1200, 800);
        DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
        gStyle->SetOptStat(0);

        TH1D *h = new TH1D(Form("h_%i", radius),"",260,0,260);
        SetStyleHistoTH1ForGraphs(h,"","p_{T} (GeV/c)","#sigma(#rho^{variation})/#sigma(#rho^{default})",0.03,0.04,0.03,0.04,1,1.1);
        h->GetYaxis()->SetRangeUser(0.8,1.1);
        h->Draw("axis");

        // for(int bin = 1; bin < rFullCharged->GetNbinsX()+1; bin++){
        //     rFullCharged->SetBinError(bin,0);
        //     rHighCharged->SetBinError(bin,0);
        //     rLowCharged->SetBinError(bin,0);
        // }

        rFullCharged->Draw("p,e1,same");
        rHighCharged->Draw("p,e1,same");
        rLowCharged->Draw("p,e1,same");
        
        TLegend *legend =  GetAndSetLegend2(0.6,(0.93-(5)*textSize),0.8,0.93,textSize,1);
        legend->AddEntry(rFullCharged, "(#rho^{fu}) / (1.305 x #rho^{ch})", "p");
        legend->AddEntry(rHighCharged, "(1.395 x #rho^{ch}) / (1.305 x #rho^{ch})", "p");
        legend->AddEntry(rLowCharged, "(1.275 x #rho^{ch}) / (1.305 x #rho^{ch})", "p");
        
        legend->Draw("same");

        DrawGammaLines(0.,260.,1.,1.,5.,16,9);

        drawLatexAdd("Ratio of Unfolded Jet Spectra",0.63,0.29, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("#it{R} = 0.%i", radius),0.63,0.24, textSize,kFALSE, kFALSE, false);
        if(system == "pp") drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.63,0.19, textSize,kFALSE, kFALSE, false);
        if(system == "pPb") drawLatexAdd("p-Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.63,0.19, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.63,0.14, textSize,kFALSE, kFALSE, false);

        c->SaveAs(Form("%s/scaledChRhoComp_R0%i.%s", output.Data(), radius, filetype.Data()));
    }
}