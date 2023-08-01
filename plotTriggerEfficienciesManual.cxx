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
//#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningZg.C"

void plotTriggerEfficienciesManual(TString mc_file, TString outputdir, TString fileType, TString system, int minradius = 2, int maxradius = 6)
{

    Double_t textSize     = 0.03;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    vector<TFile*> files;
    vector<TString> triggers;
    if(system=="pp") triggers = {"INT7","EMC7","EJE"};
    if(system=="pPb") triggers = {"INT7","EJ2","EJ1"};
    vector<TH1D*> vecJetSpectrum;
    vector<TH1D*> vecClusterSpectrum;
    vector<double> rebinning = getJetPtBinningNonLinSmear8TeV();


    gSystem->Exec("mkdir -p "+outputdir+"/TriggerEfficienciesManual");

    TFile *fmc = TFile::Open(mc_file);
    if(!fmc || fmc->IsZombie()){
        cout << "MC file not found!" << endl;
        return;
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.075;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend =  GetAndSetLegend2(0.6,0.35,0.9,0.35+((3)*textSize*1.5),textSize);

    TH1D *dummy = new TH1D("dummy","",220,20,320);
    dummy->GetYaxis()->SetRangeUser(0.0,1.41);
    SetStyleHistoTH1ForGraphs(dummy,"","p_{T} (GeV/c)","Trigger Efficiency",0.03,0.04,0.03,0.04,1,0.7);

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int trigger = 0; trigger < triggers.size(); trigger++){    
            TString dirname = Form("JetSpectrum_FullJets_R0%i_%s_default",radius,triggers.at(trigger).Data());
            TDirectory *dirMC = (TDirectory*)fmc->Get(dirname.Data());
            if(!dirMC){
                cout << "Directory " << dirname << " not found!" << endl;
                return;
            }
            TList *listMC = (TList*)dirMC->Get(dirname.Data());
            if(!listMC){
                cout << "List " << dirname << " not found!" << endl;
                return;
            }
            TH1D *eventsMC = (TH1D*)listMC->FindObject("hClusterCounter");
            TH2D *jetspectrum2d = (TH2D*)listMC->FindObject("hJetSpectrum");
            if(!jetspectrum2d){
                cout << "2D spectrum not found!" << endl;
                return;
            }
            TH1D *jetspectrum1d = (TH1D*)jetspectrum2d->ProjectionY(Form("jetspectrum1d_R0%i_%s", radius, triggers.at(trigger).Data()),1,1);

            TH1D *rebinnedspectrum = (TH1D*)jetspectrum1d->Rebin(rebinning.size()-1, Form("rebinnedspectrum_%s_%i",triggers.at(trigger).Data(),radius), rebinning.data());
            rebinnedspectrum->Scale(1., "width");

            vecJetSpectrum.push_back(jetspectrum1d);
        }
        TH1D *triggerEfficiencyLow = (TH1D*)vecJetSpectrum.at(0)->Clone(Form("triggerefficencyLow_%i",radius));
        triggerEfficiencyLow->Divide(vecJetSpectrum.at(1),vecJetSpectrum.at(0),1,1,"B");
        triggerEfficiencyLow->SetMarkerStyle(styles[radius-2]);
        triggerEfficiencyLow->SetMarkerColor(colors[radius-2]);
        triggerEfficiencyLow->SetLineColor(colors[radius-2]);
        legend->AddEntry(triggerEfficiencyLow,triggers.at(1)+" / "+triggers.at(0),"p");
        
        TH1D *triggerEfficiencyHigh = (TH1D*)vecJetSpectrum.at(1)->Clone(Form("triggerefficencyHigh_%i",radius));
        triggerEfficiencyHigh->Divide(vecJetSpectrum.at(2),vecJetSpectrum.at(0),1,1,"B");
        triggerEfficiencyHigh->SetMarkerStyle(styles[radius-1]);
        triggerEfficiencyHigh->SetMarkerColor(colors[radius-1]);
        triggerEfficiencyHigh->SetLineColor(colors[radius-1]);
        legend->AddEntry(triggerEfficiencyHigh,triggers.at(2)+" / "+triggers.at(0),"p");
        legend->AddEntry(dummy,Form("R = 0.%i", radius),"");
        
        dummy->Draw("axis");
        triggerEfficiencyLow->Draw("p,e,same");
        triggerEfficiencyHigh->Draw("p,e,same");
        DrawGammaLines(20.,320.,1.,1.,8.,16,9);
        legend->Draw("same");
        canvas->SaveAs(Form("%s/TriggerEfficienciesManual/TriggerEfficiency_R0%i_%s_%s.%s",outputdir.Data(),radius,jetType.Data(),system.Data(),fileType.Data()));
        vecJetSpectrum.clear();
        legend->Clear();
        //vecClusterSpectrum.clear();

    
    }

}