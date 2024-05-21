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

void getNewProductionStats(TString runwisedir){
    void *dirp = gSystem->OpenDirectory(runwisedir);
    const char *subdir;
    vector<TString> runs;
    vector<int> stats;
    while ((subdir = (char*)gSystem->GetDirEntry(dirp))) {
        if (TString(subdir) == "." || TString(subdir) == "..") continue;
        TString subfolderPath = runwisedir + "/" + TString(subdir);
        TFile *file = TFile::Open(subfolderPath + "/AnalysisResults.root");
        TString triggerDirName = "JetSpectrum_FullJets_R02_INT7_default";
        TDirectory *triggerDir = (TDirectory*)file->Get(triggerDirName.Data());
        TList *list = (TList*)triggerDir->Get(triggerDirName.Data());
        TH1D *hClusterCounter = (TH1D*)list->FindObject("hClusterCounter");
        int nClusters = hClusterCounter->GetBinContent(1);
        runs.push_back(TString(subdir));
        stats.push_back(nClusters);   
        file->Close();
    }
    gSystem->FreeDirectory(dirp);

    // Get sum of elements in stats
    int sum = 0;
    for (int i = 0; i < stats.size(); i++){
        sum += stats[i];
    }

    // Get fraction of each run and print
    ofstream myfile;
    myfile.open (Form("%s/newProductionStats.txt",runwisedir.Data()));
    for (int i = 0; i < stats.size(); i++){
        myfile << "Run: " << runs[i] << " ; % Total Stats: " << ((double)stats[i]/(double)sum)*100 <<"%" << endl;
    }


}