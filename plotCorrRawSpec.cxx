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

void plotCorrRawSpec(TString file, TString output, TString fileType)
{
    // Define variables
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 6;
    TString triggers[3] = {"Min. Bias (INT7)", "EMCal-L0 (EMC7)", "EMCal-L1 (EJE)"};

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};
    double xsec = 55.8;

    vector<TH1D*> vecMB;
    vector<TH1D*> vecEMC7;
    vector<TH1D*> vecEJE;
    vector<TH1D*> vecCombined;

    // Open unfolding results file
    TFile *f = TFile::Open(file);
    if(!f || f->IsZombie()) return;

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

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.11;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.81,(0.76-(maxradius-minradius+1)*textSize),0.96,0.76,textSize,2);

    TH1D *dummy = (TH1D*)vecMB.at(0)->Clone("dummy");
    dummy->GetXaxis()->SetRangeUser(0,350);
    dummy->GetYaxis()->SetRangeUser(1e-12,2e-2);
    SetStyleHistoTH1ForGraphs(dummy,"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.2);

    for(int radius = minradius; radius <= maxradius; radius++){
        //if(radius==minradius){
            vecMB.at(radius-minradius)->GetXaxis()->SetRangeUser(0,30);
            //vecMB.at(radius-minradius)->GetYaxis()->SetRangeUser(1e-11,2e-4);
            //SetStyleHistoTH1ForGraphs(vecMB.at(radius-minradius),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.2);
        //}
        vecMB.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecMB.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecMB.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        legend->AddEntry(vecMB.at(radius-minradius), Form("R=0.%i", radius), "p");

        //vecMB.at(radius-minradius)->Draw(radius==minradius? "p,e" : "p,e,same");
        if(radius == minradius) dummy->Draw("AXIS");
        vecMB.at(radius-minradius)->Draw("p,e1,same");
    }
    legend->Draw();
    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Full Jets",0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Min. Bias (INT7)",0.95,0.79, 0.03,kFALSE, kFALSE, kTRUE);
    canvas->SaveAs(Form("%s/CorrRawSpec/corrRawSpec_INT7.%s",output.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradius; radius++){
        //if(radius==minradius){
            vecEMC7.at(radius-minradius)->GetXaxis()->SetRangeUser(30,60);
        //    vecEMC7.at(radius-minradius)->GetYaxis()->SetRangeUser(1e-11,2e-4);
        //    SetStyleHistoTH1ForGraphs(vecEMC7.at(radius-minradius),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.2);
        //}
        vecEMC7.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecEMC7.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecEMC7.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        legend->AddEntry(vecEMC7.at(radius-minradius), Form("R=0.%i", radius), "p");

        //vecEMC7.at(radius-minradius)->Draw(radius==minradius? "p,e" : "p,e,same");
        if(radius == minradius) dummy->Draw("AXIS");
        vecEMC7.at(radius-minradius)->Draw("p,e1,same");
    }
    legend->Draw();
    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Full Jets",0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("EMCal-L0 (EMC7)",0.95,0.79, 0.03,kFALSE, kFALSE, kTRUE);
    canvas->SaveAs(Form("%s/CorrRawSpec/corrRawSpec_EMC7.%s",output.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradius; radius++){
        //if(radius==minradius){
            //vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(60,240);
            if(radius==2 || radius==3 || radius==4) vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(60,240);
            if(radius==5) vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(60,160);
            if(radius==6) vecEJE.at(radius-minradius)->GetXaxis()->SetRangeUser(60,130);
        //    vecEJE.at(radius-minradius)->GetYaxis()->SetRangeUser(1e-11,2e-4);
        //    SetStyleHistoTH1ForGraphs(vecEJE.at(radius-minradius),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.2);
        //}
        vecEJE.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecEJE.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecEJE.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        legend->AddEntry(vecEJE.at(radius-minradius), Form("R=0.%i", radius), "p");

        //vecEJE.at(radius-minradius)->Draw(radius==minradius? "p,e" : "p,e,same");
        if(radius == minradius) dummy->Draw("AXIS");
        vecEJE.at(radius-minradius)->Draw("p,e1,same");
    }
    legend->Draw();
    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Full Jets",0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("EMCal-L1 (EJE)",0.95,0.79, 0.03,kFALSE, kFALSE, kTRUE);
    canvas->SaveAs(Form("%s/CorrRawSpec/corrRawSpec_EJE.%s",output.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius==minradius){
            //vecCombined.at(radius-minradius)->GetXaxis()->SetRangeUser(20,240);
            if(radius==2 || radius==3 || radius==4) vecCombined.at(radius-minradius)->GetXaxis()->SetRangeUser(20,240);
            if(radius==5) vecCombined.at(radius-minradius)->GetXaxis()->SetRangeUser(20,160);
            if(radius==6) vecCombined.at(radius-minradius)->GetXaxis()->SetRangeUser(20,130);
            //vecCombined.at(radius-minradius)->GetYaxis()->SetRangeUser(3e-10,2e-3);
            SetStyleHistoTH1ForGraphs(vecCombined.at(radius-minradius),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.2);
        }
        vecCombined.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecCombined.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecCombined.at(radius-minradius)->SetLineColor(colors[radius-minradius]);

        legend->AddEntry(vecCombined.at(radius-minradius), Form("R=0.%i", radius), "p");

        //vecCombined.at(radius-minradius)->Draw(radius==minradius? "p,e" : "p,e,same");
        if(radius == minradius) dummy->Draw("AXIS");
        vecCombined.at(radius-minradius)->Draw("p,e1,same");
    }
    legend->Draw();
    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Full Jets",0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Combined Raw Spectrum",0.95,0.79, 0.03,kFALSE, kFALSE, kTRUE);
    canvas->SaveAs(Form("%s/CorrRawSpec/corrRawSpec_Combined.%s",output.Data(),fileType.Data()));
    legend->Clear();
}
