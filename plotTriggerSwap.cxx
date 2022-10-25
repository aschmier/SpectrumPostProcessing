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
    int colors[10] = {1,2,4,6,8,9,30,40,46,49};
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


    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.12,0.14,0.5,0.12+(spectra.size()+1)*textSize*1.5,textSize);

    for(int i = 0; i < spectra.size(); i++){
        SetStyleHistoTH1ForGraphs(spectra.at(i),"","p_{T} (GeV/c)","d#sigma/dp_{T}d#eta (mb/(GeV/c))",0.03,0.04,0.03,0.04,1,1.2);
        spectra.at(i)->GetXaxis()->SetRangeUser(20,320);
        //spectra.at(i)->GetYaxis()->SetRangeUser(0.000000009,.0001);
        spectra.at(i)->SetFillColor(colors[i]);
        spectra.at(i)->SetMarkerStyle(styles[i]);
        spectra.at(i)->SetMarkerSize(2.5);
        spectra.at(i)->SetMarkerColor(colors[i]);
        if(system == "pp") legend->AddEntry(spectra.at(i), triggers_pp[i].Data(), "p");
        if(system == "pA") legend->AddEntry(spectra.at(i), triggers_pA[i].Data(), "p");
        if(i==0) spectra.at(i)->Draw();
        else spectra.at(i)->Draw("same");
    }

    legend->Draw();

    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("Full Jets",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/TriggerSwap/TriggerSwap_R0%i.%s",output.Data(),radius,fileType.Data()));

    c->SetLogy(0);
    c->SetLogx(1);

    SetStyleHistoTH1ForGraphs(rEMCINT,"","p_{T} (GeV/c)","EMC7/INT7",0.03,0.04,0.03,0.04,1,1.2);
    rEMCINT->GetXaxis()->SetRangeUser(4.5,100);
    rEMCINT->GetYaxis()->SetRangeUser(0,2);
    rEMCINT->GetXaxis()->SetMoreLogLabels();
    //rEMCINT->GetYaxis()->SetRangeUser(0.000000009,.0001);
    rEMCINT->SetFillColor(0);
    rEMCINT->SetMarkerStyle(0);
    rEMCINT->SetMarkerSize(2.5);
    rEMCINT->SetMarkerColor(0);
    rEMCINT->Draw();

    TLine * l1 = new TLine (0,1,280,1);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);
    l1->Draw("same");

    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/TriggerSwap/ratio_EMCINT_data_R0%i.%s",output.Data(),radius,fileType.Data()));

    SetStyleHistoTH1ForGraphs(rEJEEMC,"","p_{T} (GeV/c)","EJE/EMC7",0.03,0.04,0.03,0.04,1,1.2);
    rEJEEMC->GetXaxis()->SetRangeUser(4.5,100);
    rEJEEMC->GetYaxis()->SetRangeUser(0,2);
    rEJEEMC->GetXaxis()->SetMoreLogLabels();
    //rEJEEMC->GetYaxis()->SetRangeUser(0.000000009,.0001);
    rEJEEMC->SetFillColor(0);
    rEJEEMC->SetMarkerStyle(0);
    rEJEEMC->SetMarkerSize(2.5);
    rEJEEMC->SetMarkerColor(0);
    rEJEEMC->Draw();

    l1->Draw("same");

    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("%s/TriggerSwap/ratio_EJEEMC_data_R0%i.%s",output.Data(),radius,fileType.Data()));
}
