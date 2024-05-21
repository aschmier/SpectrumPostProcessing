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

void plotTrackQA(TString nfileINT7, TString nfileEMC7, TString nfileEJE, TString outputdir, TString system)
{
    double textSize  = 0.03;
    TString jetType  = "Full";
    TString fileType = "pdf";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<TH1D*> vecHPlus;
    vector<TH1D*> vecHMinus;
    vector<TString> fileNamesData{nfileINT7, nfileEMC7, nfileEJE};
    vector<TString> triggers{"INT7","EMC7","EJE"};
    vector<double> detLevelBin = getTrackQABinning();

    TCanvas* canvas       = new TCanvas("canvas","",10,10,750,500);
    double leftMargin   = 0.12;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TString filename = Form("%s/trackQA_pp8TeV_data.root",outputdir.Data());
    TFile *outFile = new TFile(filename.Data(),"RECREATE");

    TLine *l = new TLine(0,1,260,1);
    l->SetLineColor(14);
    l->SetLineWidth(3);
    l->SetLineStyle(7);

    for(int t = 0; t < triggers.size(); t++){
        TFile *fdata = TFile::Open(fileNamesData.at(t));
        if(!fdata || fdata->IsZombie()){
            cout << "File not found!" << endl;
            return;
        }

        AliEmcalList *list = (AliEmcalList*)fdata->Get(Form("AliEmcalTrackingQATask_%s_histos",triggers.at(t).Data()));
        if(!list) cout << "Not list!" << endl;
        THnSparse *sparse    = (THnSparse*)list->FindObject("fTracks");
        sparse->GetAxis(1)->SetRangeUser(-0.7,0.7);
        sparse->GetAxis(2)->SetRangeUser(1.4,3.1);
        sparse->GetAxis(6)->SetRange(1,1);
        TH1D *hminusfine = (TH1D*)sparse->Projection(0);
        TH1D *hminus = (TH1D*)hminusfine->Rebin(detLevelBin.size()-1, Form("hminus_rebin_%s",triggers.at(t).Data()), detLevelBin.data());
        hminus->Scale(1.,"width");
        //for(int b = 1; b < hminus->GetNbinsX(); b++){
        //    cout << "Bin low edge: " << hminus->GetXaxis()->GetBinLowEdge(b) << endl;
        //    cout << "Bin width: " << hminus->GetXaxis()->GetBinWidth(b) << endl << endl;
        //}
        if(triggers[t]=="INT7") hminus->GetXaxis()->SetRangeUser(10,30);
        else if(triggers[t]=="EMC7") hminus->GetXaxis()->SetRangeUser(30,50);
        else hminus->GetXaxis()->SetRangeUser(50,160);
        vecHMinus.push_back(hminus);
    }

    for(int t = 0; t < triggers.size(); t++){
        TFile *fdata = TFile::Open(fileNamesData.at(t));
        if(!fdata || fdata->IsZombie()){
            cout << "File not found!" << endl;
            return;
        }

        TList *list = (TList*)fdata->Get(Form("AliEmcalTrackingQATask_%s_histos",triggers.at(t).Data()));
        THnSparse *sparse    = (THnSparse*)list->FindObject("fTracks");
        sparse->GetAxis(1)->SetRangeUser(-0.7,0.7);
        sparse->GetAxis(2)->SetRangeUser(1.4,3.1);
        sparse->GetAxis(6)->SetRange(2,2);
        TH1D *hplusfine = (TH1D*)sparse->Projection(0);
        TH1D *hplus = (TH1D*)hplusfine->Rebin(detLevelBin.size()-1, Form("hplus_rebin_%s",triggers.at(t).Data()), detLevelBin.data());
        hplus->Scale(1.,"width");
        if(triggers[t]=="INT7") hplus->GetXaxis()->SetRangeUser(10,30);
        else if(triggers[t]=="EMC7") hplus->GetXaxis()->SetRangeUser(30,50);
        else hplus->GetXaxis()->SetRangeUser(50,160);
        vecHPlus.push_back(hplus);
    }

    TLegend *legend =  GetAndSetLegend2(0.85,(0.7-(3)*textSize),0.95,0.7,textSize,1);

    outFile->cd();
    outFile->mkdir("hists");
    outFile->cd("hists");

    // Plot data jets for all pp triggers in one plot
    TH1D* plot = new TH1D("plot", "", 260, 0, 260);
    for(int t=0; t<triggers.size(); t++){
        TH1D* dummy = (TH1D*)vecHPlus.at(t)->Clone(Form("rPlusMinus_%s",triggers.at(t).Data()));
        dummy->Divide(vecHPlus.at(t), vecHMinus.at(t), 1, 1, "b");
        dummy->SetMarkerStyle(styles[t]);
        dummy->SetMarkerColor(colors[t]);
        dummy->SetLineColor(colors[t]);
        //plot->GetXaxis()->SetRangeUser(20,320);
        //plot->GetYaxis()->SetRangeUser(0,1.1);
        SetStyleHistoTH1ForGraphs(plot,"","p_{T} (GeV/c)","#frac{N(h^{+})}{N(h^{-})}",0.03,0.04,0.03,0.04,1,1.1);
        if(t==0) plot->Draw("p,e");
        dummy->Draw("p,e,same");
        legend->AddEntry(dummy, triggers.at(t).Data(), "p");
    }
    legend->Draw();
    l->Draw("same");

    if(system == "pp") drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.80, textSize,kFALSE, kFALSE, true);
    if(system == "pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.80, textSize,kFALSE, kFALSE, true);

    drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.76, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("ALICE Data",0.93,0.72, textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/QoverPtShift_EMCal.%s", outputdir.Data(), fileType.Data()));
    outFile->Write();
    outFile->Close();
    delete outFile;
    legend->Clear();
}
