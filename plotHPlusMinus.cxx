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

void plotHPlusMinus(TString nfileINT7, TString nfileEMC7, TString nfileEJE, TString outputdir, TString system, TString fileType = "pdf")
{
    double textSize  = 0.03;
    int minradius    = 2;
    int maxradius    = 6;
    TString jetType  = "Full";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<TH1D*> vecHPlus[maxradius-minradius+1];
    vector<TH1D*> vecHMinus[maxradius-minradius+1];
    vector<double> detlevelbin = getJetPtBinningNonLinSmearPoor();
    vector<TString> fileNamesData{nfileINT7, nfileEMC7, nfileEJE};
    vector<TString> triggers;
    if(system=="pp") triggers = {"INT7","EMC7","EJE"};
    if(system=="pPb") triggers = {"INT7","EJ2","EJ1"};


    for(int radius = minradius; radius <= maxradius; radius++){
        for(int t = 0; t < triggers.size(); t++){
            TFile *fdata = TFile::Open(fileNamesData.at(t));
            if(!fdata || fdata->IsZombie()){
                cout << "File not found!" << endl;
                return;
            }

            TString dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers.at(t).Data());

            TDirectory *dir = (TDirectory*)fdata->Get(dirname.Data());
            TList *list     = (TList*)dir->Get(dirname.Data());
            TH1D *events    = (TH1D*)list->FindObject("hClusterCounter");
            TH1D *hplus     = (TH1D*)list->FindObject("hQAPosTrVsPt");
            TH1D *hminus    = (TH1D*)list->FindObject("hQANegTrVsPt");
            //TH1D *rspec     = (TH1D*)spec1d->Rebin(detlevelbin.size()-1, Form("rdataspec_%i_%s",radius,triggers.at(t).Data()), detlevelbin.data());
            //TH1D *rclus     = (TH1D*)clus1d->Rebin(detlevelbin.size()-1, Form("rdataclus_%i_%s",radius,triggers.at(t).Data()), detlevelbin.data());
            //rspec->Scale(1.,"width");
            //rspec->Scale(1/events->GetBinContent(1));
            //rclus->Scale(1.,"width");
            hplus->Scale(1/events->GetBinContent(1));
            hminus->Scale(1/events->GetBinContent(1));
            vecHPlus[radius-minradius].push_back(hplus);
            vecHMinus[radius-minradius].push_back(hminus);
        }
    }

    TCanvas* canvas       = new TCanvas("canvas","",10,10,750,500);
    double leftMargin   = 0.12;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend =  GetAndSetLegend2(0.85,(0.8-(3)*textSize),0.95,0.8,textSize,1);

    TLine *l = new TLine(20,1,350,1);
    l->SetLineColor(14);
    l->SetLineWidth(3);
    l->SetLineStyle(7);

    for(int radius = minradius; radius <= maxradius; radius++){
        // Plot data jets for all pp triggers in one plot
        for(int t=0; t<triggers.size(); t++){
            TH1D* dummy = (TH1D*)vecHPlus[radius-minradius].at(t)->Clone(Form("rPlusMinus_R0%i_%i",radius,t));
            dummy->Divide(vecHPlus[radius-minradius].at(t), vecHMinus[radius-minradius].at(t), 1, 1, "b");
            dummy->SetMarkerStyle(styles[t]);
            dummy->SetMarkerColor(colors[t]);
            dummy->SetLineColor(colors[t]);
            dummy->GetXaxis()->SetRangeUser(20,200);
            dummy->GetYaxis()->SetRangeUser(-1,3);
            SetStyleHistoTH1ForGraphs(dummy,"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{d}{dp_{T}^{jet}}#frac{N^{+}}{N^{-}}",0.03,0.04,0.03,0.04,1,1.1);
            if(t==0) dummy->Draw("p,e");
            else dummy->Draw("p,e,same");
            legend->AddEntry(dummy, triggers.at(t).Data(), "p");
        }
        legend->Draw();
        l->Draw("same");

        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/hplusminus_R0%i.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();
    }
}
