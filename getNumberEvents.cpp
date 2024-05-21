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
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D_rf.C"
#include "/home/austin/alice/unfoldingResults/makeRejectionFactor.cpp"

void getNumberEvents(TString period, TString mbfile, TString emc7file, TString ejefile, TString out)
{
    cout << "Doing period " << period << "..." << endl;

    int neventsMB, neventsEMC7, neventsEJE;
    stringstream s;
    ofstream outfile;
    outfile.open(out.Data(),std::ios::app);

    cout << "Getting INT7 events..." << endl;
    TFile *fmb   = TFile::Open(mbfile);
    if(!fmb || fmb->IsZombie()) return;
    TString nameMB    = "JetSpectrum_FullJets_R02_INT7_nodownscalecorr";
    TDirectory *dirMB = (TDirectory*)fmb->Get(nameMB.Data());
    TList *listMB     = (TList*)dirMB->Get(nameMB.Data());
    TH1D *eventsMB    = (TH1D*)listMB->FindObject("hClusterCounterAbs");
    neventsMB     = eventsMB->GetBinContent(1);

    if(period == "LHC12a" || period == "LHC12b"){
        neventsEMC7 = 0;
        neventsEJE = 0;
    }else{
        cout << "Getting EMC7 events..." << endl;
        TFile *femc7 = TFile::Open(emc7file);
        if(!femc7 || femc7->IsZombie()) return;
        TString nameEMC7    = "JetSpectrum_FullJets_R02_EMC7_nodownscalecorr";
        TDirectory *dirEMC7 = (TDirectory*)femc7->Get(nameEMC7.Data());
        TList *listEMC7     = (TList*)dirEMC7->Get(nameEMC7.Data());
        TH1D *eventsEMC7    = (TH1D*)listEMC7->FindObject("hClusterCounterAbs");
        neventsEMC7     = eventsEMC7->GetBinContent(1);

        cout << "Getting EJE events..." << endl;
        TFile *feje  = TFile::Open(ejefile);
        if(!feje || feje->IsZombie()) return;
        TString nameEJE    = "JetSpectrum_FullJets_R02_EJE_nodownscalecorr";
        TDirectory *dirEJE = (TDirectory*)feje->Get(nameEJE.Data());
        TList *listEJE     = (TList*)dirEJE->Get(nameEJE.Data());
        TH1D *eventsEJE    = (TH1D*)listEJE->FindObject("hClusterCounterAbs");
        neventsEJE     = eventsEJE->GetBinContent(1);
    }

    //cout << period << "\t" << neventsMB << "\t" << neventsEMC7 << "\t" << neventsEJE << endl;
    s << period << "\t" << neventsMB << "\t" << neventsEMC7 << "\t" << neventsEJE << "\n";
    outfile << s.str();

}
