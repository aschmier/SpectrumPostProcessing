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

void plotTriggerClusters(TString mb_file, TString emc7_file, TString eje_file, TString outputdir, TString fileType, TString system, int minradius = 2, int maxradius = 6)
{
    Double_t textSize     = 0.03;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14] = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<TFile*> files;
    vector<TString> triggers;
    if(system=="pp") triggers = {"INT7","EMC7","EJE"};
    if(system=="pPb") triggers = {"INT7","EJ2","EJ1"};
    vector<TString> triggers2;
    if(system=="pp") triggers2 = {"Min. Bias","EMCal-L0","EMCal-L1"};
    if(system=="pPb") triggers2 = {"Min. Bias","EMCal-L1 EJ2","EMCal-L1 EJ1"};
    vector<TH1D*> vecClusters[maxradius-minradius+1];
    vector<double> binningrffine = getJetPtBinningRejectionFactorsFine();

    gSystem->Exec("mkdir -p "+outputdir+"/TriggerClusters");


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
            if(!dir){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers.at(trigger).Data());
                dir = (TDirectory*)files.at(trigger)->Get(dirname.Data());
            }
            TList *list = (TList*)dir->Get(dirname.Data());
            TH1D *events = (TH1D*)list->FindObject("hClusterCounter");
            TH1D *clusters = (TH1D*)list->FindObject("hClusterEnergy1D");
            clusters->Scale(1/events->GetBinContent(1));
            vecClusters[radius-minradius].push_back(clusters);
        }
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.1;
    Double_t rightMargin        = 0.025;
    Double_t topMargin          = 0.025;
    Double_t bottomMargin       = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend;
    
    if(system == "pPb") legend = GetAndSetLegend2(0.725,(0.82-(triggers.size())*textSize),0.825,0.82,textSize,1);
    else legend = GetAndSetLegend2(0.78,(0.82-(triggers.size())*textSize),0.88,0.82,textSize,1);

    // Make dummy histogram for axes
    TH1D *h = new TH1D("h","",22000,0,220);
    SetStyleHistoTH1ForGraphs(h,"","#it{E}^{cluster}","#frac{1}{#it{N}^{trig}} #frac{d#it{N}}{d#it{E}^{cluster}}",0.03,0.04,0.03,0.04,1,1.1);
    //h->GetXaxis()->SetMoreLogLabels();
    h->GetYaxis()->SetRangeUser(1e-10,40);
    h->Draw("axis");

    for(int radius = minradius; radius <= maxradius; radius++){
            for(int trigger = 0; trigger < triggers.size(); trigger++){
                TH1D *rebinnedClusters = (TH1D*)vecClusters[radius-minradius].at(trigger)->Rebin(binningrffine.size()-1, Form("rebinnedClusters_%s_%i",triggers.at(trigger).Data(),radius), binningrffine.data());
                rebinnedClusters->Scale(1.,"width");
                rebinnedClusters->SetMarkerStyle(styles[trigger]);
                rebinnedClusters->SetMarkerColor(colors[trigger]);
                rebinnedClusters->SetLineColor(colors[trigger]);
                rebinnedClusters->GetXaxis()->SetRangeUser(0.3,200);
                rebinnedClusters->Draw("p,e,same");
                legend->AddEntry(rebinnedClusters, Form("%s (%s)", triggers2.at(trigger).Data(), triggers.at(trigger).Data()), "p");
            }
            legend->Draw("same");
            if(system == "pPb"){ 
                drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.75,0.88, 0.03, false, false, false);
                //drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.75,0.84, 0.03, false, false, false);
            }else{ 
                drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.805,0.88, 0.03, false, false, false);
                //drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.805,0.84, 0.03, false, false, false);
            }
            canvas->SaveAs(Form("%s/TriggerClusters/clusters_R0%i.%s", outputdir.Data(), radius, fileType.Data()));
    }
}
