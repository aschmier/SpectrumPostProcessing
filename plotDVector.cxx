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

void plotDVector(TString file, TString output, TString fileType, TString system)
{
    // Define variables
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 5;
    int regiter   = 7;
    TString triggers[3] = {"Min. Bias (INT7)", "EMCal-L0 (EMC7)", "EMCal-L1 (EJE)"};

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    vector<TH1D*> vecDVec;

    // Open unfolding results file
    TFile *f = TFile::Open(file);
    if(!f || f->IsZombie()) return;

    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *topdir = (TDirectory*)f->Get(Form("R0%i",radius));
        if(!topdir) return;
        TDirectory *reg    = (TDirectory*)topdir->Get(Form("reg%i",regiter));
        TH1D *dvector      = (TH1D*)reg->Get(Form("dvector_Reg%i",regiter));
        vecDVec.push_back(dvector);
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.11;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    for(int radius = minradius; radius <= maxradius; radius++){
        //vecMB.at(radius-minradius)->GetXaxis()->SetRangeUser(0,280);
        //vecMB.at(radius-minradius)->GetYaxis()->SetRangeUser(1e-11,2e-4);
        SetStyleHistoTH1ForGraphs(vecDVec.at(radius-minradius),"","Regularization Parameter","D-Vector",0.03,0.04,0.03,0.04,1,1.2);
        vecDVec.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecDVec.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecDVec.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        vecDVec.at(radius-minradius)->Draw("b");

        if(system == "pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
        else drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("Full Jets, #it{R} = 0.%i", radius),0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd("D-Vector Iterations Comparison",0.95,0.79, 0.03,kFALSE, kFALSE, kTRUE);
        canvas->SaveAs(Form("%s/DVector/DVector_R0%i.%s",output.Data(),radius,fileType.Data()));
    }

    TLegend *leg = new TLegend(0.6,0.6,0.9,0.9);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextSize(0.03);
    leg->AddEntry(vecDVec.at(0),"R = 0.2","p");
    leg->AddEntry(vecDVec.at(1),"R = 0.3","p");
    leg->AddEntry(vecDVec.at(2),"R = 0.4","p");
    leg->AddEntry(vecDVec.at(3),"R = 0.5","p");
    leg->AddEntry(vecDVec.at(4),"R = 0.6","p");
    leg->Draw();
}
