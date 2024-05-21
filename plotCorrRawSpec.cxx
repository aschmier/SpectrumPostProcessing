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

void plotCorrRawSpec(TString file, TString output, TString fileType, TString system, int minradius = 2, int maxradius = 6)
{
    // Define variables
    Double_t textSize     = 0.03;
    TString triggers[3];
    if(system=="pp"){
        triggers[0] = "Min. Bias (INT7)";
        triggers[1] = "EMCal-L0 (EMC7)";
        triggers[2] = "EMCal-L1 (EJE)";
    }
    if(system=="pPb"){
        triggers[0] = "Min. Bias (INT7)";
        triggers[1] = "EMCal-L1 (EJ2)";
        triggers[2] = "EMCal-L1 (EJ1)";
    }
    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizes[11] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    double xsec = 55.8;

        gSystem->Exec("mkdir -p "+output+"/CorrRawSpec");


    vector<TH1D*> vecMB;
    vector<TH1D*> vecEMC7;
    vector<TH1D*> vecEJE;
    vector<TH1D*> vecCombined;

    // Open unfolding results file
    TFile *f = TFile::Open(file);
    if(!f || f->IsZombie()) return;

    if(system=="pp"){   
        for(int radius = minradius; radius <= maxradius; radius++){
            TDirectory *topdir   = (TDirectory*)f->Get(Form("R0%i",radius));
            TDirectory *rawlevel = (TDirectory*)topdir->Get("rawlevel");
            TH1D *mbrebinned     = (TH1D*)rawlevel->Get("mbrebinned");
            TH1D *emc7rebinned   = (TH1D*)rawlevel->Get("emc7rebinned");
            TH1D *ejerebinned    = (TH1D*)rawlevel->Get("ejerebinned");
            TH1D *combined       = (TH1D*)rawlevel->Get(Form("hraw_R0%i",radius));
            mbrebinned->Scale(xsec,"width");
            emc7rebinned->Scale(xsec,"width");
            ejerebinned->Scale(xsec,"width");
            combined->Scale(1.,"width");
            vecMB.push_back(mbrebinned);
            vecEMC7.push_back(emc7rebinned);
            vecEJE.push_back(ejerebinned);
            vecCombined.push_back(combined);
        }
    }

    if(system=="pPb"){   
        for(int radius = minradius; radius <= maxradius; radius++){
            TDirectory *topdir   = (TDirectory*)f->Get(Form("R0%i",radius));
            TDirectory *rawlevel = (TDirectory*)topdir->Get("rawlevel");
            TH1D *mbrebinned     = (TH1D*)rawlevel->Get("mbrebinned");
            TH1D *emc7rebinned   = (TH1D*)rawlevel->Get("ej2rebinned");
            TH1D *ejerebinned    = (TH1D*)rawlevel->Get("ej1rebinned");
            TH1D *combined       = (TH1D*)rawlevel->Get(Form("hraw_R0%i",radius));
            mbrebinned->Scale(xsec,"width");
            emc7rebinned->Scale(xsec,"width");
            ejerebinned->Scale(xsec,"width");
            combined->Scale(1.,"width");
            vecMB.push_back(mbrebinned);
            vecEMC7.push_back(emc7rebinned);
            vecEJE.push_back(ejerebinned);
            vecCombined.push_back(combined);
        }
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,800,1000);  // gives the page size
    Double_t leftMargin         = 0.15;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.01;
    Double_t bottomMargin       = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.7,(0.88-(maxradius-minradius)*textSize),0.96,0.88,textSize,2);
    TLegend *legend2 =  GetAndSetLegend2(0.75,(0.78-(3)*textSize),0.93,0.78,textSize);


    TH1D *dummy = new TH1D("dummy","",260,0,260);
    dummy->GetXaxis()->SetRangeUser(0,260);
    if(system=="pp") dummy->GetYaxis()->SetRangeUser(1e-9,7e-4);
    if(system=="pPb") dummy->GetYaxis()->SetRangeUser(2e-5,8);

    SetStyleHistoTH1ForGraphs(dummy,"","#it{p}_{T}^{jet}","#frac{1}{#it{N}^{trig}} #frac{d#it{N}}{d#it{p}_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.5);
    dummy->Draw("axis");


    for(int radius = minradius; radius <= maxradius; radius++){
        vecMB.at(radius-minradius)->GetXaxis()->SetRangeUser(20,30);
        if(system == "pPb"){
            vecEMC7.at(radius-minradius)->GetXaxis()->SetRangeUser(30,50);
            if(radius==2 || radius==3) vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(50,240);
            if(radius==4) vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(50,120);
        }else{
            vecEMC7.at(radius-minradius)->GetXaxis()->SetRangeUser(30,60);
            if(radius==2 || radius==3 || radius==4) vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(60,240);
            if(radius==5) vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(60,160);
            if(radius==6) vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(60,120);
        }
            
        vecMB.at(radius-minradius)->SetMarkerStyle(styles[0]);
        vecMB.at(radius-minradius)->SetMarkerSize(sizes[0]);
        vecMB.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecMB.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        vecEMC7.at(radius-minradius)->SetMarkerStyle(styles[1]);
        vecEMC7.at(radius-minradius)->SetMarkerSize(sizes[1]);
        vecEMC7.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecEMC7.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        vecEJE.at(radius-minradius)->SetMarkerStyle(styles[2]);
        vecEJE.at(radius-minradius)->SetMarkerSize(sizes[2]);
        vecEJE.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecEJE.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        legend->AddEntry(vecMB.at(radius-minradius), Form("R=0.%i", radius), "p");

        vecMB.at(radius-minradius)->Draw("p,e1,same");
        vecEMC7.at(radius-minradius)->Draw("p,e1,same");
        vecEJE.at(radius-minradius)->Draw("p,e1,same");
    }
    legend2->AddEntry(vecMB.at(0), "Min. Bias", "p");
    if(system == "pPb") legend2->AddEntry(vecEMC7.at(0), "EJ2", "p");
    else legend2->AddEntry(vecEMC7.at(0), "EMC7", "p");
    if(system == "pPb") legend2->AddEntry(vecEJE.at(0), "EJ1", "p");
    else legend2->AddEntry(vecEJE.at(0), "EJE", "p");

    legend->Draw();
    legend2->Draw();
    if(system=="pp") drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.95,0.92, 0.03,kFALSE, kFALSE, kTRUE);
    if(system=="pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.95,0.92, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Full Jets",0.95,0.89, 0.03,kFALSE, kFALSE, kTRUE);
    canvas->SaveAs(Form("%s/CorrRawSpec/corrRawSpec.%s",output.Data(),fileType.Data()));
    legend->Clear();
}
