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
#include "/home/austin/alice/AliPhysics/PWG/EMCAL/EMCALbase/AliEmcalList.h"

TH1 *makeRebinned(TH1 *hist, TString trigger) {
        binninghelper binhandler; 
        if(trigger == "INT7") binhandler = binninghelper(0.15, {{0.3, 0.05}, {0.99999, 0.1}, {3., 0.2}, {10., 0.5}, {12.,2.}, {15.,3.}, {20., 5.}, {40., 20.}, {70., 30.}, {100., 30.}});
        else if(trigger == "EMC7") binhandler = binninghelper(0.15, {{0.3, 0.05}, {0.99999, 0.1}, {3., 0.2}, {10., 0.5}, {12.,2.}, {15.,3.}, {20., 5.}, {40., 20.}, {70., 30.}, {100., 30.}});
        else if(trigger == "EJ") binhandler = binninghelper(0.15, {{0.3, 0.05}, {0.99999, 0.1}, {3., 0.2}, {10., 0.5}, {20.,1.}, {40., 4.}, {50., 10.}, {70., 20.}, {110., 40.}, {200., 90.}});
        else if(trigger == "EJE") binhandler = binninghelper(0.15, {{0.3, 0.05}, {0.99999, 0.1}, {3., 0.2}, {10., 0.5}, {20.,1.}, {40., 4.}, {50., 10.}, {70., 20.}, {110., 40.}, {200., 90.}});
        else cout << "Trigger not recognized!" << endl;
        auto newbinning = binhandler.CreateCombinedBinning();
        auto rebinned = hist->Rebin(newbinning.size()-1., Form("%srebinned", hist->GetName()), newbinning.data());
        return rebinned;
}

//std::vector<double> getTrackQABinning_pPb(){
//  binninghelper binning(10., {{12., 2.}, {15., 3.}, {20., 5.}, {30., 10.}, {34., 2.}, {42., 4.}, {50., 8.}, {70., 10.}, {100., 30.}, {150., 50.}});
//  return binning.CreateCombinedBinning();
//}

void plotTrackQA(TString datafile, TString outputdir, TString system, TString wagon, int etasign = 0, bool emcalrestrict = false)
{
    double textSize  = 0.03;
    TString jetType  = "Full";
    TString fileType = "png";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<TH1D*> vecHPlus;
    vector<TH1D*> vecHMinus;
    vector<TString> triggers;
    if(system=="pp") triggers = {"INT7","EMC7","EJE"};
    else if(system=="pPb") triggers = {"INT7","EJ"};
    vector<double> detLevelBin;
    if(system=="pp") detLevelBin = getTrackQABinning_pp();
    else if(system=="pPb") detLevelBin = getTrackQABinning_pPb();
    else cout << "System not recognized!" << endl;

    TCanvas* canvas       = new TCanvas("canvas","",10,10,750,500);
    double leftMargin   = 0.12;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TString filename;
    if(system=="pp") filename = Form("%s/trackQA_pp8000GeV_data.root",outputdir.Data());
    else if(system=="pPb") filename = Form("%s/trackQA_pPb8160GeV_data.root",outputdir.Data());
    else cout << "System not recognized!" << endl;
    //TFile *outFile = new TFile(filename.Data(),"RECREATE");

    TFile *fdata = TFile::Open(datafile);
    if(!fdata || fdata->IsZombie()){
        cout << "File not found!" << endl;
        return;
    }

    TLine *l = new TLine(0,1,260,1);
    l->SetLineColor(14);
    l->SetLineWidth(3);
    l->SetLineStyle(7);

    for(int t = 0; t < triggers.size(); t++){
        AliEmcalList *list = (AliEmcalList*)fdata->Get(Form("AliEmcalTrackingQATask_%s_%s_histos",triggers.at(t).Data(),wagon.Data()));
        if(!list) cout << "File list for trigger " << triggers.at(t) << " not found!" << endl;
        auto sparse = dynamic_cast<THnSparse *>(list->FindObject("fTracks"));
        if(!sparse) cout << "Sparse for trigger " << triggers.at(t) << " not found!" << endl;
        auto norm   = dynamic_cast<TH1 *>(list->FindObject("fHistEventCount"))->GetBinContent(1);
        sparse->Sumw2();
        
        if(emcalrestrict){
            if(system=="pp") sparse->GetAxis(2)->SetRangeUser(1.405,3.135);
            if(system=="pPb") sparse->GetAxis(2)->SetRangeUser(1.405,3.292);
            if(etasign==0) sparse->GetAxis(1)->SetRangeUser(-0.7,0.7);
            else if(etasign>0) sparse->GetAxis(1)->SetRangeUser(0.01,0.699);
            else sparse->GetAxis(1)->SetRangeUser(-0.699,-0.01);
        }
        if(etasign != 0){
            if(etasign < 0) sparse->GetAxis(1)->SetRangeUser(-0.899, -0.01);
	        else sparse->GetAxis(1)->SetRangeUser(0.01, 0.899);
	    }else{
            sparse->GetAxis(1)->SetRangeUser(-0.899, 0.899);
        }

        sparse->GetAxis(6)->SetRange(1,1);
        TH1 *hminus = makeRebinned(sparse->Projection(0), triggers.at(t));
        if(!hminus) cout << "Projection for trigger " << triggers.at(t) << " not found!" << endl;
        hminus->SetName(Form("hminus_%s",triggers.at(t).Data()));
        hminus->SetDirectory(nullptr);
        hminus->Scale(1./norm);
        hminus->Scale(1.,"width");        
        //if(triggers[t]=="INT7") hminus->GetXaxis()->SetRangeUser(10,30);
        //else if(triggers[t]=="EMC7") hminus->GetXaxis()->SetRangeUser(30,50);
        //else if(triggers[t]=="EJE") hminus->GetXaxis()->SetRangeUser(50,160);
        //else hminus->GetXaxis()->SetRangeUser(30,160);
        //hminus->GetXaxis()->SetRangeUser(0,160);
        vecHMinus.push_back((TH1D*)hminus);

        sparse->GetAxis(6)->SetRange(2,2);
        TH1 *hplus = makeRebinned(sparse->Projection(0), triggers.at(t));
        hplus->SetName(Form("hplus_%s",triggers.at(t).Data()));
        hplus->SetDirectory(nullptr);
        hplus->Scale(1./norm);
        hplus->Scale(1.,"width");
        //if(triggers[t]=="INT7") hplus->GetXaxis()->SetRangeUser(10,30);
        //else if(triggers[t]=="EMC7") hplus->GetXaxis()->SetRangeUser(30,50);
        //else if(triggers[t]=="EJE") hplus->GetXaxis()->SetRangeUser(50,160);
        //else hplus->GetXaxis()->SetRangeUser(30,160);
        //hplus->GetXaxis()->SetRangeUser(0,160);
        vecHPlus.push_back((TH1D*)hplus);
    }

    TLegend *legend =  GetAndSetLegend2(0.85,(0.7-(3)*textSize),0.95,0.7,textSize,1);

    // outFile->cd();
    // outFile->mkdir("hists");
    // outFile->cd("hists");

    // Plot data jets for all pp triggers in one plot
    TH1D* plot = new TH1D("plot", "", 200, 0, 200);
    plot->GetYaxis()->SetRangeUser(0.5,1.5);
    for(int t=0; t<triggers.size(); t++){
        plot->Draw("axis");
        TH1D* dummy = (TH1D*)vecHPlus.at(t)->Clone(Form("rPlusMinus_%s",triggers.at(t).Data()));
        dummy->Divide(vecHPlus.at(t), vecHMinus.at(t));
        dummy->SetMarkerStyle(styles[t]);
        dummy->SetMarkerColor(colors[t]);
        dummy->SetLineColor(colors[t]);
        SetStyleHistoTH1ForGraphs(plot,"","p_{T} (GeV/c)","#frac{N(h^{+})}{N(h^{-})}",0.03,0.04,0.03,0.04,1,1.1);
        dummy->Draw("p,e,same");

        DrawGammaLines(0.,200.,1.,1.,4.,16,9);


        if(system == "pp") drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
        if(system == "pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);

        drawLatexAdd("Ratio of Charged Tracks",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Trigger: %s", triggers.at(t).Data()),0.93,0.82, textSize,kFALSE, kFALSE, true);
        if(emcalrestrict){
            if(etasign==0) drawLatexAdd(Form("|#eta| < 0.7, 1.405 < #phi < %1.3f", system=="pPb"? 3.292 : 3.135),0.93,0.78, textSize,kFALSE, kFALSE, true);
            else if(etasign>0) drawLatexAdd(Form("0.01 < #eta < 0.699, 1.405 < #phi < %1.3f", system=="pPb"? 3.292 : 3.135),0.93,0.78, textSize,kFALSE, kFALSE, true);
            else drawLatexAdd(Form("-0.699 < #eta < -0.01, 1.405 < #phi < %1.3f", system=="pPb"? 3.292 : 3.135),0.93,0.78, textSize,kFALSE, kFALSE, true);
        }else{
            if(etasign==0) drawLatexAdd("|#eta| < 0.9",0.93,0.78, textSize,kFALSE, kFALSE, true);
            else if(etasign>0) drawLatexAdd("0.01 < #eta < 0.899",0.93,0.78, textSize,kFALSE, kFALSE, true);
            else drawLatexAdd("-0.899 < #eta < -0.01",0.93,0.78, textSize,kFALSE, kFALSE, true);
        }
        drawLatexAdd(Form("Q/p_{T} Shift: %se-4", wagon.Data()),0.93,0.74, textSize,kFALSE, kFALSE, true);

        TString etasignstr = "";
        if(etasign==0) etasignstr = "fulleta";
        else if(etasign>0) etasignstr = "poseta";
        else etasignstr = "negeta";
    
        canvas->SaveAs(Form("%s/QoverPtShift_%s_%s_%s_%s_%s.%s", outputdir.Data(), emcalrestrict? "EMCalRestricted" : "FullTPC", system.Data(), etasignstr.Data(), triggers.at(t).Data(), wagon.Data(), fileType.Data()));
        // outFile->Write();
        // outFile->Close();
        // delete outFile;
    }
}
