#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"

void plotDpT(TString filename, TString outputdir, TString fileType, TString suffix, int minradius, int maxradius, TString datafile, TString system)
{
    double textSize  = 0.03;
    TString jetType  = "Full";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    TString triggers[3];
    triggers[0] = "INT7";
    if(system=="pPb") triggers[1] = "EJ2";
    if(system=="pp") triggers[1] = "EMC7";
    if(system=="pPb") triggers[2] = "EJ1";
    if(system=="pp") triggers[2] = "EJE";
    double nEvt[3];

    gSystem->Exec("mkdir -p "+outputdir);

    TFile *f = TFile::Open(filename.Data());
    if(!f || f->IsZombie()){
        cout << "File not found!" << endl;
        return;
    }

    ifstream file;
    file.open(datafile,ios_base::in);
    if (!file) {
        cout << "ERROR: Data file " << datafile.Data() << " not found!" << endl;
        return;
    }

    // initialize vectors for temporary storage of values
    std::vector<Double_t> xVal;
    std::vector<Double_t> yVal;
    
    // read from file
    TString                 tempString;
    std::vector<TString>    tempStringColumn(2);
    std::string line;
    for( std::string line; getline(file, line); ) {
        file >> tempString;
        tempStringColumn[0]     = tempString;
        file >> tempStringColumn[1];
        // x value and error
        xVal.push_back(tempStringColumn[0].Atof());
        // y value and error
        yVal.push_back(tempStringColumn[1].Atof());
    }

    // check for equal number of rows for each column
    Bool_t  isEqualNumberOfRows     = kTRUE;
    Int_t   nRowsTemp[2];
    nRowsTemp[0]                    = xVal.size();
    nRowsTemp[1]                    = yVal.size();
    
    Int_t nRows                     = xVal.size();

    // create TGraphAsymmErrors
    TGraphAsymmErrors* graph        = new TGraphAsymmErrors(nRows);
    for (Int_t i=0; i<nRows; i++) {
        graph->SetPoint(        i, xVal[i], yVal[i]);
        graph->SetPointError(   i, 0, 0, 0, 0);
    }

    graph->SetMarkerStyle(20);
    graph->SetMarkerSize(1.5);
    graph->SetMarkerColor(kBlack);
    graph->SetLineColor(kBlack);
    graph->SetLineWidth(2);


    TCanvas* canvas       = new TCanvas("canvas", "", 1000, 1000);
    double leftMargin   = 0.1;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.73,0.74-(4)*textSize,0.88,0.74,textSize);
    TLegend *legendmean = GetAndSetLegend2(0.13,0.28-(3)*textSize,0.33,0.28,textSize);
    // loop over radii
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int i = 0; i < 3; i++){
            TString dirname     = Form("JetSpectrum_%sJets_R0%i_%s_%s", jetType.Data(), radius, triggers[i].Data(), suffix.Data());
            TString listname     = Form("JetSpectrum_%sJets_R0%i_%s_%s", jetType.Data(), radius, triggers[i].Data(), suffix.Data());
            TDirectory *dir = (TDirectory*)f->Get(dirname.Data());
            TList *list     = (TList*)dir->Get(listname.Data());
            TH2D *hDpT2D    = (TH2D*)list->FindObject("hRhoVsDeltaPtRC");
            TH1D *hDpT      = (TH1D*)hDpT2D->ProjectionX(Form("hDpT_%s",triggers[i].Data()),1,100);
            hDpT->Scale(1./hDpT->Integral());
            SetStyleHistoTH1ForGraphs(hDpT,"","#delta#it{p}_{T}^{RC} (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);

            hDpT->SetMarkerStyle(styles[i]);
            hDpT->SetMarkerColor(colors[i]);
            hDpT->SetLineColor(colors[i]);
            hDpT->SetMarkerSize(1.5);

            legend->AddEntry(hDpT,triggers[i].Data(),"p");
            if(i==0) hDpT->Draw("p,e");
            else hDpT->Draw("p,e,same");
        }
        legend->Draw("same");
        // draw text
        if(system=="pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
        if(system=="pp") drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.87, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.84, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("R = 0.%i", radius),0.93,0.78,textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("%s", suffix=="default" ? "Full (EMCal) Rho" : "Charged (TPC) Rho"),0.93,0.75,textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/plotDpT_R0%i.%s",outputdir.Data(),radius,fileType.Data()));
        legend->Clear();

        for(int i = 0; i < 3; i++){
            TString dirname     = Form("JetSpectrum_%sJets_R0%i_%s_%s", jetType.Data(), radius, triggers[i].Data(), suffix.Data());
            TString listname     = Form("JetSpectrum_%sJets_R0%i_%s_%s", jetType.Data(), radius, triggers[i].Data(), suffix.Data());
            TDirectory *dir = (TDirectory*)f->Get(dirname.Data());
            TList *list     = (TList*)dir->Get(listname.Data());
            TH2D *hDpT2D    = (TH2D*)list->FindObject("hRhoVsDeltaPtRC");
            TH1D *hRho      = (TH1D*)hDpT2D->ProjectionY(Form("hRho_%s",triggers[i].Data()),1,hDpT2D->GetNbinsX()-1);
            hRho->Scale(1./hRho->Integral());
            SetStyleHistoTH1ForGraphs(hRho,"","#rho (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);
            hRho->GetYaxis()->SetRangeUser(7e-7,1);

            hRho->SetMarkerStyle(styles[i]);
            hRho->SetMarkerColor(colors[i]);
            hRho->SetLineColor(colors[i]);
            hRho->SetMarkerSize(1.5);

            legendmean->AddEntry(hRho,Form("Mean Rho: %f", hRho->GetMean()),"p");
            legend->AddEntry(hRho,triggers[i].Data(),"p");
            if(i==0) hRho->Draw("p,e");
            else hRho->Draw("p,e,same");
        }
        //graph->Draw("p,e,same");
        //legend->AddEntry(graph,"Megan INT7","p");
        legendmean->Draw("same");
        legend->Draw("same");
        // draw text
        if(system=="pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
        if(system=="pp") drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.87, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.84, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("R = 0.%i", radius),0.93,0.78,textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("%s", suffix=="default" ? "Full (EMCal) Rho" : "Charged (TPC) Rho"),0.93,0.75,textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/plotRho_R0%i.%s",outputdir.Data(),radius,fileType.Data()));
        legendmean->Clear();
        legend->Clear();
    }
}
