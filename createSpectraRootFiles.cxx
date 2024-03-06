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

void createSpectraRootFiles(TString HEPtextfile, TString output, int radius)
{
    Double_t textSize     = 0.03;
    int styles[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizes[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};
    double alphasval = 0.25;
    Color_t colors[13]      = {kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    vector<TString> files;
    vector<int> cols;
    vector<TString> systems;
    vector<TGraphErrors*> vecSpectrum;
   
    // Read HEPtextfile
    ifstream txtfile(HEPtextfile);
    if(!txtfile){ cout << "Text file " << HEPtextfile << " not found!" << endl; return; }
    TString hepfile, system;
    int ncols;

    while(txtfile >> hepfile >> ncols >> system){
        cout << "Reading " << hepfile << endl;
        cout << "Columns: " << ncols << endl;
        cout << "System: " << system << endl;
        
        files.push_back(hepfile);
        cols.push_back(ncols);
        systems.push_back(system);
    }

    TFile *fOutput = new TFile(Form("%s/cms_atlas_rootfiles.root",output.Data()),"RECREATE");
    for(int system = 0; system < systems.size(); system++){
        fOutput->mkdir(systems.at(system));
    }

    gSystem->Exec("mkdir -p "+output+"/EnergyComparisons");

    for(int file = 0; file < files.size(); file++){
        cout << "Processing " << systems.at(file) << endl;
        cout << "File: " << files.at(file) << endl;
        cout << "Columns: " << cols.at(file) << endl;

        TGraphAsymmErrors *spectrum = ParseHEPData(files.at(file).Data(), cols.at(file), 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
        for(int point = 0; point < spectrum->GetN(); point++){
            double staterror = spectrum->GetErrorY(point);
            spectrum->SetPointEYhigh(point,staterror);
            spectrum->SetPointEYlow(point,staterror);
            spectrum->SetPointEXhigh(point,0);
            spectrum->SetPointEXlow(point,0);
        }

        spectrum->SetMarkerStyle(styles[file]);
        spectrum->SetMarkerSize(sizes[file]);
        spectrum->SetMarkerColor(colors[file]);
        spectrum->SetLineColor(colors[file]);

        fOutput->cd(systems.at(file));
        auto basedir = static_cast<TDirectory *>(gDirectory);
        basedir->mkdir(Form("R0%i",radius));
        basedir->cd(Form("R0%i",radius));

        spectrum->Write("spectrumStat");
    }

}
