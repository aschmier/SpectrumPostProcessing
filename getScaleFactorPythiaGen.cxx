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

void getScaleFactorPythiaGen(TString ppFile, TString pPbFile, TString rootfiledir, TString outputdir, TString filetype, int minradius, int maxradius){
    gSystem->Exec("mkdir -p "+outputdir);
    gSystem->Exec("mkdir -p "+rootfiledir);

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

    double textSize = 0.03;

    vector<double> partlevelbin = getJetPtBinningNonLinTrue8TeVCourse();

    TFile *fpp = TFile::Open(ppFile);
    if(!fpp || fpp->IsZombie()){
        cout << "pp file not found!" << endl;
        return;
    }

    TFile *fpPb = TFile::Open(pPbFile);
    if(!fpPb || fpPb->IsZombie()){
        cout << "pPb file not found!" << endl;
        return;
    }

    TFile *fOutput = new TFile(Form("%s/scaleFactorPythia.root",rootfiledir.Data()),"RECREATE");

    // Create dummy histogram
    TH1D *dummy = new TH1D("dummy", "", 260, 0, 260);
    dummy->GetYaxis()->SetRangeUser(0.8,1.2);
    SetStyleHistoTH1ForGraphs(dummy,"","#it{p}_{T} (GeV/#it{c})","Pythia pp 8.16 TeV / Pythia pp 8 TeV",textSize,0.04,textSize,0.04,1,1.2);

    for(int radius = minradius; radius <= maxradius; radius++){
        fOutput->mkdir(Form("R0%i",radius));
        fOutput->cd(Form("R0%i",radius));

        TString listname = Form("PartLevelJetResultsFullJetR0%i_nooutlier", radius);

        TList *listpp     = (TList*)fpp->Get(listname.Data());
        TH1D *specppfine  = (TH1D*)listpp->FindObject("hJetPt");
        TH1D *specpp      = (TH1D*)specppfine->Rebin(partlevelbin.size()-1, Form("specpp_%i",radius), partlevelbin.data());
        specpp->Scale(1.,"width");

        TList *listpPb    = (TList*)fpPb->Get(listname.Data());
        TH1D *specpPbfine = (TH1D*)listpPb->FindObject("hJetPt");
        TH1D *specpPb     = (TH1D*)specpPbfine->Rebin(partlevelbin.size()-1, Form("specpPb_%i",radius), partlevelbin.data());
        specpPb->Scale(1.,"width");

        TH1D *ratio = (TH1D*)specpPb->Clone(Form("ratio_R0%i",radius));
        ratio->Divide(specpPb, specpp);
        ratio->Sumw2();

        // Draw
        dummy->Draw("axis");

        ratio->SetMarkerStyle(styles[radius-2]);
        ratio->SetMarkerColor(colors[radius-2]);
        ratio->SetLineColor(colors[radius-2]);
        ratio->Draw("p,e,same");

        TLine *l100 = new TLine (0,1,260,1);
        l100->SetLineColor(14);
        l100->SetLineWidth(3);
        l100->SetLineStyle(7);
        l100->Draw("same");

        drawLatexAdd(Form("R = 0.%i", radius),0.63,0.28, 0.05, false, false, false);

        ratio->Write(Form("PythiaRatio_R0%i",radius));


        c->SaveAs(outputdir+Form("/PythiaRatio_R0%i.%s",radius,filetype.Data()));

    }

    fOutput->Close();
    fpp->Close();
    fpPb->Close();

    return;
}