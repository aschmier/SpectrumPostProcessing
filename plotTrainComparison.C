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

void plotTrainComparison(TString originalFile, TString systematicsFile, TString original, TString variation, TString type, Int_t radius)
{
    // Define variables
    Double_t textSize     = 0.03;
    Int_t regnum          = 7;
    TString baseDirectory = "/home/austin/alice/unfoldingResults";

    int styles[14] = {4,8,21,25,27,28,33,34,41,43,45,47,48,49};
    int colors[14] = {1,2,3,4,6,7,8,9,28,30,40,41,46,49};

    vector<TString> files;
    vector<TString> variations;
    vector<TH1D*> syshistos;

    // Open the default file and get default histograms
    TFile *fOrig = TFile::Open(originalFile);
    if(!fOrig || fOrig->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    TDirectory *regDirOrig = (TDirectory*)fOrig->Get(Form("R0%i",radius));
    TDirectory *regOrig    = (TDirectory*)regDirOrig->Get(Form("reg%i",regnum));
    TH1D *normUnfoldedOrig = (TH1D*)regOrig->Get(Form("normalized_reg%i",regnum));

    // Save the file names and variations for each uncertainty into vectors
    ifstream sysfile(systematicsFile);
    if(!sysfile){ cout << "No systematics file found!" << endl; return; }
    TString sfile, var;
    while(sysfile >> sfile >> var){
        files.push_back(sfile);
        variations.push_back(var);
    }

    // Get the histograms...
    for(int i = 0; i < files.size(); i++){
        TFile *fVar = TFile::Open(files.at(i));
        if(!fVar || fVar->IsZombie()) return;
        TDirectory *regDirSys = (TDirectory*)fVar->Get(Form("R0%i",radius));
        TDirectory *regSys    = (TDirectory*)regDirSys->Get(Form("reg%i",regnum));
        TH1D *normUnfoldedSys = (TH1D*)regSys->Get(Form("normalized_reg%i",regnum));
        syshistos.push_back(normUnfoldedSys);
    }

    TH1D *statError = (TH1D*)normUnfoldedOrig->Clone("ratio");
    TH1D *normUnfoldedOrig_e0 = (TH1D*)normUnfoldedOrig->Clone("zero_error_orig");
    for(int i = 0; i < normUnfoldedOrig_e0->GetNbinsX(); i++) normUnfoldedOrig_e0->SetBinError(i,0);
    statError->Divide(normUnfoldedOrig,normUnfoldedOrig_e0);

    // Set up canvas for plotting
    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

    TLegend *legend =  GetAndSetLegend2(0.12,0.14,0.5,0.12+((syshistos.size()+1)/2)*textSize*1.5,textSize,2);

    SetStyleHistoTH1ForGraphs(statError,"","p_{T} (GeV/c)","Var/Orig",0.03,0.04,0.03,0.04,1,1.2);
    statError->GetXaxis()->SetRangeUser(20,320);
    statError->GetYaxis()->SetRangeUser(0.85,1.15);
    statError->SetFillColor(17);
    statError->SetMarkerStyle(21);
    statError->SetMarkerSize(2.5);
    statError->SetMarkerColor(17);
    //statError->Sumw2();
    statError->Draw("E2");

    TLine * l1 = new TLine (15,1,280,1);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);
    l1->Draw("same");

    legend->AddEntry(statError, "Statistical Uncertainty", "p");

    for(int j = 0; j < syshistos.size(); j++){
        TH1D *sysRatio = (TH1D*)normUnfoldedOrig->Clone(Form("syshist_%i",j));
        sysRatio->Divide(syshistos.at(j),normUnfoldedOrig,1,1,"B");
        sysRatio->SetMarkerStyle(styles[j+1]);
        sysRatio->SetLineColor(colors[j+1]);
        sysRatio->SetLineWidth(2);
        sysRatio->SetMarkerSize(2.5);
        sysRatio->SetMarkerColor(colors[j+1]);
        sysRatio->Draw("same");
        legend->AddEntry(sysRatio, Form("%s/%s",variations.at(j).Data(),original.Data()), "p");
    }

    legend->Draw();

    drawLatexAdd(Form("%s Unfolding, Reg %i",type.Data(),regnum),0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
    drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);

    c->SaveAs(Form("/home/austin/alice/unfoldingResults/train_comparison/%s_%s_R0%i_reg%i.png",variation.Data(),type.Data(),radius,regnum));
}
