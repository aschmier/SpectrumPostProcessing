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

void plotKinEff(TString file, TString output, TString fileType, TString system)
{
    // Define variables
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 5;

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    vector<TH1D*> vecEffKine;

    // Open unfolding results file
    TFile *f = TFile::Open(file);
    if(!f || f->IsZombie()) return;

    // Print file name
    cout << "File: " << file << endl;
    cout << f->GetName() << endl;

    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *topdir   = (TDirectory*)f->Get(Form("R0%i",radius));
        if(!topdir) cout << "Directory R0" << radius << " not found" << endl;
        TDirectory *response = (TDirectory*)topdir->Get("response");
        TH1D *effKine     = (TH1D*)response->Get(Form("effKine_R0%i", radius));
        //effKine->Scale(1.,"width");
        vecEffKine.push_back(effKine);
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.11;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogx();

    TLegend *legend =  GetAndSetLegend2(0.7,(0.3-(maxradius-minradius+1)*textSize),0.85,0.3,textSize,2);

    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==minradius){
            vecEffKine.at(radius-minradius)->GetXaxis()->SetRangeUser(5,380);
            //vecEffKine.at(radius-minradius)->GetYaxis()->SetRangeUser(1e-11,2e-4);
            SetStyleHistoTH1ForGraphs(vecEffKine.at(radius-minradius),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.2);
        }
        vecEffKine.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecEffKine.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecEffKine.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        legend->AddEntry(vecEffKine.at(radius-minradius), Form("R=0.%i", radius), "p");

        vecEffKine.at(radius-minradius)->Draw(radius==minradius? "p,e" : "p,e,same");
    }

    TLine *l80 = new TLine (5,0.8,600,0.8);
    l80->SetLineColor(14);
    l80->SetLineWidth(3);
    l80->SetLineStyle(7);
    l80->Draw("same");

    TLine *l100 = new TLine (5,1,600,1);
    l100->SetLineColor(14);
    l100->SetLineWidth(3);
    l100->SetLineStyle(7);
    l100->Draw("same");

    TLine *l20 = new TLine (20,0,20,1.05);
    l20->SetLineColor(14);
    l20->SetLineWidth(3);
    l20->SetLineStyle(7);
    l20->Draw("same");

    TLine *l320 = new TLine (320,0,320,1.05);
    l320->SetLineColor(14);
    l320->SetLineWidth(3);
    l320->SetLineStyle(7);
    l320->Draw("same");

    legend->Draw();
    if(system=="pp") drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.84,0.35, 0.03,kFALSE, kFALSE, kTRUE);
    if(system=="pPb") drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV",0.84,0.35, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Full Jets",0.84,0.31, 0.03,kFALSE, kFALSE, kTRUE);

    // Create output directory if it doesn't already exist
    system(Form("mkdir -p %s/KinematicEfficiency",output.Data()));
    canvas->SaveAs(Form("%s/KinematicEfficiency/EffKine.%s",output.Data(),fileType.Data()));
}
