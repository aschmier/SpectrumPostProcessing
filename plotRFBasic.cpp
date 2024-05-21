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
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D_rf.C"
#include "/home/austin/alice/unfoldingResults/makeRejectionFactor.cpp"

void plotRFBasic(TString in, TString out)
{
    // global variables
    Double_t textSize = 0.04;
    TString fileType  = "pdf";
    TString jetType   = "Full";
    int minradius     = 2;
    int maxradius     = 6;

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    // open file
    TFile *f = TFile::Open(in);
    if(!f || f->IsZombie()) return;

    // Set up canvas for plotting
    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

    for(int r=minradius; r<=maxradius; r++){
        TDirectory *base     = (TDirectory*)f->Get(Form("R0%i",r));
        TDirectory *rawlevel = (TDirectory*)base->Get("rawlevel");
        TH1D *rfactors       = (TH1D*)rawlevel->Get("rfactors");
        rfactors->GetXaxis()->SetBinLabel(1,"EMC7/INT7");
        rfactors->GetXaxis()->SetBinLabel(2,"EJE/EMC7");
        SetStyleHistoTH1ForGraphs(rfactors,"","","Rejection Factor",0.05,0.04,0.03,0.04,1,0.8);
        rfactors->Draw();
        drawLatexAdd(Form("pp #it{#sqrt{s_{NN}}} = 8 TeV; Full Jets, R = 0.%i",r),0.14,0.9, textSize,false, false, false);
        drawLatexAdd(Form("RF EMC7/INT7 = %1.2f +/- %1.2f",rfactors->GetBinContent(1),rfactors->GetBinError(1)),0.14,0.83, textSize,false, false, false);
        drawLatexAdd(Form("RF EJE/EMC7  = %1.2f +/- %1.2f",rfactors->GetBinContent(2),rfactors->GetBinError(2)),0.14,0.78, textSize,false, false, false);
        c->SaveAs(Form("%s/RFactorsBasic_R0%i.%s",out.Data(),r,fileType.Data()));
    }
}
