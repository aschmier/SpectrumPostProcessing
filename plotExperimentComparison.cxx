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

//================================================================================================================
//Function to parse TGraphAsymmErrors from HEP data file
// - lines that should not be used from the file must be commented out with "%"
// - totalNumberOfColumns should be set to the total number of columns in the file, which are not commented out
// - column numbering starts at 0
// - rows should be given in order of increasing pt
// - if x error column numbers are set to -1, half the distance to the neighbouring bin is taken as value (syemmtric around x)
// - this function cannot deal with only one of columnXErrLow and columnXErrHigh being set to -1
// - this function can not deal with missing y errors (i.e. columnYErrLow or columnYErrHigh set to -1)
// - for symmetric errors and errors stored in column, e.g. columnXErrLow and columnXErrHigh can be set to the same number
// - if err. columns store error values, set isXErrVal or isYErrVal to true
// - if err. columns store bin boundaires, set isXErrVal or isYErrVal to false
//================================================================================================================
TGraphAsymmErrors* ParseHEPData(TString hepDataFile,
                                Int_t   totalNumberOfColumns,
                                Int_t   columnX,
                                Int_t   columnXErrLow,
                                Int_t   columnXErrHigh,
                                Int_t   columnY,
                                Int_t   columnYErrLow,
                                Int_t   columnYErrHigh,
                                Bool_t  isXErrVal,
                                Bool_t  isYErrVal,
                                Bool_t  debugMode = kFALSE) {

    // create streamer
    ifstream file;
    if (debugMode) cout << "HEP data file: " << hepDataFile.Data() << endl;
    file.open(hepDataFile,ios_base::in);
    if (!file) {
        cout << "ERROR: HEP data file " << hepDataFile.Data() << " not found!" << endl;
        return NULL;
    }

    // check for correct column numbers
    if (columnX<0) {
        cout << "ERROR: columnX set to " << columnX << endl;
        return NULL;
    }
    if (columnY<0) {
        cout << "ERROR: columnY set to " << columnY << endl;
        return NULL;
    }
    if (columnYErrLow<0 || columnYErrHigh<0) {
        cout << "ERROR: columnYErrLow set to " << columnYErrLow << " and columnYErrHigh set to " << columnYErrHigh << endl;
        return NULL;
    }

    // initialize vectors for temporary storage of values
    std::vector<Double_t> xVal;
    std::vector<Double_t> xErrLow;
    std::vector<Double_t> xErrHigh;
    std::vector<Double_t> yVal;
    std::vector<Double_t> yErrLow;
    std::vector<Double_t> yErrHigh;

    // read from file
    TString                 tempString;
    std::vector<TString>    tempStringColumn(totalNumberOfColumns);
    std::string line;
    for( std::string line; getline(file, line); ) {
        file >> tempString;
        if (!tempString.BeginsWith("%") && !tempString.BeginsWith("#") && tempString.CompareTo("")) {
            tempStringColumn[0]     = tempString;
            if (debugMode) cout << tempStringColumn[0].Data() << "\t";
            for (Int_t i=1; i<totalNumberOfColumns; i++) {
                file >> tempStringColumn[i];
                if (debugMode) cout << tempStringColumn[i].Data() << "\t";
            }
            if (debugMode) cout << endl;

            // x value and error
            xVal.push_back(tempStringColumn[columnX].Atof());
            if (columnXErrLow>=0)   xErrLow.push_back(tempStringColumn[columnXErrLow].Atof());
            else                    xErrLow.push_back(-1);
            if (columnXErrHigh>=0)  xErrHigh.push_back(tempStringColumn[columnXErrHigh].Atof());
            else                    xErrHigh.push_back(-1);

            // y value and error
            yVal.push_back(tempStringColumn[columnY].Atof());
            yErrLow.push_back(tempStringColumn[columnYErrLow].Atof());
            yErrHigh.push_back(tempStringColumn[columnYErrHigh].Atof());
        } else
            continue;
    }

    // check for equal number of rows for each column
    Bool_t  isEqualNumberOfRows     = kTRUE;
    Int_t   nRowsTemp[6];
    nRowsTemp[0]                    = xVal.size();
    nRowsTemp[1]                    = xErrLow.size();
    nRowsTemp[2]                    = xErrHigh.size();
    nRowsTemp[3]                    = yVal.size();
    nRowsTemp[4]                    = yErrLow.size();
    nRowsTemp[5]                    = yErrHigh.size();
    for (Int_t i=0; i<5; i++) {
        if (nRowsTemp[i]!=nRowsTemp[i+1]) {
            isEqualNumberOfRows     = kFALSE;
            break;
        }
    }
    if (!isEqualNumberOfRows) {
        cout << "number of rows in " << hepDataFile.Data() << " are not equal for different columns!" << endl;
        return NULL;
    }
    Int_t nRows                     = xVal.size();

    // calculate x errors if necessary (i.e. column numbers set to -1)
    std::vector<Double_t> tempXErr(xVal.size());
    if (columnXErrLow<0 || columnXErrHigh<0) {
        for (Int_t i=0; i<nRows; i++) {

            // calculate x error
            if (i==0)               tempXErr[i] = (xVal[1]-xVal[0])/2;
            else if (i==nRows-1)    tempXErr[i] = xVal[i]-(xVal[i-1] + tempXErr[i-1]);
            else                    tempXErr[i] = (xVal[i]-xVal[i-1])/2;

            // set error
            xErrLow[i]              = tempXErr[i];
            xErrHigh[i]             = tempXErr[i];
        }
    }

    // calculate errors if bin boundaries were given
    if (!isXErrVal && columnXErrLow>=0 && columnXErrHigh>=0) {
        for (Int_t i=0; i<nRows; i++) {
            xErrLow[i]              = TMath::Abs(xVal[i]-xErrLow[i]);
            xErrHigh[i]             = TMath::Abs(xErrHigh[i]-xVal[i]);
        }
    }
    if (!isYErrVal) {
        for (Int_t i=0; i<nRows; i++) {
            yErrLow[i]              = TMath::Abs(yVal[i]-yErrLow[i]);
            yErrHigh[i]             = TMath::Abs(yErrHigh[i]-yVal[i]);
        }
    }

    // set errors to absolute values, direction is taken care of by TGraphAsymmErrors
    for (Int_t i=0; i<nRows; i++) {
        xErrLow[i]                  = TMath::Abs(xErrLow[i]);
        xErrHigh[i]                 = TMath::Abs(xErrHigh[i]);

        yErrLow[i]                  = TMath::Abs(yErrLow[i]);
        yErrHigh[i]                 = TMath::Abs(yErrHigh[i]);
    }

    // cout values (debug mode)
    if (debugMode) {
        cout << "nRows = " << nRows << endl;
        for (Int_t i=0; i<nRows; i++) {
            cout << "x = " << xVal[i] << "\t+ " << xErrHigh[i] << "\t- " << xErrLow[i] << "\t y = " << yVal[i] << "\t+ " << yErrHigh[i] << "\t- " << yErrLow[i] << "rel unc: "<< yErrHigh[i] /yVal[i]*100<< endl;
        }
    }

    // create TGraphAsymmErrors
    TGraphAsymmErrors* graph        = new TGraphAsymmErrors(nRows);
    for (Int_t i=0; i<nRows; i++) {
        graph->SetPoint(        i, xVal[i], yVal[i]);
        graph->SetPointError(   i, xErrLow[i], xErrHigh[i], yErrLow[i], yErrHigh[i]);
    }
    return graph;
}

void plotExperimentComparison(TString alicefiles, TString atlasfiles, TString output, TString fileType)
{
    Double_t textSize     = 0.03;
    int radius = 4;

    int scale[5] = {1,3,10,30,100};

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.1,1.,1.5,1.2,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    double alphasval = 0.25;

    Color_t coloralice    = kGreen+2;
    Color_t coloratlas    = kOrange+1;
    Color_t colorcms   = kBlack;

    vector<TGraphErrors*> vecSpectrum8TeV;
    vector<TGraphErrors*> vecSpectrumSys8TeV;
    vector<TGraphErrors*> vecRatio8TeV;
    vector<TGraphErrors*> vecRatioSys8TeV;

    vector<TGraphAsymmErrors*> vecSpectrumALTASlowY;
    vector<TGraphAsymmErrors*> vecSpectrumSysALTASlowY;
    vector<TGraphAsymmErrors*> vecRatioALTASlowY;
    vector<TGraphAsymmErrors*> vecRatioSysALTASlowY;

    vector<TGraphAsymmErrors*> vecSpectrumALTAShighY;
    vector<TGraphAsymmErrors*> vecSpectrumSysALTAShighY;
    vector<TGraphAsymmErrors*> vecRatioALTAShighY;
    vector<TGraphAsymmErrors*> vecRatioSysALTAShighY;

    gSystem->Exec("mkdir -p "+output+"/ExperimentComparison");

    // Open root files
    TFile *fresults = TFile::Open(alicefiles);
    if(!fresults || fresults->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    // 5 TeV
    for(int radius = minradius; radius <=maxradius; radius++){
        TString tableNameSpec = Form("%s/%s.tsv", hepdatafolder5TeV.Data(), (mapSpectrum5TeV[radius]).Data());
        TGraphAsymmErrors *spectrum5TeV_stat = ParseHEPData(tableNameSpec, 12, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
        TGraphAsymmErrors *spectrum5TeV_corr = ParseHEPData(tableNameSpec, 12, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);
        TGraphAsymmErrors *spectrum5TeV_shap = ParseHEPData(tableNameSpec, 12, 0, 1, 2, 3, 8, 9, kFALSE, kTRUE);
        //TGraphAsymmErrors *spectrum5TeV_lumi = ParseHEPData(tableNameSpec, 12, 0, 1, 2, 3, 10, 11, kFALSE, kTRUE);

        TGraphAsymmErrors *spectrum5TeV_sysE = (TGraphAsymmErrors*)spectrum5TeV_corr->Clone(Form("spectrum5TeV_sysE_R0%i", radius));
        for(int point = 0; point < spectrum5TeV_sysE->GetN(); point++){
            double syserror = sqrt(pow(spectrum5TeV_corr->GetErrorY(point),2) + pow(spectrum5TeV_shap->GetErrorY(point),2));
            spectrum5TeV_sysE->SetPointEYhigh(point,syserror);
            spectrum5TeV_sysE->SetPointEYlow(point,syserror);
        }

        for(int point = 0; point < spectrum5TeV_stat->GetN(); point++){
            double staterror = spectrum5TeV_stat->GetErrorY(point);
            spectrum5TeV_stat->SetPointEYhigh(point,staterror);
            spectrum5TeV_stat->SetPointEYlow(point,staterror);
            spectrum5TeV_stat->SetPointEXhigh(point,0);
            spectrum5TeV_stat->SetPointEXlow(point,0);
            double syserror = spectrum5TeV_sysE->GetErrorY(point);
            spectrum5TeV_sysE->SetPointEYhigh(point,syserror);
            spectrum5TeV_sysE->SetPointEYlow(point,syserror);
        }

        spectrum5TeV_stat->SetMarkerStyle(styles[radius-minradius]);
        spectrum5TeV_stat->SetMarkerSize(sizesempty[radius-minradius]);
        spectrum5TeV_stat->SetMarkerColor(color5TeV);
        spectrum5TeV_stat->SetLineColor(color5TeV);

        spectrum5TeV_sysE->SetFillColorAlpha(color5TeV,alphasval);
        spectrum5TeV_sysE->SetLineColor(color5TeV);
        spectrum5TeV_sysE->SetFillStyle(1000);

        vecSpectrum5TeV.push_back(spectrum5TeV_stat);
        vecSpectrumSys5TeV.push_back(spectrum5TeV_sysE);

        if(radius != minradius){
            TString tableNameRatio = Form("%s/%s.tsv", hepdatafolder5TeV.Data(), (mapRatio5TeV[radius]).Data());
            TGraphAsymmErrors *ratio5TeV_stat = ParseHEPData(tableNameRatio, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
            TGraphAsymmErrors *ratio5TeV_sysE = ParseHEPData(tableNameRatio, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);

            for(int point = 0; point < ratio5TeV_stat->GetN(); point++){
                double staterror = ratio5TeV_stat->GetErrorY(point);
                ratio5TeV_stat->SetPointEYhigh(point,staterror);
                ratio5TeV_stat->SetPointEYlow(point,staterror);
                ratio5TeV_stat->SetPointEXhigh(point,0);
                ratio5TeV_stat->SetPointEXlow(point,0);
                double syserror = ratio5TeV_sysE->GetErrorY(point);
                ratio5TeV_sysE->SetPointEYhigh(point,syserror);
                ratio5TeV_sysE->SetPointEYlow(point,syserror);
            }

            ratio5TeV_stat->SetMarkerStyle(styles[radius-(minradius)]);
            ratio5TeV_stat->SetMarkerSize(sizesempty[radius-(minradius)]);
            ratio5TeV_stat->SetMarkerColor(color5TeV);
            ratio5TeV_stat->SetLineColor(color5TeV);

            ratio5TeV_sysE->SetLineColor(color5TeV);
            ratio5TeV_sysE->SetFillColorAlpha(color5TeV,alphasval);
            ratio5TeV_sysE->SetFillStyle(1);

            vecRatio5TeV.push_back(ratio5TeV_stat);
            vecRatioSys5TeV.push_back(ratio5TeV_sysE);
        }
    }

    // 8 TeV
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *rdir = (TDirectory*)fresults->Get(Form("R0%i",radius));
        TGraphErrors *spectrum     = (TGraphErrors*)rdir->Get(Form("finalSpectrum_R0%i",radius));
        TGraphErrors *spectrumSys  = (TGraphErrors*)rdir->Get(Form("finalSpectrumSys_R0%i",radius));

        for(int point = 0; point < spectrum->GetN(); point++){
            double staterror = spectrum->GetErrorY(point);
            spectrum->SetPointError(point,0,staterror);
            double syserror = spectrumSys->GetErrorY(point);
            spectrumSys->SetPointError(point,spectrumSys->GetErrorX(point),syserror);
        }

        spectrum->SetMarkerStyle(stylesfilled[radius-minradius]);
        spectrum->SetMarkerSize(sizesfilled[radius-minradius]);
        spectrum->SetMarkerColor(color8TeV);
        spectrum->SetLineColor(color8TeV);

        spectrumSys->SetFillColorAlpha(color8TeV,alphasval);
        spectrumSys->SetLineColor(color8TeV);
        spectrumSys->SetFillStyle(1000);

        vecSpectrum8TeV.push_back(spectrum);
        vecSpectrumSys8TeV.push_back(spectrumSys);

        if(radius != minradius){
            TGraphErrors *ratio        = (TGraphErrors*)rdir->Get(Form("finalRatio_R0%i",radius));
            TGraphErrors *ratioSys     = (TGraphErrors*)rdir->Get(Form("finalRatioSys_R0%i",radius));

            for(int point = 0; point < ratio->GetN(); point++){
                double staterror = ratio->GetErrorY(point);
                ratio->SetPointError(point,0,staterror);
                double syserror = ratioSys->GetErrorY(point);
                ratioSys->SetPointError(point,ratioSys->GetErrorX(point),syserror);
            }

            ratio->SetMarkerStyle(stylesfilled[radius-(minradius)]);
            ratio->SetMarkerSize(sizesfilled[radius-(minradius)]);
            ratio->SetMarkerColor(color8TeV);
            ratio->SetLineColor(color8TeV);

            ratioSys->SetLineColor(color8TeV);
            ratioSys->SetFillColorAlpha(color8TeV,alphasval);
            ratioSys->SetFillStyle(1000);

            vecRatio8TeV.push_back(ratio);
            vecRatioSys8TeV.push_back(ratioSys);
        }
    }

    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("cSpectrum", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();

    TCanvas *cRatio   = new TCanvas("c2", "", 800, 800);
    DrawPaperCanvasSettings(cRatio,0.14,0.025,0.025,0.08);

    gStyle->SetOptStat(0);

    TLegend *legendSpectrum;
    TLegend *legendRatio;

    TLegend *legendErrorKey =  GetAndSetLegend2(0.19,0.11,0.39,0.11+(2)*textSize,textSize);
    legendErrorKey->AddEntry(vecSpectrumSys13TeV.at(0), "Systematic Uncertainty", "f");
    legendErrorKey->AddEntry(vecSpectrum13TeV.at(0), "Statistical Uncertainty", "e");

    TLegend *legendErrorKeyRatio =  GetAndSetLegend2(0.6,0.11,0.8,0.11+(2)*textSize,textSize);
    legendErrorKeyRatio->AddEntry(vecRatioSys13TeV.at(0), "Systematic Uncertainty", "f");
    legendErrorKeyRatio->AddEntry(vecRatio13TeV.at(0), "Statistical Uncertainty", "e");

    // Set up dummy histos for plotting
    TH1D *dummySpectrum = new TH1D("dummySpectrum","",350,0,350);
    dummySpectrum->GetXaxis()->SetRangeUser(0,270);
    dummySpectrum->GetYaxis()->SetRangeUser(2e-8,3e-2);
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    TH1D *dummyRatio = new TH1D("dummyRatio","",350,0,350);
    dummyRatio->GetXaxis()->SetRangeUser(0,270);
    dummyRatio->GetYaxis()->SetRangeUser(0,1.4);
    SetStyleHistoTH1ForGraphs(dummyRatio,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}^{#it{R}=0.2}}{d#it{p}_{T}d#it{#eta}}/#frac{d^{2}#it{#sigma}^{#it{R}=0.#it{X}}}{d#it{p}_{T}d#it{#eta}}",textSize,0.035,textSize,0.035,1,1.7);

    TLine * line = new TLine (0,1,280,1);
    line->SetLineColor(14);
    line->SetLineWidth(3);
    line->SetLineStyle(7);

    cSpectrum->cd();
    ///////////////////////////////
    // Plot spectrum comparisons //
    ///////////////////////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        if(radius == 2 || radius == 4) legendSpectrum    =  GetAndSetLegend2(0.71,0.82-(4)*textSize,0.91,0.82,textSize);
        else legendSpectrum    =  GetAndSetLegend2(0.71,0.82-(3)*textSize,0.91,0.82,textSize);

        TGraphErrors *finalSpectrum8TeV = (TGraphErrors*)vecSpectrum8TeV.at(radius-minradius)->Clone(Form("finalSpectrum8_R0%i", radius));
        TGraphErrors *finalSpectrumSys8TeV = (TGraphErrors*)vecSpectrumSys8TeV.at(radius-minradius)->Clone(Form("finalSpectrumSys8_R0%i", radius));
        legendSpectrum->AddEntry(finalSpectrum8TeV, "#sqrt{#it{s}} = 8 TeV", "p");

        TGraphAsymmErrors *finalSpectrum5TeV = (TGraphAsymmErrors*)vecSpectrum5TeV.at(radius-minradius)->Clone(Form("finalSpectrum5_R0%i", radius));
        TGraphAsymmErrors *finalSpectrumSys5TeV = (TGraphAsymmErrors*)vecSpectrumSys5TeV.at(radius-minradius)->Clone(Form("finalSpectrumSys5_R0%i", radius));
        legendSpectrum->AddEntry(finalSpectrum5TeV, "#sqrt{#it{s}} = 5 TeV", "p");

        TGraphAsymmErrors *finalSpectrum13TeV = (TGraphAsymmErrors*)vecSpectrum13TeV.at(radius-minradius)->Clone(Form("finalSpectrum13_R0%i", radius));
        TGraphAsymmErrors *finalSpectrumSys13TeV = (TGraphAsymmErrors*)vecSpectrumSys13TeV.at(radius-minradius)->Clone(Form("finalSpectrumSys13_R0%i", radius));
        legendSpectrum->AddEntry(finalSpectrum13TeV, "#sqrt{#it{s}} = 13 TeV", "p");

        TGraphAsymmErrors *finalSpectrum2p76TeV;
        TGraphAsymmErrors *finalSpectrumSys2p76TeV;
        if(radius == 2 || radius == 4){
            finalSpectrum2p76TeV = (TGraphAsymmErrors*)vecSpectrum2p76TeV.at(radius == 2? 0 : 1)->Clone(Form("finalSpectrum2p76TeV_R0%i", radius));
            finalSpectrumSys2p76TeV = (TGraphAsymmErrors*)vecSpectrumSys2p76TeV.at(radius == 2? 0 : 1)->Clone(Form("finalSpectrumSys2p76TeV_R0%i", radius));
            legendSpectrum->AddEntry(finalSpectrum2p76TeV, "#sqrt{#it{s}} = 2.76 TeV", "p");
        }

        dummySpectrum->Draw("axis");
        finalSpectrum8TeV->Draw("p,e1,same");
        finalSpectrumSys8TeV->Draw("e2,same");
        finalSpectrum5TeV->Draw("p,e1,same");
        finalSpectrumSys5TeV->Draw("e2,same");
        finalSpectrum13TeV->Draw("p,e1,same");
        finalSpectrumSys13TeV->Draw("e2,same");
        if(radius == 2 || radius == 4){
            finalSpectrum2p76TeV->Draw("p,e1,same");
            finalSpectrumSys2p76TeV->Draw("e2,same");
        }

        legendSpectrum->Draw("same");
        legendErrorKey->Draw("same");

        drawLatexAdd("ALICE Preliminary",0.93,0.93, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.%i", radius),0.93,0.9, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.87, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

        cSpectrum->SaveAs(Form("%s/EnergyComparisons/SpectrumComparison_R0%i.%s",output.Data(),radius,fileType.Data()));

        legendSpectrum->Clear();
    }

    cRatio->cd();
    ////////////////////////////
    // Plot ratio comparisons //
    ////////////////////////////
    for(int radius = minradius+1; radius <= maxradius; radius++){
        if(radius == 4) legendRatio       = GetAndSetLegend2(0.73,0.94-(4)*textSize,0.93,0.94,textSize);
        else legendRatio       = GetAndSetLegend2(0.73,0.94-(3)*textSize,0.93,0.94,textSize);

        TGraphErrors *finalRatio8TeV = (TGraphErrors*)vecRatio8TeV.at(radius-(minradius+1))->Clone(Form("finalRatio8TeV_R02R0%i", radius));
        TGraphErrors *finalRatioSys8TeV = (TGraphErrors*)vecRatioSys8TeV.at(radius-(minradius+1))->Clone(Form("finalRatioSys8TeV_R02R0%i", radius));
        legendRatio->AddEntry(finalRatio8TeV, "#sqrt{#it{s}} = 8 TeV", "p");

        TGraphErrors *finalRatio5TeV = (TGraphErrors*)vecRatio5TeV.at(radius-(minradius+1))->Clone(Form("finalRatio5TeV_R02R0%i", radius));
        TGraphErrors *finalRatioSys5TeV = (TGraphErrors*)vecRatioSys5TeV.at(radius-(minradius+1))->Clone(Form("finalRatioSys5TeV_R02R0%i", radius));
        legendRatio->AddEntry(finalRatio5TeV, "#sqrt{#it{s}} = 5 TeV", "p");

        TGraphErrors *finalRatio13TeV = (TGraphErrors*)vecRatio13TeV.at(radius-(minradius+1))->Clone(Form("finalRatio13TeV_R02R0%i", radius));
        TGraphErrors *finalRatioSys13TeV = (TGraphErrors*)vecRatioSys13TeV.at(radius-(minradius+1))->Clone(Form("finalRatioSys13TeV_R02R0%i", radius));
        legendRatio->AddEntry(finalRatio13TeV, "#sqrt{#it{s}} = 13 TeV", "p");

        TGraphErrors *finalRatio2p76TeV;
        TGraphErrors *finalRatioSys2p76TeV;
        if(radius == 4){
            finalRatio2p76TeV = (TGraphErrors*)vecRatio2p76TeV.at(0)->Clone(Form("finalRatio2p76TeV_R02R0%i", radius));
            finalRatioSys2p76TeV = (TGraphErrors*)vecRatioSys2p76TeV.at(0)->Clone(Form("finalRatioSys2p76TeV_R02R0%i", radius));
            legendRatio->AddEntry(finalRatio2p76TeV, "#sqrt{#it{s}} = 2.76 TeV", "p");
        }

        dummyRatio->Draw("axis");
        finalRatioSys8TeV->Draw("e2,same");
        finalRatio8TeV->Draw("p,e1,same");
        finalRatioSys5TeV->Draw("e2,same");
        finalRatio5TeV->Draw("p,e1,same");
        finalRatioSys13TeV->Draw("e2,same");
        finalRatio13TeV->Draw("p,e1,same");
        if(radius == 2 || radius == 4){
            finalRatioSys2p76TeV->Draw("e2,same");
            finalRatio2p76TeV->Draw("p,e1,same");
        }

        line->Draw("same");
        legendRatio->Draw("same");
        legendErrorKeyRatio->Draw("same");

        drawLatexAdd("ALICE Preliminary",0.17,0.92, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.2/0.%i", radius),0.17,0.88, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.17,0.84, textSize,kFALSE, kFALSE, false);
        drawLatexAdd("|#it{#eta}^{tr}| > 0.7, |#it{#eta}^{cl}| > 0.7, |#it{#eta}^{jet}| > 0.7 - #it{R}",0.17,0.8, textSize,kFALSE, kFALSE, false);

        cRatio->SaveAs(Form("%s/EnergyComparisons/RatioComparison_R0%i.%s",output.Data(),radius,fileType.Data()));

        legendRatio->Clear();
    }
}
