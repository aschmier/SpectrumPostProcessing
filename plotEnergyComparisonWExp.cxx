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

void plotEnergyComparison(TString analysisresults8TeV, TString hepdatafolder2p76TeV, TString hepdatafolder5TeV, TString spectrumfile13TeV, TString ratiofile13TeV, TString output, TString fileType)
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 5;

    int scale[5] = {1,3,10,30,100};

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.1,1.,1.5,1.2,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    double alphasval = 0.25;

    Color_t colors[13]      = {kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    //Color_t colorsSys[14]   = {kGray, kRed-7, kYellow+6, kGreen-6, kCyan-6, kBlue-6, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    Color_t color8TeV    = kGreen+2;
    Color_t color5TeV    = kOrange+1;
    Color_t color13TeV   = kBlack;
    Color_t color2p76TeV = kMagenta+2;

    std::map<int, TString> mapSpectrum5TeV = {
        {2, "Table2"},
        {3, "Table3"},
        {4, "Table4"},
        {5, "Table5"},
        {6, "Table6"}
    };

    std::map<int, TString> mapRatio5TeV = {
        {3, "Table18"},
        {4, "Table19"},
        {5, "Table20"},
        {6, "Table21"}
    };

    TString spectrum2p76TeVR02 = "Table1";
    TString spectrum2p76TeVR04 = "Table3";
    TString ratio2p76TeVR02R04 = "Table2";

    vector<TGraphErrors*> vecSpectrum8TeV;
    vector<TGraphErrors*> vecSpectrumSys8TeV;
    vector<TGraphErrors*> vecRatio8TeV;
    vector<TGraphErrors*> vecRatioSys8TeV;

    vector<TGraphAsymmErrors*> vecSpectrum5TeV;
    vector<TGraphAsymmErrors*> vecSpectrumSys5TeV;
    vector<TGraphAsymmErrors*> vecRatio5TeV;
    vector<TGraphAsymmErrors*> vecRatioSys5TeV;

    vector<TGraphAsymmErrors*> vecSpectrum13TeV;
    vector<TGraphAsymmErrors*> vecSpectrumSys13TeV;
    vector<TGraphAsymmErrors*> vecRatio13TeV;
    vector<TGraphAsymmErrors*> vecRatioSys13TeV;

    vector<TGraphAsymmErrors*> vecSpectrum2p76TeV;
    vector<TGraphAsymmErrors*> vecSpectrumSys2p76TeV;
    vector<TGraphAsymmErrors*> vecRatio2p76TeV;
    vector<TGraphAsymmErrors*> vecRatioSys2p76TeV;

    gSystem->Exec("mkdir -p "+output+"/EnergyComparisons");

    // Open root files
    TFile *fresults = TFile::Open(analysisresults8TeV);
    if(!fresults || fresults->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    // Open root files
    TFile *fresults13spec = TFile::Open(spectrumfile13TeV);
    if(!fresults13spec || fresults13spec->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    // Open root files
    TFile *fresults13ratio = TFile::Open(ratiofile13TeV);
    if(!fresults13ratio || fresults13ratio->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    // 2.76 TeV
    for(int radius : {2, 4}){
        TString tableNameSpec = Form("%s/%s.tsv", hepdatafolder2p76TeV.Data(), radius == 2? spectrum2p76TeVR02.Data() : spectrum2p76TeVR04.Data());
        TGraphAsymmErrors *spectrum2p76TeV_stat = ParseHEPData(tableNameSpec, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
        TGraphAsymmErrors *spectrum2p76TeV_sysE = ParseHEPData(tableNameSpec, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);

        for(int point = 0; point < spectrum2p76TeV_stat->GetN(); point++){
            double staterror = spectrum2p76TeV_stat->GetErrorY(point);
            spectrum2p76TeV_stat->SetPointEYhigh(point,staterror);
            spectrum2p76TeV_stat->SetPointEYlow(point,staterror);
            spectrum2p76TeV_stat->SetPointEXhigh(point,0);
            spectrum2p76TeV_stat->SetPointEXlow(point,0);
            double syserror = spectrum2p76TeV_sysE->GetErrorY(point);
            spectrum2p76TeV_sysE->SetPointEYhigh(point,syserror);
            spectrum2p76TeV_sysE->SetPointEYlow(point,syserror);
        }

        spectrum2p76TeV_stat->SetMarkerStyle(styles[radius-minradius]);
        spectrum2p76TeV_stat->SetMarkerSize(sizesempty[radius-minradius]);
        spectrum2p76TeV_stat->SetMarkerColor(color2p76TeV);
        spectrum2p76TeV_stat->SetLineColor(color2p76TeV);

        spectrum2p76TeV_sysE->SetFillColorAlpha(color2p76TeV,alphasval);
        spectrum2p76TeV_sysE->SetLineColor(color2p76TeV);
        spectrum2p76TeV_sysE->SetFillStyle(1);

        vecSpectrum2p76TeV.push_back(spectrum2p76TeV_stat);
        vecSpectrumSys2p76TeV.push_back(spectrum2p76TeV_sysE);

        if(radius == 4){
            TString tableNameRatio = Form("%s/%s.tsv", hepdatafolder2p76TeV.Data(), ratio2p76TeVR02R04.Data());
            TGraphAsymmErrors *ratio2p76TeV_stat = ParseHEPData(tableNameRatio, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
            TGraphAsymmErrors *ratio2p76TeV_sysE = ParseHEPData(tableNameRatio, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);

            for(int point = 0; point < ratio2p76TeV_stat->GetN(); point++){
                double staterror = ratio2p76TeV_stat->GetErrorY(point);
                ratio2p76TeV_stat->SetPointEYhigh(point,staterror);
                ratio2p76TeV_stat->SetPointEYlow(point,staterror);
                ratio2p76TeV_stat->SetPointEXhigh(point,0);
                ratio2p76TeV_stat->SetPointEXlow(point,0);
                double syserror = ratio2p76TeV_sysE->GetErrorY(point);
                ratio2p76TeV_sysE->SetPointEYhigh(point,syserror);
                ratio2p76TeV_sysE->SetPointEYlow(point,syserror);
            }

            ratio2p76TeV_stat->SetMarkerStyle(styles[radius-(minradius)]);
            ratio2p76TeV_stat->SetMarkerSize(sizesempty[radius-(minradius)]);
            ratio2p76TeV_stat->SetMarkerColor(color2p76TeV);
            ratio2p76TeV_stat->SetLineColor(color2p76TeV);

            ratio2p76TeV_sysE->SetLineColor(color2p76TeV);
            ratio2p76TeV_sysE->SetFillColorAlpha(color2p76TeV,alphasval);
            ratio2p76TeV_sysE->SetFillStyle(1);

            vecRatio2p76TeV.push_back(ratio2p76TeV_stat);
            vecRatioSys2p76TeV.push_back(ratio2p76TeV_sysE);
        }
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
        spectrum5TeV_sysE->SetFillStyle(1);

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
        spectrumSys->SetFillStyle(1);

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
            ratioSys->SetFillStyle(1);

            vecRatio8TeV.push_back(ratio);
            vecRatioSys8TeV.push_back(ratioSys);
        }
    }

    // 13 TeV
    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *rdirspec       = (TDirectory*)fresults13spec->Get(Form("R0%i",radius));
        TH1D *spectrumStat         = (TH1D*)rdirspec->Get(Form("stat_R0%i",radius));
        TGraphAsymmErrors *spectrumStatGraph = new TGraphAsymmErrors(spectrumStat);
        TGraphAsymmErrors *spectrumCorr = (TGraphAsymmErrors*)rdirspec->Get(Form("correlatedUncertainty_R0%i",radius));
        TGraphAsymmErrors *spectrumShap = (TGraphAsymmErrors*)rdirspec->Get(Form("shapeUncertainty_R0%i",radius));

        TGraphAsymmErrors *spectrumSysE = (TGraphAsymmErrors*)spectrumCorr->Clone(Form("spectrum13TeV_sysE_R0%i", radius));
        for(int point = 0; point < spectrumSysE->GetN(); point++){
            double syserror = sqrt(pow(spectrumCorr->GetErrorY(point),2) + pow(spectrumShap->GetErrorY(point),2));
            spectrumSysE->SetPointEYhigh(point,syserror);
            spectrumSysE->SetPointEYlow(point,syserror);
        }

        for(int point = 0; point < spectrumStatGraph->GetN(); point++){
            double staterror = spectrumStatGraph->GetErrorY(point);
            spectrumStatGraph->SetPointEYhigh(point,staterror);
            spectrumStatGraph->SetPointEYlow(point,staterror);
            spectrumStatGraph->SetPointEXhigh(point,0);
            spectrumStatGraph->SetPointEXlow(point,0);
            double syserror = spectrumSysE->GetErrorY(point);
            spectrumSysE->SetPointEYhigh(point,syserror);
            spectrumSysE->SetPointEYlow(point,syserror);
        }

        spectrumStatGraph->SetMarkerStyle(styles[radius-minradius]);
        spectrumStatGraph->SetMarkerSize(sizesempty[radius-minradius]);
        spectrumStatGraph->SetMarkerColor(color13TeV);
        spectrumStatGraph->SetLineColor(color13TeV);

        spectrumSysE->SetFillColorAlpha(color13TeV,alphasval);
        spectrumSysE->SetLineColor(color13TeV);
        spectrumSysE->SetFillStyle(1);

        for(int point = (spectrumSysE->GetN())-1; point >= 0; point--){
            if(spectrumSysE->GetPointX(point) > 320) spectrumSysE->RemovePoint(point);
            if(spectrumSysE->GetPointX(point) < 20) spectrumSysE->RemovePoint(point);
        }

        for(int point = (spectrumStatGraph->GetN())-1; point >= 0; point--){
            if(spectrumStatGraph->GetPointX(point) > 320) spectrumStatGraph->RemovePoint(point);
            if(spectrumStatGraph->GetPointX(point) < 20) spectrumStatGraph->RemovePoint(point);
        }

        vecSpectrum13TeV.push_back(spectrumStatGraph);
        vecSpectrumSys13TeV.push_back(spectrumSysE);

        if(radius != minradius){
            TDirectory *rdirratio        = (TDirectory*)fresults13ratio->Get(Form("R02R0%i",radius));
            TH1D *ratioStat              = (TH1D*)rdirratio->Get(Form("stat_R02R0%i",radius));
            TGraphAsymmErrors *ratioStatGraph = new TGraphAsymmErrors(ratioStat);
            TGraphAsymmErrors *ratioCorr = (TGraphAsymmErrors*)rdirratio->Get(Form("correlatedUncertainty_R02R0%i",radius));
            TGraphAsymmErrors *ratioShap = (TGraphAsymmErrors*)rdirratio->Get(Form("shapeUncertainty_R02R0%i",radius));

            TGraphAsymmErrors *ratioSysE = (TGraphAsymmErrors*)ratioCorr->Clone(Form("ratio13TeV_sysE_R0%i", radius));
            for(int point = 0; point < ratioSysE->GetN(); point++){
                double syserror = sqrt(pow(ratioCorr->GetErrorY(point),2) + pow(ratioShap->GetErrorY(point),2));
                ratioSysE->SetPointEYhigh(point,syserror);
                ratioSysE->SetPointEYlow(point,syserror);
            }

            for(int point = 0; point < ratioStatGraph->GetN(); point++){
                double staterror = ratioStatGraph->GetErrorY(point);
                ratioStatGraph->SetPointEYhigh(point,staterror);
                ratioStatGraph->SetPointEYlow(point,staterror);
                ratioStatGraph->SetPointEXhigh(point,0);
                ratioStatGraph->SetPointEXlow(point,0);
                double syserror = ratioSysE->GetErrorY(point);
                ratioSysE->SetPointEYhigh(point,syserror);
                ratioSysE->SetPointEYlow(point,syserror);
            }

            ratioStatGraph->SetMarkerStyle(styles[radius-(minradius)]);
            ratioStatGraph->SetMarkerSize(sizesempty[radius-(minradius)]);
            ratioStatGraph->SetMarkerColor(color13TeV);
            ratioStatGraph->SetLineColor(color13TeV);

            ratioSysE->SetLineColor(color13TeV);
            ratioSysE->SetFillColorAlpha(color13TeV,alphasval);
            ratioSysE->SetFillStyle(1);

            for(int point = (ratioSysE->GetN())-1; point >= 0; point--){
                if(ratioSysE->GetPointX(point) > 320) ratioSysE->RemovePoint(point);
                if(ratioSysE->GetPointX(point) < 20) ratioSysE->RemovePoint(point);
            }

            for(int point = (ratioStatGraph->GetN())-1; point >= 0; point--){
                if(ratioStatGraph->GetPointX(point) > 320) ratioStatGraph->RemovePoint(point);
                if(ratioStatGraph->GetPointX(point) < 20) ratioStatGraph->RemovePoint(point);
            }

            vecRatio13TeV.push_back(ratioStatGraph);
            vecRatioSys13TeV.push_back(ratioSysE);
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
    dummySpectrum->GetXaxis()->SetRangeUser(0,340);
    dummySpectrum->GetYaxis()->SetRangeUser(2e-8,3e-2);
    SetStyleHistoTH1ForGraphs(dummySpectrum,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);

    TH1D *dummyRatio = new TH1D("dummyRatio","",350,0,350);
    dummyRatio->GetXaxis()->SetRangeUser(0,340);
    dummyRatio->GetYaxis()->SetRangeUser(0,1.4);
    SetStyleHistoTH1ForGraphs(dummyRatio,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}^{#it{R}=0.2}}{d#it{p}_{T}d#it{#eta}}/#frac{d^{2}#it{#sigma}^{#it{R}=0.#it{X}}}{d#it{p}_{T}d#it{#eta}}",textSize,0.035,textSize,0.035,1,1.7);

    TLine * line = new TLine (0,1,340,1);
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
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.84, textSize,kFALSE, kFALSE, true);

        cSpectrum->SaveAs(Form("%s/EnergyComparisons/SpectrumComparison_R0%i.%s",output.Data(),radius,fileType.Data()));

        legendSpectrum->Clear();
    }
/*
    legendSpectrum->Draw("same");
    legendErrorKey->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.93, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.89, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.85, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.81, textSize,kFALSE, kFALSE, true);

    cSpectrum->SaveAs(Form("%s/EnergyComparisons/SpectrumComparison.%s",output.Data(),fileType.Data()));

    legendSpectrum->Clear();
*/
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
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.17,0.8, textSize,kFALSE, kFALSE, false);

        cRatio->SaveAs(Form("%s/EnergyComparisons/RatioComparison_R0%i.%s",output.Data(),radius,fileType.Data()));

        legendRatio->Clear();
    }
/*
    line->Draw("same");
    legendRatio->Draw("same");
    legendErrorKeyRatio->Draw("same");

    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.19,0.91, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.19,0.87, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.19,0.83, textSize,kFALSE, kFALSE, false);
    drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.19,0.79, textSize,kFALSE, kFALSE, false);

    cRatio->SaveAs(Form("%s/EnergyComparisons/RatioComparison.%s",output.Data(),fileType.Data()));

    legendRatio->Clear();
    */

    // Set up canvases and legends for plotting
    TCanvas *cSqrtS   = new TCanvas("cSqrtS", "", 800, 1000);
    DrawPaperCanvasSettings(cSqrtS,0.15,0.02,0.013,0.065);
    cSqrtS->cd();
    cSqrtS->SetLogy();

    TH1D *sqrtSdummy = new TH1D("sqrtSComp","",16,0,16);
    TLegend *legendSqrtS = GetAndSetLegend2(0.17,0.96-(5)*textSize,0.37,0.96,textSize);

    TLegend *legendErrorKeySqrtS =  GetAndSetLegend2(0.68,0.96-(3)*textSize,0.88,0.96,textSize);
    TLegend *legendFitSqrtS =  GetAndSetLegend2(0.18,0.11,0.38,0.11+(4)*textSize,textSize);


    double binWidthX = 0.3;

    Color_t colorsSqrtS[5] = {kBlack, kRed+1, kBlue+1, kGreen+2, kMagenta+1};

    for(int rSqrtS = minradius; rSqrtS <= maxradius; rSqrtS++){
        if(rSqrtS == 2) sqrtSdummy->GetYaxis()->SetRangeUser(2e-7,6e-3);
        if(rSqrtS == 3) sqrtSdummy->GetYaxis()->SetRangeUser(2.5e-7,10e-3);
        if(rSqrtS == 4) sqrtSdummy->GetYaxis()->SetRangeUser(2.8e-7,15e-3);
        if(rSqrtS == 5) sqrtSdummy->GetYaxis()->SetRangeUser(3.1e-7,22e-3);
        sqrtSdummy->GetYaxis()->SetMaxDigits(2);
        SetStyleHistoTH1ForGraphs(sqrtSdummy,"","#sqrt{#it{s}} (GeV)","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,0.9,1.9);

        TGraphAsymmErrors *spectrumStat5TeV  = (TGraphAsymmErrors*)vecSpectrum5TeV.at(rSqrtS-minradius)->Clone(Form("spectrumStat5TeV_R0%i", rSqrtS));
        TGraphErrors *spectrumStat8TeV  = (TGraphErrors*)vecSpectrum8TeV.at(rSqrtS-minradius)->Clone(Form("spectrumStat8TeV_R0%i", rSqrtS));
        TGraphAsymmErrors *spectrumStat13TeV = (TGraphAsymmErrors*)vecSpectrum13TeV.at(rSqrtS-minradius)->Clone(Form("spectrumStat13TeV_R0%i", rSqrtS));
        TGraphAsymmErrors *spectrumStat2p76TeV;
        if(rSqrtS == 2 || rSqrtS == 4) spectrumStat2p76TeV = (TGraphAsymmErrors*)vecSpectrum2p76TeV.at(rSqrtS==2? 0 : 1)->Clone(Form("spectrumStat2p76TeV_R0%i", rSqrtS));

        TGraphAsymmErrors *spectrumSys5TeV  = (TGraphAsymmErrors*)vecSpectrumSys5TeV.at(rSqrtS-minradius)->Clone(Form("spectrumSys5TeV_R0%i", rSqrtS));
        TGraphErrors *spectrumSys8TeV  = (TGraphErrors*)vecSpectrumSys8TeV.at(rSqrtS-minradius)->Clone(Form("spectrumSys8TeV_R0%i", rSqrtS));
        TGraphAsymmErrors *spectrumSys13TeV = (TGraphAsymmErrors*)vecSpectrumSys13TeV.at(rSqrtS-minradius)->Clone(Form("spectrumSys13TeV_R0%i", rSqrtS));
        TGraphAsymmErrors *spectrumSys2p76TeV;
        if(rSqrtS == 2 || rSqrtS == 4) spectrumSys2p76TeV = (TGraphAsymmErrors*)vecSpectrumSys2p76TeV.at(rSqrtS==2? 0 : 1)->Clone(Form("spectrumSys2p76TeV_R0%i", rSqrtS));

        double x35stat[4] = {2.76, 5., 8., 13.};
        double y35stat[4];
        double ex35stat[4] = {0., 0., 0., 0.};
        double ey35stat[4];

        double x45stat[4] = {2.76, 5., 8., 13.};
        double y45stat[4];
        double ex45stat[4] = {0., 0., 0., 0.};
        double ey45stat[4];

        double x55stat[4] = {2.76, 5., 8., 13.};
        double y55stat[4];
        double ex55stat[4] = {0., 0., 0., 0.};
        double ey55stat[4];

        double x90stat[3] = {5., 8., 13.};
        double y90stat[3];
        double ex90stat[3] = {0., 0., 0.};
        double ey90stat[3];

        double x130stat[3] = {5., 8., 13.};
        double y130stat[3];
        double ex130stat[3] = {0., 0., 0.};
        double ey130stat[3];

        double x35Sys[4] = {2.76, 5., 8., 13.};
        double y35Sys[4];
        double ex35Sys[4] = {binWidthX, binWidthX, binWidthX, binWidthX};
        double ey35Sys[4];

        double x45Sys[4] = {2.76, 5., 8., 13.};
        double y45Sys[4];
        double ex45Sys[4] = {binWidthX, binWidthX, binWidthX, binWidthX};
        double ey45Sys[4];

        double x55Sys[4] = {2.76, 5., 8., 13.};
        double y55Sys[4];
        double ex55Sys[4] = {binWidthX, binWidthX, binWidthX, binWidthX};
        double ey55Sys[4];

        double x90Sys[3] = {5., 8., 13.};
        double y90Sys[3];
        double ex90Sys[3] = {binWidthX, binWidthX, binWidthX};
        double ey90Sys[3];

        double x130Sys[3] = {5., 8., 13.};
        double y130Sys[3];
        double ex130Sys[3] = {binWidthX, binWidthX, binWidthX};
        double ey130Sys[3];

        if(rSqrtS == 2 || rSqrtS == 4){
            double stat325temp = 0;
            double estat325temp = 0;
            double stat375temp = 0;
            double estat375temp = 0;
            for(int point = (spectrumStat2p76TeV->GetN())-1; point >= 0; point--){
                if(spectrumStat2p76TeV->GetPointX(point) == 32.5){
                    stat325temp = spectrumStat2p76TeV->GetPointY(point)*5;
                    estat325temp = spectrumStat2p76TeV->GetErrorY(point)*5;
                }
                if(spectrumStat2p76TeV->GetPointX(point) == 37.5){
                    stat375temp = spectrumStat2p76TeV->GetPointY(point)*5;
                    estat375temp = spectrumStat2p76TeV->GetErrorY(point)*5;
                }
                if(stat325temp != 0 && stat375temp != 0){
                    y35stat[0] = (stat325temp + stat375temp)/10;
                    ey35stat[0] = (estat325temp + estat375temp)/10;
                }
                if(spectrumStat2p76TeV->GetPointX(point) == 45){
                    y45stat[0] = spectrumStat2p76TeV->GetPointY(point);
                    ey45stat[0] = spectrumStat2p76TeV->GetErrorY(point);
                }
                if(spectrumStat2p76TeV->GetPointX(point) == 55){
                    y55stat[0] = spectrumStat2p76TeV->GetPointY(point);
                    ey55stat[0] = spectrumStat2p76TeV->GetErrorY(point);
                }
            }
        }else{
            y35stat[0] = 0.;
            ey35stat[0] = 0.;
            y45stat[0] = 0.;
            ey45stat[0] = 0.;
            y55stat[0] = 0.;
            ey55stat[0] = 0.;
        }

        double stat65temp5 = 0;
        double stat75temp5 = 0;
        double stat90temp5 = 0;
        double stat110temp5 = 0;
        double estat65temp5 = 0;
        double estat75temp5 = 0;
        double estat90temp5 = 0;
        double estat110temp5 = 0;
        for(int point = (spectrumStat5TeV->GetN())-1; point >= 0; point--){
            if(spectrumStat5TeV->GetPointX(point) == 35){
                y35stat[1] = spectrumStat5TeV->GetPointY(point);
                ey35stat[1] = spectrumStat5TeV->GetErrorY(point);
            }
            if(spectrumStat5TeV->GetPointX(point) == 45){
                y45stat[1] = spectrumStat5TeV->GetPointY(point);
                ey45stat[1] = spectrumStat5TeV->GetErrorY(point);
            }
            if(spectrumStat5TeV->GetPointX(point) == 55){
                y55stat[1] = spectrumStat5TeV->GetPointY(point);
                ey55stat[1] = spectrumStat5TeV->GetErrorY(point);
            }
            if(spectrumStat5TeV->GetPointX(point) == 65){
                stat65temp5 = spectrumStat5TeV->GetPointY(point)*10;
                estat65temp5 = spectrumStat5TeV->GetErrorY(point)*10;
            }
            if(spectrumStat5TeV->GetPointX(point) == 75){
                stat75temp5 = spectrumStat5TeV->GetPointY(point)*10;
                estat75temp5 = spectrumStat5TeV->GetErrorY(point)*10;
            }
            if(spectrumStat5TeV->GetPointX(point) == 90){
                stat90temp5 = spectrumStat5TeV->GetPointY(point)*20;
                estat90temp5 = spectrumStat5TeV->GetErrorY(point)*20;
            }
            if(spectrumStat5TeV->GetPointX(point) == 110){
                stat110temp5 = spectrumStat5TeV->GetPointY(point)*20;
                estat110temp5 = spectrumStat5TeV->GetErrorY(point)*20;
            }
            if(stat65temp5 != 0 && stat75temp5 != 0 && stat90temp5 != 0 && stat110temp5 != 0){
                y90stat[0] = (stat65temp5 + stat75temp5 + stat90temp5 + stat110temp5)/60;
                ey90stat[0] = (estat65temp5 + estat75temp5 + estat90temp5 + estat110temp5)/60;
            }
            if(spectrumStat5TeV->GetPointX(point) == 130){
                y130stat[0] = spectrumStat5TeV->GetPointY(point);
                ey130stat[0] = spectrumStat5TeV->GetErrorY(point);
            }
        }

        double stat675temp = 0;
        double stat825temp = 0;
        double stat975temp = 0;
        double stat1125temp = 0;
        double estat675temp = 0;
        double estat825temp = 0;
        double estat975temp = 0;
        double estat1125temp = 0;
        for(int point = (spectrumStat8TeV->GetN())-1; point >= 0; point--){
            if(spectrumStat8TeV->GetPointX(point) == 35){
                y35stat[2] = spectrumStat8TeV->GetPointY(point);
                ey35stat[2] = spectrumStat8TeV->GetErrorY(point);
            }
            if(spectrumStat8TeV->GetPointX(point) == 45){
                y45stat[2] = spectrumStat8TeV->GetPointY(point);
                ey45stat[2] = spectrumStat8TeV->GetErrorY(point);
            }
            if(spectrumStat8TeV->GetPointX(point) == 55){
                y55stat[2] = spectrumStat8TeV->GetPointY(point);
                ey55stat[2] = spectrumStat8TeV->GetErrorY(point);
            }
            if(spectrumStat8TeV->GetPointX(point) == 67.5){
                stat675temp = spectrumStat8TeV->GetPointY(point)*15;
                estat675temp = spectrumStat8TeV->GetErrorY(point)*15;
            }
            if(spectrumStat8TeV->GetPointX(point) == 82.5){
                stat825temp = spectrumStat8TeV->GetPointY(point)*15;
                estat825temp = spectrumStat8TeV->GetErrorY(point)*15;
            }
            if(spectrumStat8TeV->GetPointX(point) == 97.5){
                stat975temp = spectrumStat8TeV->GetPointY(point)*15;
                estat975temp = spectrumStat8TeV->GetErrorY(point)*15;
            }
            if(spectrumStat8TeV->GetPointX(point) == 112.5){
                stat1125temp = spectrumStat8TeV->GetPointY(point)*15;
                estat1125temp = spectrumStat8TeV->GetErrorY(point)*15;
            }
            if(stat675temp != 0 && stat825temp != 0 && stat975temp != 0 && stat1125temp != 0){
                y90stat[1] = (stat675temp + stat825temp + stat975temp + stat1125temp)/60;
                ey90stat[1] = (estat675temp + estat825temp + estat975temp + estat1125temp)/60;
            }
            if(spectrumStat8TeV->GetPointX(point) == 130){
                y130stat[1] = spectrumStat8TeV->GetPointY(point);
                ey130stat[1] = spectrumStat8TeV->GetErrorY(point);
            }
        }

        double stat65temp13 = 0;
        double stat75temp13 = 0;
        double stat85temp13 = 0;
        double stat95temp13 = 0;
        double stat105temp13 = 0;
        double stat115temp13 = 0;
        double estat65temp13 = 0;
        double estat75temp13 = 0;
        double estat85temp13 = 0;
        double estat95temp13 = 0;
        double estat105temp13 = 0;
        double estat115temp13 = 0;
        double stat125temp = 0;
        double estat125temp = 0;
        double stat135temp = 0;
        double estat135temp = 0;
        for(int point = (spectrumStat13TeV->GetN())-1; point >= 0; point--){
            if(spectrumStat13TeV->GetPointX(point) == 35){
                y35stat[3] = spectrumStat13TeV->GetPointY(point);
                ey35stat[3] = spectrumStat13TeV->GetErrorY(point);
            }
            if(spectrumStat13TeV->GetPointX(point) == 45){
                y45stat[3] = spectrumStat13TeV->GetPointY(point);
                ey45stat[3] = spectrumStat13TeV->GetErrorY(point);
            }
            if(spectrumStat13TeV->GetPointX(point) == 55){
                y55stat[3] = spectrumStat13TeV->GetPointY(point);
                ey55stat[3] = spectrumStat13TeV->GetErrorY(point);
            }
            if(spectrumStat13TeV->GetPointX(point) == 65){
                stat65temp13 = spectrumStat13TeV->GetPointY(point)*10;
                estat65temp13 = spectrumStat13TeV->GetErrorY(point)*10;
            }
            if(spectrumStat13TeV->GetPointX(point) == 75){
                stat75temp13 = spectrumStat13TeV->GetPointY(point)*10;
                estat75temp13 = spectrumStat13TeV->GetErrorY(point)*10;
            }
            if(spectrumStat13TeV->GetPointX(point) == 85){
                stat85temp13 = spectrumStat13TeV->GetPointY(point)*10;
                estat85temp13 = spectrumStat13TeV->GetErrorY(point)*10;
            }
            if(spectrumStat13TeV->GetPointX(point) == 95){
                stat95temp13 = spectrumStat13TeV->GetPointY(point)*10;
                estat95temp13 = spectrumStat13TeV->GetErrorY(point)*10;
            }
            if(spectrumStat13TeV->GetPointX(point) == 105){
                stat105temp13 = spectrumStat13TeV->GetPointY(point)*10;
                estat105temp13 = spectrumStat13TeV->GetErrorY(point)*10;
            }
            if(spectrumStat13TeV->GetPointX(point) == 115){
                stat115temp13 = spectrumStat13TeV->GetPointY(point)*10;
                estat115temp13 = spectrumStat13TeV->GetErrorY(point)*10;
            }
            if(stat65temp13 != 0 && stat75temp13 != 0 && stat85temp13 != 0 && stat95temp13 != 0 && stat105temp13 != 0 && stat115temp13 != 0){
                y90stat[2] = (stat65temp13 + stat75temp13 + stat85temp13 + stat95temp13 + stat105temp13 + stat115temp13)/60;
                ey90stat[2] = (estat65temp13 + estat75temp13 + estat85temp13 + estat95temp13 + estat105temp13 + estat115temp13)/60;
            }
            if(spectrumStat13TeV->GetPointX(point) == 125){
                stat125temp = spectrumStat13TeV->GetPointY(point)*10;
                estat125temp = spectrumStat13TeV->GetErrorY(point)*10;
            }
            if(spectrumStat13TeV->GetPointX(point) == 135){
                stat135temp = spectrumStat13TeV->GetPointY(point)*10;
                estat135temp = spectrumStat13TeV->GetErrorY(point)*10;
            }
            if(stat125temp != 0 && stat135temp != 0){
                y130stat[2] = (stat125temp + stat135temp)/20;
                ey130stat[2] = (estat125temp + estat135temp)/20;
            }
        }

        ///////////////////////

        if(rSqrtS == 2 || rSqrtS == 4){
            double sys325temp = 0;
            double esys325temp = 0;
            double sys375temp = 0;
            double esys375temp = 0;
            double esys350temp = 0;
            for(int point = (spectrumSys2p76TeV->GetN())-1; point >= 0; point--){
                if(spectrumSys2p76TeV->GetPointX(point) == 32.5){
                    sys325temp = spectrumSys2p76TeV->GetPointY(point)*5;
                    esys325temp = ((spectrumSys2p76TeV->GetErrorY(point))/sys325temp)*5;
                }
                if(spectrumSys2p76TeV->GetPointX(point) == 37.5){
                    sys375temp = spectrumSys2p76TeV->GetPointY(point)*5;
                    esys375temp = ((spectrumSys2p76TeV->GetErrorY(point))/sys375temp)*5;
                }
                if(sys325temp != 0 && sys375temp != 0){
                    if(esys325temp > esys375temp) esys350temp = esys325temp;
                    else esys350temp = esys375temp;
                    y35Sys[0] = (sys325temp + sys375temp)/10;
                    ey35Sys[0] = (2*esys350temp*y35Sys[0])/10;
                }
                if(spectrumSys2p76TeV->GetPointX(point) == 45){
                    y45Sys[0] = spectrumSys2p76TeV->GetPointY(point);
                    ey45Sys[0] = spectrumSys2p76TeV->GetErrorY(point);
                }
                if(spectrumSys2p76TeV->GetPointX(point) == 55){
                    y55Sys[0] = spectrumSys2p76TeV->GetPointY(point);
                    ey55Sys[0] = spectrumSys2p76TeV->GetErrorY(point);
                }
            }
        }else{
            y35Sys[0] = 0.;
            ey35Sys[0] = 0.;
            y45Sys[0] = 0.;
            ey45Sys[0] = 0.;
            y55Sys[0] = 0.;
            ey55Sys[0] = 0.;
        }

        double sys65temp5 = 0;
        double sys75temp5 = 0;
        double sys90temp5 = 0;
        double sys110temp5 = 0;
        double esys65temp5 = 0;
        double esys75temp5 = 0;
        double esys90temp5 = 0;
        double esys110temp5 = 0;
        double esys90temp5total = 0;
        for(int point = (spectrumSys5TeV->GetN())-1; point >= 0; point--){
            if(spectrumSys5TeV->GetPointX(point) == 35){
                y35Sys[1] = spectrumSys5TeV->GetPointY(point);
                ey35Sys[1] = spectrumSys5TeV->GetErrorY(point);
            }
            if(spectrumSys5TeV->GetPointX(point) == 45){
                y45Sys[1] = spectrumSys5TeV->GetPointY(point);
                ey45Sys[1] = spectrumSys5TeV->GetErrorY(point);
            }
            if(spectrumSys5TeV->GetPointX(point) == 55){
                y55Sys[1] = spectrumSys5TeV->GetPointY(point);
                ey55Sys[1] = spectrumSys5TeV->GetErrorY(point);
            }
            if(spectrumSys5TeV->GetPointX(point) == 65){
                sys65temp5 = spectrumSys5TeV->GetPointY(point)*10;
                esys65temp5 = ((spectrumSys5TeV->GetErrorY(point))/sys65temp5)*10;
            }
            if(spectrumSys5TeV->GetPointX(point) == 75){
                sys75temp5 = spectrumSys5TeV->GetPointY(point)*10;
                esys75temp5 = ((spectrumSys5TeV->GetErrorY(point))/sys75temp5)*10;
            }
            if(spectrumSys5TeV->GetPointX(point) == 90){
                sys90temp5 = spectrumSys5TeV->GetPointY(point)*20;
                esys90temp5 = ((spectrumSys5TeV->GetErrorY(point))/sys90temp5)*10;
            }
            if(spectrumSys5TeV->GetPointX(point) == 110){
                sys110temp5 = spectrumSys5TeV->GetPointY(point)*20;
                esys110temp5 = ((spectrumSys5TeV->GetErrorY(point))/sys110temp5)*10;
            }
            if(sys65temp5 != 0 && sys75temp5 != 0 && sys90temp5 != 0 && sys110temp5 != 0){
                if(esys65temp5 > esys75temp5) esys90temp5total = esys65temp5;
                else if(esys90temp5 > esys90temp5total) esys90temp5total = esys90temp5;
                else if(esys110temp5 > esys90temp5total) esys90temp5total = esys110temp5;
                else esys90temp5total = esys75temp5;
                y90Sys[0] = (sys65temp5 + sys75temp5 + sys90temp5 + sys110temp5)/60;
                ey90Sys[0] = (4*esys90temp5total*y90Sys[0])/60;
            }
            if(spectrumSys5TeV->GetPointX(point) == 130){
                y130Sys[0] = spectrumSys5TeV->GetPointY(point);
                ey130Sys[0] = spectrumSys5TeV->GetErrorY(point);
            }
        }

        double sys675temp = 0;
        double sys825temp = 0;
        double sys975temp = 0;
        double sys1125temp = 0;
        double esys675temp = 0;
        double esys825temp = 0;
        double esys975temp = 0;
        double esys1125temp = 0;
        double esys90temp8 = 0;
        for(int point = (spectrumSys8TeV->GetN())-1; point >= 0; point--){
            if(spectrumSys8TeV->GetPointX(point) == 35){
                y35Sys[2] = spectrumSys8TeV->GetPointY(point);
                ey35Sys[2] = spectrumSys8TeV->GetErrorY(point);
            }
            if(spectrumSys8TeV->GetPointX(point) == 45){
                y45Sys[2] = spectrumSys8TeV->GetPointY(point);
                ey45Sys[2] = spectrumSys8TeV->GetErrorY(point);
            }
            if(spectrumSys8TeV->GetPointX(point) == 55){
                y55Sys[2] = spectrumSys8TeV->GetPointY(point);
                ey55Sys[2] = spectrumSys8TeV->GetErrorY(point);
            }
            if(spectrumSys8TeV->GetPointX(point) == 67.5){
                sys675temp = spectrumSys8TeV->GetPointY(point)*15;
                esys675temp = ((spectrumSys8TeV->GetErrorY(point))/sys675temp)*15;
            }
            if(spectrumSys8TeV->GetPointX(point) == 82.5){
                sys825temp = spectrumSys8TeV->GetPointY(point)*15;
                esys825temp = ((spectrumSys8TeV->GetErrorY(point))/sys825temp)*15;
            }
            if(spectrumSys8TeV->GetPointX(point) == 97.5){
                sys975temp = spectrumSys8TeV->GetPointY(point)*15;
                esys975temp = ((spectrumSys8TeV->GetErrorY(point))/sys975temp)*15;
            }
            if(spectrumSys8TeV->GetPointX(point) == 112.5){
                sys1125temp = spectrumSys8TeV->GetPointY(point)*15;
                esys1125temp = ((spectrumSys8TeV->GetErrorY(point))/sys1125temp)*15;
            }
            if(sys675temp != 0 && sys825temp != 0 && sys975temp != 0 && sys1125temp != 0){
                if(esys675temp > esys825temp) esys90temp8 = esys675temp;
                else if(esys975temp > esys90temp8) esys90temp8 = esys975temp;
                else if(esys1125temp > esys90temp8) esys90temp8 = esys1125temp;
                else esys90temp8 = esys825temp;
                y90Sys[1] = (sys675temp + sys825temp + sys975temp + sys1125temp)/60;
                ey90Sys[1] = (4*esys90temp8*y90Sys[1])/60;
            }
            if(spectrumSys8TeV->GetPointX(point) == 130){
                y130Sys[1] = spectrumSys8TeV->GetPointY(point);
                ey130Sys[1] = spectrumSys8TeV->GetErrorY(point);
            }
        }

        double sys65temp13 = 0;
        double sys75temp13 = 0;
        double sys85temp13 = 0;
        double sys95temp13 = 0;
        double sys105temp13 = 0;
        double sys115temp13 = 0;
        double esys65temp13 = 0;
        double esys75temp13 = 0;
        double esys85temp13 = 0;
        double esys95temp13 = 0;
        double esys105temp13 = 0;
        double esys115temp13 = 0;
        double esys90temp13 = 0;
        double sys125temp = 0;
        double esys125temp = 0;
        double sys135temp = 0;
        double esys135temp = 0;
        double esys130temp = 0;
        for(int point = (spectrumSys13TeV->GetN())-1; point >= 0; point--){
            if(spectrumSys13TeV->GetPointX(point) == 35){
                y35Sys[3] = spectrumSys13TeV->GetPointY(point);
                ey35Sys[3] = spectrumSys13TeV->GetErrorY(point);
            }
            if(spectrumSys13TeV->GetPointX(point) == 45){
                y45Sys[3] = spectrumSys13TeV->GetPointY(point);
                ey45Sys[3] = spectrumSys13TeV->GetErrorY(point);
            }
            if(spectrumSys13TeV->GetPointX(point) == 55){
                y55Sys[3] = spectrumSys13TeV->GetPointY(point);
                ey55Sys[3] = spectrumSys13TeV->GetErrorY(point);
            }
            if(spectrumSys13TeV->GetPointX(point) == 65){
                sys65temp13 = spectrumSys13TeV->GetPointY(point)*10;
                esys65temp13 = ((spectrumSys13TeV->GetErrorY(point))/sys65temp13)*10;
            }
            if(spectrumSys13TeV->GetPointX(point) == 75){
                sys75temp13 = spectrumSys13TeV->GetPointY(point)*10;
                esys75temp13 = ((spectrumSys13TeV->GetErrorY(point))/sys75temp13)*10;
            }
            if(spectrumSys13TeV->GetPointX(point) == 85){
                sys85temp13 = spectrumSys13TeV->GetPointY(point)*10;
                esys85temp13 = ((spectrumSys13TeV->GetErrorY(point))/sys85temp13)*10;
            }
            if(spectrumSys13TeV->GetPointX(point) == 95){
                sys95temp13 = spectrumSys13TeV->GetPointY(point)*10;
                esys95temp13 = ((spectrumSys13TeV->GetErrorY(point))/sys95temp13)*10;
            }
            if(spectrumSys13TeV->GetPointX(point) == 105){
                sys105temp13 = spectrumSys13TeV->GetPointY(point)*10;
                esys105temp13 = ((spectrumSys13TeV->GetErrorY(point))/sys105temp13)*10;
            }
            if(spectrumSys13TeV->GetPointX(point) == 115){
                sys115temp13 = spectrumSys13TeV->GetPointY(point)*10;
                esys115temp13 = ((spectrumSys13TeV->GetErrorY(point))/sys115temp13)*10;
            }
            if(sys65temp13 != 0 && sys75temp13 != 0 && sys85temp13 != 0 && sys95temp13 != 0 && sys105temp13 != 0 && sys115temp13 != 0){
                if(esys65temp13 > esys75temp13) esys90temp13 = esys65temp13;
                else if(esys85temp13 > esys90temp13) esys90temp13 = esys85temp13;
                else if(esys95temp13 > esys90temp13) esys90temp13 = esys95temp13;
                else if(esys105temp13 > esys90temp13) esys90temp13 = esys105temp13;
                else if(esys115temp13 > esys90temp13) esys90temp13 = esys115temp13;
                else esys90temp13 = esys75temp13;
                y90Sys[2] = (sys65temp13 + sys75temp13 + sys85temp13 + sys95temp13 + sys105temp13 + sys115temp13)/60;
                ey90Sys[2] = (6*esys90temp13*y90Sys[2])/60;
            }
            if(spectrumSys13TeV->GetPointX(point) == 125){
                sys125temp = spectrumSys13TeV->GetPointY(point)*10;
                esys125temp = ((spectrumSys13TeV->GetErrorY(point))/sys125temp)*10;
            }
            if(spectrumSys13TeV->GetPointX(point) == 135){
                sys135temp = spectrumSys13TeV->GetPointY(point)*10;
                esys135temp = ((spectrumSys13TeV->GetErrorY(point))/sys135temp)*10;
            }
            if(sys125temp != 0 && sys135temp != 0){
                if(esys125temp > esys135temp) esys130temp = esys125temp;
                else esys130temp = esys135temp;
                y130Sys[2] = (sys125temp + sys135temp)/20;
                ey130Sys[2] = (2*esys130temp*y130Sys[2])/20;
            }
        }

        TGraphErrors *sqrtSComp35stat  = new TGraphErrors(4,x35stat,y35stat,ex35stat,ey35stat);
        TGraphErrors *sqrtSComp45stat  = new TGraphErrors(4,x45stat,y45stat,ex45stat,ey45stat);
        TGraphErrors *sqrtSComp55stat  = new TGraphErrors(4,x55stat,y55stat,ex55stat,ey55stat);
        TGraphErrors *sqrtSComp90stat  = new TGraphErrors(3,x90stat,y90stat,ex90stat,ey90stat);
        TGraphErrors *sqrtSComp130stat = new TGraphErrors(3,x130stat,y130stat,ex130stat,ey130stat);

        TGraphErrors *sqrtSComp35Sys  = new TGraphErrors(4,x35Sys,y35Sys,ex35Sys,ey35Sys);
        TGraphErrors *sqrtSComp45Sys  = new TGraphErrors(4,x45Sys,y45Sys,ex45Sys,ey45Sys);
        TGraphErrors *sqrtSComp55Sys  = new TGraphErrors(4,x55Sys,y55Sys,ex55Sys,ey55Sys);
        TGraphErrors *sqrtSComp90Sys  = new TGraphErrors(3,x90Sys,y90Sys,ex90Sys,ey90Sys);
        TGraphErrors *sqrtSComp130Sys = new TGraphErrors(3,x130Sys,y130Sys,ex130Sys,ey130Sys);

        if(rSqrtS == 3 || rSqrtS == 5){
            sqrtSComp35stat->RemovePoint(0);
            sqrtSComp45stat->RemovePoint(0);
            sqrtSComp55stat->RemovePoint(0);

            sqrtSComp35Sys->RemovePoint(0);
            sqrtSComp45Sys->RemovePoint(0);
            sqrtSComp55Sys->RemovePoint(0);
        }

        sqrtSComp35stat->SetMarkerStyle(stylesfilled[0]);
        sqrtSComp35stat->SetMarkerSize(sizesfilled[0]);
        sqrtSComp35stat->SetMarkerColor(colorsSqrtS[0]);
        sqrtSComp35stat->SetLineColor(colorsSqrtS[0]);
        sqrtSComp35stat->SetLineWidth(3);
        sqrtSComp35stat->SetLineStyle(1);

        sqrtSComp45stat->SetMarkerStyle(stylesfilled[1]);
        sqrtSComp45stat->SetMarkerSize(sizesfilled[1]);
        sqrtSComp45stat->SetMarkerColor(colorsSqrtS[1]);
        sqrtSComp45stat->SetLineColor(colorsSqrtS[1]);
        sqrtSComp45stat->SetLineWidth(3);
        sqrtSComp45stat->SetLineStyle(1);

        sqrtSComp55stat->SetMarkerStyle(stylesfilled[2]);
        sqrtSComp55stat->SetMarkerSize(sizesfilled[2]);
        sqrtSComp55stat->SetMarkerColor(colorsSqrtS[2]);
        sqrtSComp55stat->SetLineColor(colorsSqrtS[2]);
        sqrtSComp55stat->SetLineWidth(3);
        sqrtSComp55stat->SetLineStyle(1);

        sqrtSComp90stat->SetMarkerStyle(stylesfilled[3]);
        sqrtSComp90stat->SetMarkerSize(sizesfilled[3]);
        sqrtSComp90stat->SetMarkerColor(colorsSqrtS[3]);
        sqrtSComp90stat->SetLineColor(colorsSqrtS[3]);
        sqrtSComp90stat->SetLineWidth(3);
        sqrtSComp90stat->SetLineStyle(1);

        sqrtSComp130stat->SetMarkerStyle(stylesfilled[4]);
        sqrtSComp130stat->SetMarkerSize(sizesfilled[4]);
        sqrtSComp130stat->SetMarkerColor(colorsSqrtS[4]);
        sqrtSComp130stat->SetLineColor(colorsSqrtS[4]);
        sqrtSComp130stat->SetLineWidth(3);
        sqrtSComp130stat->SetLineStyle(1);

        sqrtSComp35Sys->SetFillColorAlpha(colorsSqrtS[0],alphasval);
        sqrtSComp35Sys->SetLineColor(colorsSqrtS[0]);
        sqrtSComp35Sys->SetFillStyle(1);

        sqrtSComp45Sys->SetFillColorAlpha(colorsSqrtS[1],alphasval);
        sqrtSComp45Sys->SetLineColor(colorsSqrtS[1]);
        sqrtSComp45Sys->SetFillStyle(1);

        sqrtSComp55Sys->SetFillColorAlpha(colorsSqrtS[2],alphasval);
        sqrtSComp55Sys->SetLineColor(colorsSqrtS[2]);
        sqrtSComp55Sys->SetFillStyle(1);

        sqrtSComp90Sys->SetFillColorAlpha(colorsSqrtS[3],alphasval);
        sqrtSComp90Sys->SetLineColor(colorsSqrtS[3]);
        sqrtSComp90Sys->SetFillStyle(1);

        sqrtSComp130Sys->SetFillColorAlpha(colorsSqrtS[4],alphasval);
        sqrtSComp130Sys->SetLineColor(colorsSqrtS[4]);
        sqrtSComp130Sys->SetFillStyle(1);

        TF1 *PLFit35 = NULL;
        TF1 *PLFit45 = NULL;
        TF1 *PLFit55 = NULL;
        TF1 *PLFit90 = NULL;
        TF1 *PLFit130 = NULL;
        if(rSqrtS == 2 || rSqrtS == 4){
            PLFit35 = new TF1("PLFit35","[0]*TMath::Power(x,[1])",2.76,13.);
            PLFit35->SetParLimits(1,1.3,2.1);
            PLFit45 = new TF1("PLFit45","[0]*TMath::Power(x,[1])",2.76,13.);
            PLFit45->SetParLimits(1,1.3,2.1);
            PLFit55 = new TF1("PLFit55","[0]*TMath::Power(x,[1])",2.76,13.);
            PLFit55->SetParLimits(1,1.3,2.1);
            PLFit90 = new TF1("PLFit90","[0]*TMath::Power(x,[1])",5.,13.);
            PLFit90->SetParLimits(1,1.3,2.1);
            PLFit130 = new TF1("PLFit130","[0]*TMath::Power(x,[1])",5.,13.);
            PLFit130->SetParLimits(1,1.3,2.1);
            PLFit35->SetLineColor(colorsSqrtS[0]);
            PLFit45->SetLineColor(colorsSqrtS[1]);
            PLFit55->SetLineColor(colorsSqrtS[2]);
            PLFit90->SetLineColor(colorsSqrtS[3]);
            PLFit130->SetLineColor(colorsSqrtS[4]);
            sqrtSComp35stat->Fit(PLFit35, "QRMEX+", "", 2.76, 13.);
            sqrtSComp45stat->Fit(PLFit45, "QRMEX+", "", 2.76, 13.);
            sqrtSComp55stat->Fit(PLFit55, "QRMEX+", "", 2.76, 13.);
            sqrtSComp90stat->Fit(PLFit90, "QRMEX+", "", 5., 13.);
            sqrtSComp130stat->Fit(PLFit130, "QRMEX+", "", 5., 13.);
        }else{
            PLFit35 = new TF1("PLFit35","[0]*TMath::Power(x,[1])",5.,13.);
            PLFit35->SetParLimits(1,1.3,2.1);
            PLFit45 = new TF1("PLFit45","[0]*TMath::Power(x,[1])",5.,13.);
            PLFit45->SetParLimits(1,1.3,2.1);
            PLFit55 = new TF1("PLFit55","[0]*TMath::Power(x,[1])",5.,13.);
            PLFit55->SetParLimits(1,1.3,2.1);
            PLFit90 = new TF1("PLFit90","[0]*TMath::Power(x,[1])",5.,13.);
            PLFit90->SetParLimits(1,1.3,2.1);
            PLFit130 = new TF1("PLFit130","[0]*TMath::Power(x,[1])",5.,13.);
            PLFit130->SetParLimits(1,1.3,2.1);
            PLFit35->SetLineColor(colorsSqrtS[0]);
            PLFit45->SetLineColor(colorsSqrtS[1]);
            PLFit55->SetLineColor(colorsSqrtS[2]);
            PLFit90->SetLineColor(colorsSqrtS[3]);
            PLFit130->SetLineColor(colorsSqrtS[4]);
            sqrtSComp35stat->Fit(PLFit35, "QRMEX+", "", 5., 13.);
            sqrtSComp45stat->Fit(PLFit45, "QRMEX+", "", 5., 13.);
            sqrtSComp55stat->Fit(PLFit55, "QRMEX+", "", 5., 13.);
            sqrtSComp90stat->Fit(PLFit90, "QRMEX+", "", 5., 13.);
            sqrtSComp130stat->Fit(PLFit130, "QRMEX+", "", 5., 13.);
        }

        legendSqrtS->AddEntry(sqrtSComp35stat, Form("30 - 40 GeV/#it{c}, n = %1.2f #pm %1.2f", PLFit35->GetParameter(1), PLFit35->GetParError(1)), "p");
        legendSqrtS->AddEntry(sqrtSComp45stat, Form("40 - 50 GeV/#it{c}, n = %1.2f #pm %1.2f", PLFit45->GetParameter(1), PLFit45->GetParError(1)), "p");
        legendSqrtS->AddEntry(sqrtSComp55stat, Form("50 - 60 GeV/#it{c}, n = %1.2f #pm %1.2f", PLFit55->GetParameter(1), PLFit55->GetParError(1)), "p");
        legendSqrtS->AddEntry(sqrtSComp90stat, Form("60 - 120 GeV/#it{c}, n = %1.2f #pm %1.2f", PLFit90->GetParameter(1), PLFit90->GetParError(1)), "p");
        legendSqrtS->AddEntry(sqrtSComp130stat, Form("120 - 140 GeV/#it{c}, n = %1.2f #pm %1.2f", PLFit130->GetParameter(1), PLFit130->GetParError(1)), "p");

        //legendFitSqrtS->AddEntry(PLFit35, Form("Fit = (%.2e #pm %1.0e)x^(%1.3f #pm %1.0e)", PLFit35->GetParameter(0), PLFit35->GetParError(0), PLFit35->GetParameter(1), PLFit35->GetParError(1)), "l");
        //legendFitSqrtS->AddEntry(PLFit45, Form("Fit = (%.2e #pm %1.0e)x^(%1.3f #pm %1.0e)", PLFit45->GetParameter(0), PLFit45->GetParError(0), PLFit45->GetParameter(1), PLFit45->GetParError(1)), "l");
        //legendFitSqrtS->AddEntry(PLFit55, Form("Fit = (%.2e #pm %1.0e)x^(%1.3f #pm %1.0e)", PLFit55->GetParameter(0), PLFit55->GetParError(0), PLFit55->GetParameter(1), PLFit55->GetParError(1)), "l");
        //legendFitSqrtS->AddEntry(PLFit130, Form("Fit = (%.2e #pm %1.0e)x^(%1.3f #pm %1.0e)", PLFit130->GetParameter(0), PLFit130->GetParError(0), PLFit130->GetParameter(1), PLFit130->GetParError(1)), "l");


        legendErrorKeySqrtS->AddEntry(sqrtSComp35Sys, "Syst. Uncertainty", "f");
        legendErrorKeySqrtS->AddEntry(sqrtSComp35stat, "Stat. Uncertainty", "e");
        legendErrorKeySqrtS->AddEntry(sqrtSComp35stat, "f(x) = ax^{n}", "l");

        sqrtSdummy->Draw("axis");
        sqrtSComp35Sys->Draw("e2,same");
        sqrtSComp45Sys->Draw("e2,same");
        sqrtSComp55Sys->Draw("e2,same");
        sqrtSComp90Sys->Draw("e2,same");
        sqrtSComp130Sys->Draw("e2,same");
        sqrtSComp35stat->Draw("p,e1,same");
        sqrtSComp45stat->Draw("p,e1,same");
        sqrtSComp55stat->Draw("p,e1,same");
        sqrtSComp90stat->Draw("p,e1,same");
        sqrtSComp130stat->Draw("p,e1,same");
        legendSqrtS->Draw("same");
        legendErrorKeySqrtS->Draw("same");
        legendFitSqrtS->Draw("same");

        drawLatexAdd("ALICE Preliminary",0.93,0.2, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.%i", rSqrtS),0.93,0.17, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("#it{p}_{T}^{ch} > 0.15 GeV/#it{c}, #it{E}^{cl} > 0.3 GeV",0.93,0.14, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("|#it{#eta}^{tr}| < 0.7, |#it{#eta}^{cl}| < 0.7, |#it{#eta}^{jet}| < 0.7 - #it{R}",0.93,0.11, textSize,kFALSE, kFALSE, true);

        cSqrtS->SaveAs(Form("%s/EnergyComparisons/sqrtSComp_R0%i.%s",output.Data(),rSqrtS,fileType.Data()));

        legendSqrtS->Clear();
        legendErrorKeySqrtS->Clear();
        legendFitSqrtS->Clear();
    }

}
