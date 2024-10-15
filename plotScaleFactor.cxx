#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"

void plotScaleFactor(TString filename, TString outputdir, TString filetype, TString system, double centmin = 0, double centmax = 100)
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
    double rightMargin  = 0.03;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogz();
    TLegend *legend =  GetAndSetLegend2(0.13,0.9-(8)*textSize,0.33,0.9,textSize);
    TList *list = (TList*)f->Get("Scale_tracks_caloClusters_150_300_Histos");
    TH2D *hScalevsCent = (TH2D*)list->FindObject("fHistScalevsCent");
    SetStyleHistoTH1ForGraphs(dummy1d,"","Centrality (%)","S_{TPC}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScalevsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScalevsCent.%s",outputdir.Data(),filetype.Data()));
    double meanScalevsCent = hScalevsCent->GetMean(2);

    TH2D *hScaleEmcalvsCent = (TH2D*)list->FindObject("fHistScaleEmcalvsCent");
    SetStyleHistoTH1ForGraphs(dummy1d,"","Centrality (%)","S_{EMC}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScaleEmcalvsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScaleEmcalvsCent.%s",outputdir.Data(),filetype.Data()));
    double meanScaleEmcalvsCent = hScaleEmcalvsCent->GetMean(2);

    TH2D *hScale2EmcalvsCent = (TH2D*)list->FindObject("fHistScale2EmcalvsCent");
    SetStyleHistoTH1ForGraphs(dummy1d,"","Centrality (%)","S_{2 x EMC}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScale2EmcalvsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScale2EmcalvsCent.%s",outputdir.Data(),filetype.Data()));
    double meanScale2EmcalvsCent = hScale2EmcalvsCent->GetMean(2);

    TH2D *hScale3EmcalvsCent = (TH2D*)list->FindObject("fHistScale3EmcalvsCent");
    SetStyleHistoTH1ForGraphs(dummy1d,"","Centrality (%)","S_{3 x EMC}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScale3EmcalvsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScale3EmcalvsCent.%s",outputdir.Data(),filetype.Data()));
    double meanScale3EmcalvsCent = hScale3EmcalvsCent->GetMean(2);

    TH2D *hScaleShift1EmcalvsCent = (TH2D*)list->FindObject("fHistScaleShift1EmcalvsCent");
    SetStyleHistoTH1ForGraphs(dummy1d,"","Centrality (%)","S_{EMC,S1}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScaleShift1EmcalvsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScaleShift1EmcalvsCent.%s",outputdir.Data(),filetype.Data()));
    double meanScaleShift1EmcalvsCent = hScaleShift1EmcalvsCent->GetMean(2);

    TH2D *hScaleShift2EmcalvsCent = (TH2D*)list->FindObject("fHistScaleShift2EmcalvsCent");
    SetStyleHistoTH1ForGraphs(dummy1d,"","Centrality (%)","S_{EMC,S2}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScaleShift2EmcalvsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScaleShift2EmcalvsCent.%s",outputdir.Data(),filetype.Data()));
    double meanScaleShift2EmcalvsCent = hScaleShift2EmcalvsCent->GetMean(2);

    TH2D *hScaleShiftMeanEmcalvsCent = (TH2D*)list->FindObject("fHistScaleShiftMeanEmcalvsCent");
    SetStyleHistoTH1ForGraphs(dummy1d,"","Centrality (%)","S_{EMC,Mean}",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->Draw("axis");
    hScaleShiftMeanEmcalvsCent->Draw("colz,same");
    canvas->SaveAs(Form("%s/hScaleShiftMeanEmcalvsCent.%s",outputdir.Data(),filetype.Data()));
    double meanScaleShiftMeanEmcalvsCent = hScaleShiftMeanEmcalvsCent->GetMean(2);

    TH2D *fHistScaleEmcalvsPhi = (TH2D*)list->FindObject("fHistScaleEmcalvsPhi");
    SetStyleHistoTH1ForGraphs(dummy1d,"","#phi","<SF>",textSize,0.035,textSize,0.035,1,1.2);
    dummy1d->GetXaxis()->SetRangeUser(0,2*TMath::Pi());
    dummy1d->Draw("axis");
    fHistScaleEmcalvsPhi->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistScaleEmcalvsPhi.%s",outputdir.Data(),filetype.Data()));
    dummy1d->GetXaxis()->SetRangeUser(0,100);

    TH2D *fHistPtTPCvsCent = (TH2D*)list->FindObject("fHistPtTPCvsCent");

    TH2D *fHistPtEMCALvsCent = (TH2D*)list->FindObject("fHistPtEMCALvsCent");

    TH2D *fHistTrackPtvsCent = (TH2D*)list->FindObject("fHistTrackPtvsCent");

    TH1D *dummy1detaphi = new TH1D("dummy1d","",100,-1,1);
    dummy1detaphi->GetYaxis()->SetRangeUser(0,2*TMath::Pi());
    SetStyleHistoTH1ForGraphs(dummy1detaphi,"","#eta","#phi",textSize,0.035,textSize,0.035,1,1.2);

    canvas->SetLogz(0);

    TH2D *fHistTrackEtaPhi = (TH2D*)list->FindObject("fHistTrackEtaPhi");
    dummy1detaphi->Draw("axis");
    fHistTrackEtaPhi->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistTrackEtaPhi.%s",outputdir.Data(),filetype.Data()));

    TH2D *fHistEtaPhiScaleEMCAL = (TH2D*)list->FindObject("fHistEtaPhiScaleEMCAL");
    dummy1detaphi->Draw("axis");
    fHistEtaPhiScaleEMCAL->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistEtaPhiScaleEMCAL.%s",outputdir.Data(),filetype.Data()));

    TH2D *fHistEtaPhiScale2EMCAL = (TH2D*)list->FindObject("fHistEtaPhiScale2EMCAL");
    dummy1detaphi->Draw("axis");
    fHistEtaPhiScale2EMCAL->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistEtaPhiScale2EMCAL.%s",outputdir.Data(),filetype.Data()));

    TH2D *fHistEtaPhiScale3EMCAL = (TH2D*)list->FindObject("fHistEtaPhiScale3EMCAL");
    dummy1detaphi->Draw("axis");
    fHistEtaPhiScale3EMCAL->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistEtaPhiScale3EMCAL.%s",outputdir.Data(),filetype.Data()));

    TH2D *fHistEtaPhiScaleShift1EMCAL = (TH2D*)list->FindObject("fHistEtaPhiScaleShift1Emcal");
    dummy1detaphi->Draw("axis");
    fHistEtaPhiScaleShift1EMCAL->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistEtaPhiScaleShift1EMCAL.%s",outputdir.Data(),filetype.Data()));

    TH2D *fHistEtaPhiScaleShift2EMCAL = (TH2D*)list->FindObject("fHistEtaPhiScaleShift2Emcal");
    dummy1detaphi->Draw("axis");
    fHistEtaPhiScaleShift2EMCAL->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistEtaPhiScaleShift2EMCAL.%s",outputdir.Data(),filetype.Data()));

    TH2D *fHistEtaPhiScaleShiftMeanEMCAL = (TH2D*)list->FindObject("fHistEtaPhiScaleShiftMeanEmcal");
    dummy1detaphi->Draw("axis");
    fHistEtaPhiScaleShiftMeanEMCAL->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistEtaPhiScaleShiftMeanEMCAL.%s",outputdir.Data(),filetype.Data()));

    TH2D *fHistEtaPhiScaleEmcalvsPhi = (TH2D*)list->FindObject("fHistEtaPhiScaleEmcalvsPhi");
    dummy1detaphi->Draw("axis");
    fHistEtaPhiScaleEmcalvsPhi->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistEtaPhiScaleEmcalvsPhi.%s",outputdir.Data(),filetype.Data()));

    TH1D *dummy1dPtPhi = new TH1D("dummy1dPtPhi","",100,0,100);
    dummy1dPtPhi->GetYaxis()->SetRangeUser(0,2*TMath::Pi());
    SetStyleHistoTH1ForGraphs(dummy1dPtPhi,"","#it{p}_{T}","#phi",textSize,0.035,textSize,0.035,1,1.2);

    canvas->SetLogz(1);

    TH2D *fHistPtvsPhi = (TH2D*)list->FindObject("fHistPtvsPhi");
    dummy1dPtPhi->Draw("axis");
    fHistPtvsPhi->Draw("colz,same");
    canvas->SaveAs(Form("%s/fHistPtvsPhi.%s",outputdir.Data(),filetype.Data()));

    canvas->SetLogz(0);


    SetStyleHistoTH1ForGraphs(dummy1d,"","Centrality (%)","<SF>",textSize,0.035,textSize,0.035,1,1.2);


    TH1D *hScalevsCent1D = new TH1D("hScalevsCent1D","",100,1,101);
    TH1D *hScaleEmcalvsCent1D = new TH1D("hScaleEmcalvsCent1D","",100,1,101);
    TH1D *hScale2EmcalvsCent1D = new TH1D("hScale2EmcalvsCent1D","",100,1,101);
    TH1D *hScale3EmcalvsCent1D = new TH1D("hScale3EmcalvsCent1D","",100,1,101);
    TH1D *hScaleShift1EmcalvsCent1D = new TH1D("hScaleShift1EmcalvsCent1D","",100,1,101);
    TH1D *hScaleShift2EmcalvsCent1D = new TH1D("hScaleShift2EmcalvsCent1D","",100,1,101);
    TH1D *hScaleShiftMeanEmcalvsCent1D = new TH1D("hScaleShiftMeanEmcalvsCent1D","",100,1,101);
    TH1D *fHistPtTPCvsCent1D = new TH1D("fHistPtTPCvsCent1D","",100,1,101);
    TH1D *fHistPtEMCALvsCent1D = new TH1D("fHistPtEMCALvsCent1D","",100,1,101);
    TH1D *fHistTrackPtvsCent1D = new TH1D("fHistTrackPtvsCent1D","",100,1,101);
    TH1D *fHistScaleEmcalvsPhi1D = new TH1D("fHistScaleEmcalvsPhi1D","",8,0,2*TMath::Pi());

    int maxbin = hScalevsCent->GetNbinsX();
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
        hScale3EmcalvsCent->GetXaxis()->SetRange(b,b);
        hScale3EmcalvsCent1D->SetBinContent(b,hScale3EmcalvsCent->GetMean(2));
        hScale3EmcalvsCent1D->SetBinError(b,hScale3EmcalvsCent->GetMeanError(2));
        hScaleShift1EmcalvsCent->GetXaxis()->SetRange(b,b);
        hScaleShift1EmcalvsCent1D->SetBinContent(b,hScaleShift1EmcalvsCent->GetMean(2));
        hScaleShift1EmcalvsCent1D->SetBinError(b,hScaleShift1EmcalvsCent->GetMeanError(2));
        hScaleShift2EmcalvsCent->GetXaxis()->SetRange(b,b);
        hScaleShift2EmcalvsCent1D->SetBinContent(b,hScaleShift2EmcalvsCent->GetMean(2));
        hScaleShift2EmcalvsCent1D->SetBinError(b,hScaleShift2EmcalvsCent->GetMeanError(2));
        hScaleShiftMeanEmcalvsCent->GetXaxis()->SetRange(b,b);
        hScaleShiftMeanEmcalvsCent1D->SetBinContent(b,hScaleShiftMeanEmcalvsCent->GetMean(2));
        hScaleShiftMeanEmcalvsCent1D->SetBinError(b,hScaleShiftMeanEmcalvsCent->GetMeanError(2));
        fHistPtTPCvsCent->GetXaxis()->SetRange(b,b);
        fHistPtTPCvsCent1D->SetBinContent(b,fHistPtTPCvsCent->GetMean(2));
        fHistPtTPCvsCent1D->SetBinError(b,fHistPtTPCvsCent->GetMeanError(2));
        fHistPtEMCALvsCent->GetXaxis()->SetRange(b,b);
        fHistPtEMCALvsCent1D->SetBinContent(b,fHistPtEMCALvsCent->GetMean(2));
        fHistPtEMCALvsCent1D->SetBinError(b,fHistPtEMCALvsCent->GetMeanError(2));
        fHistTrackPtvsCent->GetXaxis()->SetRange(b,b);
        fHistTrackPtvsCent1D->SetBinContent(b,fHistTrackPtvsCent->GetMean(2));
        fHistTrackPtvsCent1D->SetBinError(b,fHistTrackPtvsCent->GetMeanError(2));
    }

    double meanScalePhi = 0.;
    for(int b2 = 1; b2 <= fHistScaleEmcalvsPhi->GetNbinsX(); b2++){
        fHistScaleEmcalvsPhi->GetXaxis()->SetRange(b2,b2);
        fHistScaleEmcalvsPhi1D->SetBinContent(b2,fHistScaleEmcalvsPhi->GetMean(2));
        fHistScaleEmcalvsPhi1D->SetBinError(b2,fHistScaleEmcalvsPhi->GetMeanError(2));
        meanScalePhi += fHistScaleEmcalvsPhi->GetMean(2);
    }
    meanScalePhi = meanScalePhi/8.;

    hScalevsCent->GetXaxis()->SetRange(1,maxbin);
    hScaleEmcalvsCent->GetXaxis()->SetRange(1,maxbin);
    hScale2EmcalvsCent->GetXaxis()->SetRange(1,maxbin);
    hScale3EmcalvsCent->GetXaxis()->SetRange(1,maxbin);
    hScaleShift1EmcalvsCent->GetXaxis()->SetRange(1,maxbin);
    hScaleShift2EmcalvsCent->GetXaxis()->SetRange(1,maxbin);
    hScaleShiftMeanEmcalvsCent->GetXaxis()->SetRange(1,maxbin);
    fHistPtTPCvsCent->GetXaxis()->SetRange(1,maxbin);
    fHistPtEMCALvsCent->GetXaxis()->SetRange(1,maxbin);
    fHistTrackPtvsCent->GetXaxis()->SetRange(1,maxbin);
    
    TH1D *hScaleTPCDist = (TH1D*)hScalevsCent->ProjectionY("hScaleTPCDist");
    TH1D *hScaleEMCALDist = (TH1D*)hScaleEmcalvsCent->ProjectionY("hScaleEMCALDist");
    TH1D *hScale2EMCALDist = (TH1D*)hScale2EmcalvsCent->ProjectionY("hScale2EMCALDist");
    TH1D *hScale3EMCALDist = (TH1D*)hScale3EmcalvsCent->ProjectionY("hScale3EMCALDist");
    TH1D *hScaleShift1EMCALDist = (TH1D*)hScaleShift1EmcalvsCent->ProjectionY("hScaleShift1EMCALDist");
    TH1D *hScaleShift2EMCALDist = (TH1D*)hScaleShift2EmcalvsCent->ProjectionY("hScaleShift2EMCALDist");
    TH1D *hScaleShiftMeanEMCALDist = (TH1D*)hScaleShiftMeanEmcalvsCent->ProjectionY("hScaleShiftMeanEMCALDist");
    TH1D *hScaleEmcalvsPhiDist[8];

    hScaleTPCDist->Scale(1./hScaleTPCDist->Integral());
    hScaleEMCALDist->Scale(1./hScaleEMCALDist->Integral());
    hScale2EMCALDist->Scale(1./hScale2EMCALDist->Integral());
    hScale3EMCALDist->Scale(1./hScale3EMCALDist->Integral());
    hScaleShift1EMCALDist->Scale(1./hScaleShift1EMCALDist->Integral());
    hScaleShift2EMCALDist->Scale(1./hScaleShift2EMCALDist->Integral());
    hScaleShiftMeanEMCALDist->Scale(1./hScaleShiftMeanEMCALDist->Integral());

    for(int b3 = 1; b3 <= fHistScaleEmcalvsPhi->GetNbinsX(); b3++){
        hScaleEmcalvsPhiDist[b3-1] = (TH1D*)fHistScaleEmcalvsPhi->ProjectionY(Form("hScaleEmcalvsPhiDist_%d",b3),b3,b3);
        hScaleEmcalvsPhiDist[b3-1]->Scale(1./hScaleEmcalvsPhiDist[b3-1]->Integral());
    }

    dummy1d->GetYaxis()->SetRangeUser(1.2,2.8);
    dummy1d->Draw("axis");

    hScalevsCent1D->SetMarkerStyle(styles[0]);
    hScalevsCent1D->SetMarkerColor(colors[0]);
    hScalevsCent1D->SetLineColor(colors[0]);
    hScalevsCent1D->Draw("p,e,same");
    legend->AddEntry(hScalevsCent1D,Form("<TPC Acceptance> = %f", meanScalevsCent),"p");

    hScaleEmcalvsCent1D->SetMarkerStyle(styles[1]);
    hScaleEmcalvsCent1D->SetMarkerColor(colors[1]);
    hScaleEmcalvsCent1D->SetLineColor(colors[1]);
    hScaleEmcalvsCent1D->Draw("p,e,same");
    legend->AddEntry(hScaleEmcalvsCent1D,Form("<EMCal Acceptance> = %f", meanScaleEmcalvsCent),"p");

    hScale2EmcalvsCent1D->SetMarkerStyle(styles[2]);
    hScale2EmcalvsCent1D->SetMarkerColor(colors[2]);
    hScale2EmcalvsCent1D->SetLineColor(colors[2]);
    hScale2EmcalvsCent1D->Draw("p,e,same");
    legend->AddEntry(hScale2EmcalvsCent1D,Form("<2xEMCal Acceptance> = %f", meanScale2EmcalvsCent),"p");

    hScale3EmcalvsCent1D->SetMarkerStyle(styles[3]);
    hScale3EmcalvsCent1D->SetMarkerColor(colors[3]);
    hScale3EmcalvsCent1D->SetLineColor(colors[3]);
    hScale3EmcalvsCent1D->Draw("p,e,same");
    legend->AddEntry(hScale3EmcalvsCent1D,Form("<3xEMCal Acceptance> = %f", meanScale3EmcalvsCent),"p");

    hScaleShift1EmcalvsCent1D->SetMarkerStyle(styles[4]);
    hScaleShift1EmcalvsCent1D->SetMarkerColor(colors[4]);
    hScaleShift1EmcalvsCent1D->SetLineColor(colors[4]);
    hScaleShift1EmcalvsCent1D->Draw("p,e,same");
    legend->AddEntry(hScaleShift1EmcalvsCent1D,Form("<1xShifted EMCal Acceptance> = %f", meanScaleShift1EmcalvsCent),"p");

    hScaleShift2EmcalvsCent1D->SetMarkerStyle(styles[5]);
    hScaleShift2EmcalvsCent1D->SetMarkerColor(colors[5]);
    hScaleShift2EmcalvsCent1D->SetLineColor(colors[5]);
    hScaleShift2EmcalvsCent1D->Draw("p,e,same");
    legend->AddEntry(hScaleShift2EmcalvsCent1D,Form("<2xShifted EMCal Acceptance> = %f", meanScaleShift2EmcalvsCent),"p");

    hScaleShiftMeanEmcalvsCent1D->SetMarkerStyle(styles[6]);
    hScaleShiftMeanEmcalvsCent1D->SetMarkerColor(colors[6]);
    hScaleShiftMeanEmcalvsCent1D->SetLineColor(colors[6]);
    hScaleShiftMeanEmcalvsCent1D->Draw("p,e,same");
    legend->AddEntry(hScaleShiftMeanEmcalvsCent1D,Form("<Mean Shift EMCal Acceptance> = %f", meanScaleShiftMeanEmcalvsCent),"p");

    DrawGammaLines(0,100,meanScalePhi,meanScalePhi,4.,1,1);
    legend->AddEntry(fHistScaleEmcalvsPhi1D,Form("<#pi/4 TPC Sections> = %f", meanScalePhi),"l");

    legend->Draw("same");
    canvas->SaveAs(Form("%s/hScalevsCent1D.%s",outputdir.Data(),filetype.Data()));

    legend->Clear();
    TLegend *legend2 =  GetAndSetLegend2(0.65,0.9-(2)*textSize,0.85,0.9,textSize);

    TH1D *dummy1dpt = new TH1D("dummy1dpt","",100,0,100);
    dummy1dpt->GetYaxis()->SetRangeUser(0,1);
    dummy1dpt->GetXaxis()->SetRangeUser(0,110);
    SetStyleHistoTH1ForGraphs(dummy1dpt,"","Centrality (%)","<p_{T}>",textSize,0.035,textSize,0.035,1,1.2);
    dummy1dpt->Draw("axis");

    fHistPtTPCvsCent1D->SetMarkerStyle(styles[0]);
    fHistPtTPCvsCent1D->SetMarkerColor(colors[0]);
    fHistPtTPCvsCent1D->SetLineColor(colors[0]);
    //fHistPtTPCvsCent1D->Draw("p,e,same");
    //legend2->AddEntry(fHistPtTPCvsCent1D,"Full TPC","p");

    fHistPtEMCALvsCent1D->SetMarkerStyle(styles[1]);
    fHistPtEMCALvsCent1D->SetMarkerColor(colors[1]);
    fHistPtEMCALvsCent1D->SetLineColor(colors[1]);
    //fHistPtEMCALvsCent1D->Draw("p,e,same");
    //legend2->AddEntry(fHistPtEMCALvsCent1D,"EMCal Acceptance","p");

    fHistTrackPtvsCent1D->SetMarkerStyle(styles[2]);
    fHistTrackPtvsCent1D->SetMarkerColor(colors[2]);
    fHistTrackPtvsCent1D->SetLineColor(colors[2]);
    fHistTrackPtvsCent1D->Draw("p,e,same");
    legend2->AddEntry(fHistTrackPtvsCent1D,"Tracks","p");

    legend2->Draw("same");
    canvas->SaveAs(Form("%s/fHistPtTPCvsCent1D.%s",outputdir.Data(),filetype.Data()));

    TH1D *dummy1dphi = new TH1D("dummy1dphi","",100,0,100);
    dummy1dpt->GetYaxis()->SetRangeUser(0,5);
    dummy1dpt->GetXaxis()->SetRangeUser(0,2*TMath::Pi());
    SetStyleHistoTH1ForGraphs(dummy1dpt,"","#phi","<SF>",textSize,0.035,textSize,0.035,1,1.2);
    dummy1dpt->Draw("axis");

    fHistScaleEmcalvsPhi1D->SetMarkerStyle(styles[6]);
    fHistScaleEmcalvsPhi1D->SetMarkerColor(colors[6]);
    fHistScaleEmcalvsPhi1D->SetLineColor(colors[6]);
    fHistScaleEmcalvsPhi1D->Draw("text,same");

    drawLatexAdd(Form("Mean = %f", meanScalePhi),0.42,0.19, 0.03,kFALSE, kFALSE, false);

    canvas->SaveAs(Form("%s/fHistScaleEmcalvsPhi1D.%s",outputdir.Data(),filetype.Data()));

    legend2->Clear();

    legend2 =  GetAndSetLegend2(0.55,0.9-(7)*textSize,0.75,0.9,textSize);


    TH1D *dummyDist = new TH1D("dummyDist","",100,0,5);
    dummyDist->GetYaxis()->SetRangeUser(0,1.4e-2);

    SetStyleHistoTH1ForGraphs(dummyDist,"","<SF>","Normalized Counts",textSize,0.035,textSize,0.035,1,1.6);

    dummyDist->Draw("axis");
    
    hScaleTPCDist->SetMarkerStyle(styles[0]);
    hScaleTPCDist->SetMarkerColor(colors[0]);
    hScaleTPCDist->SetLineColor(colors[0]);
    hScaleTPCDist->Draw("p,e,same");
    legend2->AddEntry(hScaleTPCDist,Form("Max TPC = %0.3f +/- %f", hScaleTPCDist->GetBinCenter(hScaleTPCDist->GetMaximumBin()), hScaleTPCDist->GetBinError(hScaleTPCDist->GetMaximumBin())),"p");

    hScaleEMCALDist->SetMarkerStyle(styles[1]);
    hScaleEMCALDist->SetMarkerColor(colors[1]);
    hScaleEMCALDist->SetLineColor(colors[1]);
    hScaleEMCALDist->Draw("p,e,same");
    legend2->AddEntry(hScaleEMCALDist,Form("Max EMCal = %0.3f", hScaleEMCALDist->GetBinCenter(hScaleEMCALDist->GetMaximumBin())),"p");

    hScale2EMCALDist->SetMarkerStyle(styles[2]);
    hScale2EMCALDist->SetMarkerColor(colors[2]);
    hScale2EMCALDist->SetLineColor(colors[2]);
    hScale2EMCALDist->Draw("p,e,same");
    legend2->AddEntry(hScale2EMCALDist,Form("Max 2xEMCal = %0.3f", hScale2EMCALDist->GetBinCenter(hScale2EMCALDist->GetMaximumBin())),"p");

    hScale3EMCALDist->SetMarkerStyle(styles[3]);
    hScale3EMCALDist->SetMarkerColor(colors[3]);
    hScale3EMCALDist->SetLineColor(colors[3]);
    hScale3EMCALDist->Draw("p,e,same");
    legend2->AddEntry(hScale3EMCALDist,Form("Max 3xEMCal = %0.3f", hScale3EMCALDist->GetBinCenter(hScale3EMCALDist->GetMaximumBin())),"p");

    hScaleShift1EMCALDist->SetMarkerStyle(styles[4]);
    hScaleShift1EMCALDist->SetMarkerColor(colors[4]);
    hScaleShift1EMCALDist->SetLineColor(colors[4]);
    hScaleShift1EMCALDist->Draw("p,e,same");
    legend2->AddEntry(hScaleShift1EMCALDist,Form("Max EMCal,S1 = %0.3f", hScaleShift1EMCALDist->GetBinCenter(hScaleShift1EMCALDist->GetMaximumBin())),"p");

    hScaleShift2EMCALDist->SetMarkerStyle(styles[5]);
    hScaleShift2EMCALDist->SetMarkerColor(colors[5]);
    hScaleShift2EMCALDist->SetLineColor(colors[5]);
    hScaleShift2EMCALDist->Draw("p,e,same");
    legend2->AddEntry(hScaleShift2EMCALDist,Form("Max EMCal,S2 = %0.3f", hScaleShift2EMCALDist->GetBinCenter(hScaleShift2EMCALDist->GetMaximumBin())),"p");

    hScaleShiftMeanEMCALDist->SetMarkerStyle(styles[6]);
    hScaleShiftMeanEMCALDist->SetMarkerColor(colors[6]);
    hScaleShiftMeanEMCALDist->SetLineColor(colors[6]);
    hScaleShiftMeanEMCALDist->Draw("p,e,same");
    legend2->AddEntry(hScaleShiftMeanEMCALDist,Form("Max EMCal,SMean = %0.3f", hScaleShiftMeanEMCALDist->GetBinCenter(hScaleShiftMeanEMCALDist->GetMaximumBin())),"p");

    legend2->Draw("same");
    canvas->SaveAs(Form("%s/hScaleShift1DDistros.%s",outputdir.Data(),filetype.Data()));
    legend2->Clear();

    TString sectors[9] = {"0", "#pi/4", "#pi/2", "3#pi/4", "#pi", "5#pi/4", "3#pi/2", "7#pi/4", "2#pi"};

    dummyDist->Draw("axis");
    for(int b4 = 0; b4 < 8; b4++){
        hScaleEmcalvsPhiDist[b4]->SetMarkerStyle(styles[b4]);
        hScaleEmcalvsPhiDist[b4]->SetMarkerColor(colors[b4]);
        hScaleEmcalvsPhiDist[b4]->SetLineColor(colors[b4]);
        hScaleEmcalvsPhiDist[b4]->Draw("p,e,same");
        legend2->AddEntry(hScaleEmcalvsPhiDist[b4],Form("%s < #phi < %s : Max = %0.3f", sectors[b4].Data(), sectors[b4+1].Data(), hScaleEmcalvsPhiDist[b4]->GetBinCenter(hScaleEmcalvsPhiDist[b4]->GetMaximumBin())),"p");
    }

    legend2->Draw("same");
    canvas->SaveAs(Form("%s/hScaleEmcalvsPhiDist.%s",outputdir.Data(),filetype.Data()));
    legend2->Clear();


    f->Close();
    delete f;
    delete canvas;
    delete dummy1d;
    delete dummy1dpt;
    delete dummy1detaphi;
    delete legend;
    delete legend2;
    delete hScalevsCent;
    delete hScaleEmcalvsCent;
    delete hScale2EmcalvsCent;
    delete hScalevsCent1D;
    delete hScaleEmcalvsCent1D;
    delete hScale2EmcalvsCent1D;

    return;
}
