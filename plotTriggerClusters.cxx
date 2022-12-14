#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

void plotTriggerClusters(TString mb_file, TString emc7_file, TString eje_file, TString outputdir, TString fileType)
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 6;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14] = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<TFile*> files;
    vector<TString> triggers{"INT7","EMC7","EJE"};
    vector<TString> triggers2{"Min. Bias","EMCal-L0","EMCal-L1"};
    vector<TH1D*> vecClusters[maxradius-minradius+1];
    vector<double> binningrffine = getJetPtBinningRejectionFactorsFine();

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
            TString dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers.at(trigger).Data());
            TDirectory *dir = (TDirectory*)files.at(trigger)->Get(dirname.Data());
            TList *list = (TList*)dir->Get(dirname.Data());
            TH1D *events = (TH1D*)list->FindObject("hClusterCounter");
            TH1D *clusters = (TH1D*)list->FindObject("hClusterEnergy1D");
            clusters->Scale(1/events->GetBinContent(1));
            vecClusters[radius-minradius].push_back(clusters);
        }
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.1;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.77,(0.82-(triggers.size())*textSize),0.87,0.82,textSize,1);

    for(int radius = minradius; radius <= maxradius; radius++){
            for(int trigger = 0; trigger < triggers.size(); trigger++){
                TH1D *rebinnedClusters = (TH1D*)vecClusters[radius-minradius].at(trigger)->Rebin(binningrffine.size()-1, Form("rebinnedClusters_%s_%i",triggers.at(trigger).Data(),radius), binningrffine.data());
                rebinnedClusters->Scale(1.,"width");
                rebinnedClusters->SetMarkerStyle(styles[trigger]);
                rebinnedClusters->SetMarkerColor(colors[trigger]);
                rebinnedClusters->SetLineColor(colors[trigger]);
                rebinnedClusters->GetXaxis()->SetRangeUser(20,320);
                rebinnedClusters->GetYaxis()->SetRangeUser(1e-9,1);
                SetStyleHistoTH1ForGraphs(rebinnedClusters,"","E^{cluster}","#frac{1}{N^{trig}} #frac{dN}{dE^{cluster}}",0.03,0.04,0.03,0.04,1,1.1);
                rebinnedClusters->Draw(trigger==0? "p,e" : "p,e,same");
                legend->AddEntry(vecClusters[radius-minradius].at(trigger), Form("%s (%s)", triggers2.at(trigger).Data(), triggers.at(trigger).Data()), "p");
            }
            legend->Draw();
            drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.78,0.88, 0.03, false, false, false);
            drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.78,0.84, 0.03, false, false, false);
            canvas->SaveAs(Form("%s/TriggerClusters/clusters_R0%i.%s", outputdir.Data(), radius, fileType.Data()));
    }
}
