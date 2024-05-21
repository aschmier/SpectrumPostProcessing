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

void getNEntriesForRange(TString mb_file, TString emc7_file, TString eje_file)
{
    vector<TString> triggers{"INT7","EMC7","EJE"};
    vector<double> detLevelRebin = getJetPtBinningNonLinTruePoor();
    int minradius = 2;
    int maxradius = 6;
    vector<TFile*> files;
    TString jetType = "Full";

    TFile *fmb = TFile::Open(mb_file);
    if(!fmb || fmb->IsZombie()){
        cout << "INT7 file not found!" << endl;
        return;
    }
    files.push_back(fmb);

    TFile *femc7 = TFile::Open(emc7_file);
    if(!femc7 || femc7->IsZombie()){
        cout << "EMC7 file not found!" << endl;
        return;
    }
    files.push_back(femc7);

    TFile *feje = TFile::Open(eje_file);
    if(!feje || feje->IsZombie()){
        cout << "EJE file not found!" << endl;
        return;
    }
    files.push_back(feje);

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int trigger = 0; trigger < triggers.size(); trigger++){
            cout << endl << "////////// " << triggers.at(trigger).Data() << " ////////// R = 0." << radius << " //////////" << endl << endl;
            TString dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers.at(trigger).Data());
            TDirectory *dir = (TDirectory*)files.at(trigger)->Get(dirname.Data());
            if(!dir){
                dirname.ReplaceAll("nodownscalecorr","default");
                dir = (TDirectory*)files.at(trigger)->Get(dirname.Data());
            }
            TList *list  = (TList*)dir->Get(dirname.Data());
            TH2D *spec2d = (TH2D*)list->FindObject("hJetSpectrum");
            TH1D *spec1d = (TH1D*)spec2d->ProjectionY("spec1d",1,1);
            TH1D *specrebinned = (TH1D*)spec1d->Rebin(detLevelRebin.size()-1, Form("specrebinned_R0%i_%s",radius,triggers.at(trigger).Data()), detLevelRebin.data());
            for(int bin=1; bin<specrebinned->GetNbinsX()+1; bin++){
                cout << "Bin for pt " << specrebinned->GetBinLowEdge(bin) << " --> " << ( specrebinned->GetBinLowEdge(bin) + specrebinned->GetBinWidth(bin) ) << " has " << specrebinned->GetBinContent(bin) << " entries for trigger " << triggers.at(trigger) << endl;
                if(specrebinned->GetBinContent(bin) < 50) cout << "Low Statistics!" << endl;
            }
        }
    }
}
