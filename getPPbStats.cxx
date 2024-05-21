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


void getPPbStats(TString folderPath, TString outputpath, TString period, TString runlist)
{
    vector<TString> triggers{"INT7","EJ2","EJ1"};
    int radius = 2;
    TString jetType = "Full";
    double totalClustersINT7 = 0;
    double totalClustersEJ2 = 0;
    double totalClustersEJ1 = 0;

    // Create output directory if it doesn't exist.
    if(!gSystem->OpenDirectory(outputpath.Data())) gSystem->mkdir(outputpath.Data());

    // Loop over all subdirectories in folderPath. Open the TFile in the subdirectory.
    // Loop over all triggers and get the TDirectory for the trigger.
    // Get the TList from the TDirectory.
    // Get hClusterCounter from the TList.
    // Get the number of clusters in bin 1 of hClusterCounter.
    // Print the number of clusters for the trigger.
    // Close the TFile.

    ofstream outfile;
    outfile.open(Form("%s/clusterStats_%s_%s.csv",outputpath.Data(),period.Data(),runlist.Data()));
    outfile << "Run,INT7,EJ2,EJ1" << endl;
    void *dirp = gSystem->OpenDirectory(folderPath);
    const char *subdir;
    while ((subdir = (char*)gSystem->GetDirEntry(dirp))) {
        if (TString(subdir) == "." || TString(subdir) == "..") continue;
        TString subfolderPath = folderPath + "/" + TString(subdir);
        TFile *file = TFile::Open(subfolderPath + "/AnalysisResults.root");
        outfile << subdir << ",";
        for (auto trigger : triggers) {
            TString triggerDirName = Form("JetSpectrum_%sJets_R0%i_%s_default",jetType.Data(),radius,trigger.Data());
            TDirectory *triggerDir = (TDirectory*)file->Get(triggerDirName.Data());
            TList *list = (TList*)triggerDir->Get(triggerDirName.Data());
            TH1D *hClusterCounter = (TH1D*)list->FindObject("hClusterCounter");
            int nClusters = hClusterCounter->GetBinContent(1);
            outfile << nClusters << ",";
            //if(trigger == "INT7") cout << subdir << endl;
            //cout << " - " << trigger << " " << nClusters << endl;
            //if(trigger == "INT7") totalClustersINT7 += nClusters;
            //if(trigger == "EJ2") totalClustersEJ2 += nClusters;
            //if(trigger == "EJ1") totalClustersEJ1 += nClusters;
        }
        outfile << endl;
        file->Close();
    }
    gSystem->FreeDirectory(dirp);
    outfile.close();

    cout << "Total clusters:" << endl;
    cout << " - INT7 " << totalClustersINT7 << endl;
    cout << " - EJ2 " << totalClustersEJ2 << endl;
    cout << " - EJ1 " << totalClustersEJ1 << endl;
}
