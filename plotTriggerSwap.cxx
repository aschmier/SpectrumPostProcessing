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

void plotTriggerSwap(TString unfoldedFile, TString output, int radius, TString system, TString fileType)
{
    Double_t textSize     = 0.03;
    TString baseDirectory = "/home/austin/alice/unfoldingResults";
    int maxradius = 6;

    int styles[10] = {4,25,27,28,35,36,40,42,44,46};
    Color_t colors[10] = {1,kRed+2,4,6,8,9,30,40,46,49};
    TString triggers_pp[3] = {"INT7","EMC7","EJE"};
    TString triggers_pA[3] = {"INT7","EJ2","EJ1"};

    vector<TH1D*> spectra;

    TFile *fUnf = TFile::Open(unfoldedFile);
    if(!fUnf || fUnf->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    TDirectory *dUnf     = (TDirectory*)fUnf->Get(Form("R0%i",radius));
    TDirectory *rawlevel = (TDirectory*)dUnf->Get("rawlevel");
    TH1D *hINT7;
    TH1D *hEMC7;
    TH1D *hEJE;
    if(system == "pp"){
        hINT7       = (TH1D*)rawlevel->Get("mbrebinned");
        hEMC7       = (TH1D*)rawlevel->Get("emc7rebinned");
        hEJE        = (TH1D*)rawlevel->Get("ejerebinned");
    }else{
        hINT7       = (TH1D*)rawlevel->Get("mbrebinned");
        hEMC7       = (TH1D*)rawlevel->Get("ej2rebinned");
        hEJE        = (TH1D*)rawlevel->Get("ej1rebinned");
    }
    hINT7->Scale(1.,"width");
    hEMC7->Scale(1.,"width");
    hEJE->Scale(1.,"width");

    spectra.push_back(hINT7);
    spectra.push_back(hEMC7);
    spectra.push_back(hEJE);

    TH1D *rEMCINT = (TH1D*)hINT7->Clone("rEMCINT");
    rEMCINT->Divide(hEMC7,hINT7);

    TH1D *rEJEEMC = (TH1D*)hEMC7->Clone("rEJEEMC");
    rEJEEMC->Divide(hEJE,hEMC7);

    TH1D *rEJEINT = (TH1D*)hINT7->Clone("rEJEINT");
    rEJEINT->Divide(hEJE,hINT7);


    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.07,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.12,0.14,0.5,0.12+(spectra.size()+1)*textSize*1.5,textSize);

    // Make dummy histo
    TH1D *h = new TH1D("h","",260,0,260);
    SetStyleHistoTH1ForGraphs(h,"","#it{p}_{T} (GeV/#it{c})","d#sigma/d#it{p}_{T}d#eta (mb/(GeV/#it{c}))",0.03,0.04,0.03,0.04,1.1,0.8);
    h->GetXaxis()->SetRangeUser(0,320);
    if(system == "pPb") h->GetYaxis()->SetRangeUser(2e-7,10);
    else h->GetYaxis()->SetRangeUser(2e-11,3e-4);
    h->Draw("axis");

    for(int i = 0; i < spectra.size(); i++){
        spectra.at(i)->SetFillColor(colors[i]);
        spectra.at(i)->SetMarkerStyle(styles[i]);
        spectra.at(i)->SetMarkerSize(2.5);
        spectra.at(i)->SetMarkerColor(colors[i]);
        spectra.at(i)->SetLineColor(colors[i]);
        if(i==0) spectra.at(i)->GetXaxis()->SetRangeUser(10,80);
        else if(i==1) spectra.at(i)->GetXaxis()->SetRangeUser(10,160);
        else if(i==2) spectra.at(i)->GetXaxis()->SetRangeUser(10,240);
        if(system == "pp") legend->AddEntry(spectra.at(i), triggers_pp[i].Data(), "p");
        if(system == "pA") legend->AddEntry(spectra.at(i), triggers_pA[i].Data(), "p");
        spectra.at(i)->Draw("same");
    }

    legend->Draw();

    if(system == "pp") drawLatexAdd("pp #it{#sqrt{s}} = 8 TeV",0.93,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    if(system == "pPb") drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV",0.93,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Full Jets",0.93,0.87, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/TriggerSwap/ScaledSpectra_R0%i.%s",output.Data(),radius,fileType.Data()));

    c->SetLogy(0);
    c->SetLogx(1);

    // Make dummy histo
    TH1D *h2 = new TH1D("h2","",260,0,260);
    SetStyleHistoTH1ForGraphs(h2,"","p_{T} (GeV/c)","EJ2/INT7",0.03,0.04,0.03,0.04,1.1,0.8);
    h2->GetXaxis()->SetRangeUser(8,260);
    h2->GetYaxis()->SetRangeUser(0,2);
    h2->GetXaxis()->SetMoreLogLabels();
    h2->Draw("axis");

    rEMCINT->GetXaxis()->SetRangeUser(10,100);
    //rEMCINT->GetYaxis()->SetRangeUser(0,2);
    //rEMCINT->GetXaxis()->SetMoreLogLabels();
    rEMCINT->SetFillColor(colors[0]);
    rEMCINT->SetMarkerStyle(styles[0]);
    rEMCINT->SetMarkerSize(2.5);
    rEMCINT->SetMarkerColor(colors[0]);
    rEMCINT->SetLineColor(colors[0]);
    rEMCINT->Draw("p,e,same");

    TLine * l1 = new TLine (0,1,280,1);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);
    l1->Draw("same");

    if(system == "pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    else drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.93,0.87, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/TriggerSwap/ratio_EMCINT_data_R0%i.%s",output.Data(),radius,fileType.Data()));

    SetStyleHistoTH1ForGraphs(h2,"","p_{T} (GeV/c)","EJ1/EJ2",0.03,0.04,0.03,0.04,1,0.8);
    h2->Draw("axis");

    //rEJEEMC->GetXaxis()->SetRangeUser(4.5,240);
    //rEJEEMC->GetYaxis()->SetRangeUser(0,2);
    //rEJEEMC->GetXaxis()->SetMoreLogLabels();
    //rEJEEMC->GetYaxis()->SetRangeUser(0.000000009,.0001);
    rEJEEMC->SetFillColor(colors[1]);
    rEJEEMC->SetMarkerStyle(styles[1]);
    rEJEEMC->SetMarkerSize(2.5);
    rEJEEMC->SetMarkerColor(colors[1]);
    rEJEEMC->SetLineColor(colors[1]);
    rEJEEMC->Draw("p,e,same");

    l1->Draw("same");

    if(system == "pp") drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.93,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    if(system == "pPb") drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV",0.93,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.93,0.87, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/TriggerSwap/ratio_EJEEMC_data_R0%i.%s",output.Data(),radius,fileType.Data()));

    SetStyleHistoTH1ForGraphs(h2,"","p_{T} (GeV/c)","EJ1/INT7",0.03,0.04,0.03,0.04,1,0.8);
    h2->Draw("axis");

    rEJEINT->GetXaxis()->SetRangeUser(10,100);
    //rEJEEMC->GetYaxis()->SetRangeUser(0,2);
    //rEJEEMC->GetXaxis()->SetMoreLogLabels();
    //rEJEEMC->GetYaxis()->SetRangeUser(0.000000009,.0001);
    rEJEINT->SetFillColor(colors[2]);
    rEJEINT->SetMarkerStyle(styles[2]);
    rEJEINT->SetMarkerSize(2.5);
    rEJEINT->SetMarkerColor(colors[2]);
    rEJEINT->SetLineColor(colors[2]);
    rEJEINT->Draw("p,e,same");

    l1->Draw("same");

    if(system == "pp") drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.93,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    if(system == "pPb") drawLatexAdd("p--Pb #it{#sqrt{s_{NN}}} = 8.16 TeV",0.93,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.93,0.87, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/TriggerSwap/ratio_EJEINT_data_R0%i.%s",output.Data(),radius,fileType.Data()));
}
