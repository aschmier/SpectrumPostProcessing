#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"

void plotScaleFactor(TString filename, TString outputdir, TString filetype, double centmin = 0, double centmax = 100)
{
    double textSize  = 0.03;
    
    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    
    gSystem->Exec("mkdir -p "+outputdir);

    TFile *f = TFile::Open(filename.Data());
    if(!f || f->IsZombie()){
        cout << "File not found!" << endl;
        return;
    }

    TH1D *dummy1d = new TH1D("dummy1d","",100,0,100);
    dummy1d->GetYaxis()->SetRangeUser(0,5);
    dummy1d->GetXaxis()->SetRangeUser(0,110);
    SetStyleHistoTH1ForGraphs(dummy1d,"<SF>","Centrality (%)",textSize,0.035,textSize,0.035,1,1.2);

    TCanvas* canvas       = new TCanvas("canvas", "", 1000, 1000);
    double leftMargin   = 0.1;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogz();

    TLegend *legend =  GetAndSetLegend2(0.6,0.7-(4)*textSize,0.8,0.7,textSize);
    TList *list     = (TList*)f->Get("Scale_tracks_caloClusters_150_300_Histos");
    TH2D *hScalevsCent = (TH2D*)list->FindObject("fHistScalevsCent");
    SetStyleHistoTH2ForGraphs(hScalevsCent,"Centrality (%)","S_{TPC}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScalevsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScalevsCent.%s",outputdir.Data(),filetype.Data()));

    TH2D *hScaleEmcalvsCent = (TH2D*)list->FindObject("fHistScaleEmcalvsCent");
    SetStyleHistoTH2ForGraphs(hScalevsCent,"Centrality (%)","S_{EMC}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScaleEmcalvsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScaleEmcalvsCent.%s",outputdir.Data(),filetype.Data()));

    TH2D *hScale2EmcalvsCent = (TH2D*)list->FindObject("fHistScale2EmcalvsCent");
    SetStyleHistoTH2ForGraphs(hScalevsCent,"Centrality (%)","S_{2 x EMC}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScale2EmcalvsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScale2EmcalvsCent.%s",outputdir.Data(),filetype.Data()));

    

    TH1D *hScalevsCent1D = new TH1D("hScalevsCent1D","",100,1,101);
    TH1D *hScaleEmcalvsCent1D = new TH1D("hScaleEmcalvsCent1D","",100,1,101);
    TH1D *hScale2EmcalvsCent1D = new TH1D("hScale2EmcalvsCent1D","",100,1,101);

    

    for(int b = 2; b <= hScalevsCent->GetNbinsX(); b++){
        
        hScalevsCent->GetXaxis()->SetRange(b,b);
        hScalevsCent1D->SetBinContent(b,hScalevsCent->GetMean(2));
        hScalevsCent1D->SetBinError(b,hScalevsCent->GetMeanError(2));
        hScaleEmcalvsCent->GetXaxis()->SetRange(b,b);
        hScaleEmcalvsCent1D->SetBinContent(b,hScaleEmcalvsCent->GetMean(2));
        hScaleEmcalvsCent1D->SetBinError(b,hScaleEmcalvsCent->GetMeanError(2));
        hScale2EmcalvsCent->GetXaxis()->SetRange(b,b);
        hScale2EmcalvsCent1D->SetBinContent(b,hScale2EmcalvsCent->GetMean(2));
        hScale2EmcalvsCent1D->SetBinError(b,hScale2EmcalvsCent->GetMeanError(2));
        
    }
    
    dummy1d->GetYaxis()->SetRangeUser(0.8,1.7);
    dummy1d->Draw("axis");

    hScalevsCent1D->SetMarkerStyle(styles[0]);
    hScalevsCent1D->SetMarkerColor(colors[0]);
    hScalevsCent1D->SetLineColor(colors[0]);
    hScalevsCent1D->Draw("p,e,same");
    legend->AddEntry(hScalevsCent1D,"hHistScalevsCent","p");

    hScaleEmcalvsCent1D->SetMarkerStyle(styles[1]);
    hScaleEmcalvsCent1D->SetMarkerColor(colors[1]);
    hScaleEmcalvsCent1D->SetLineColor(colors[1]);
    hScaleEmcalvsCent1D->Draw("p,e,same");
    legend->AddEntry(hScaleEmcalvsCent1D,"fHistScaleEmcalvsCent","p");

    hScale2EmcalvsCent1D->SetMarkerStyle(styles[2]);
    hScale2EmcalvsCent1D->SetMarkerColor(colors[2]);
    hScale2EmcalvsCent1D->SetLineColor(colors[2]);
    hScale2EmcalvsCent1D->Draw("p,e,same");
    legend->AddEntry(hScale2EmcalvsCent1D,"fHistScale2EmcalvsCent","p");

    legend->Draw("same");
    canvas->SaveAs(Form("%s/hScalevsCent1D.%s",outputdir.Data(),filetype.Data()));

    f->Close();
    delete f;
    delete canvas;
    delete dummy1d;
    delete legend;
    delete hScalevsCent;
    delete hScaleEmcalvsCent;
    delete hScale2EmcalvsCent;
    delete hScalevsCent1D;
    delete hScaleEmcalvsCent1D;
    delete hScale2EmcalvsCent1D;

    return;
}
