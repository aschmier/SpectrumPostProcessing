#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"

void plotDpTCombined(TString filenameppcharged, TString filenameppfull, TString filenamepPbcharged, TString filenamepPbfull, TString outputdir, TString fileType, TString suffixcharged, TString suffixfull, int minradius, int maxradiuspp, int maxradiuspPb)
{
    double textSize  = 0.03;
    TString jetType  = "Full";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    std::vector<TH1D*> vecDpTppch;
    std::vector<TH1D*> vecDpTpPbch;
    std::vector<TH1D*> vecRhoppch;
    std::vector<TH1D*> vecRhopPbch;
    std::vector<TH1D*> vecDpTppfu;
    std::vector<TH1D*> vecDpTpPbfu;
    std::vector<TH1D*> vecRhoppfu;
    std::vector<TH1D*> vecRhopPbfu;

    gSystem->Exec("mkdir -p "+outputdir);

    TFile *fppch = TFile::Open(filenameppcharged.Data());
    if(!fppch || fppch->IsZombie()){
        cout << "File pp charged not found!" << endl;
        return;
    }

    TFile *fppfu = TFile::Open(filenameppfull.Data());
    if(!fppfu || fppfu->IsZombie()){
        cout << "File pp full not found!" << endl;
        return;
    }

    TFile *fpPbch = TFile::Open(filenamepPbcharged.Data());
    if(!fpPbch || fpPbch->IsZombie()){
        cout << "File pPb charged not found!" << endl;
        return;
    }

    TFile *fpPbfu = TFile::Open(filenamepPbfull.Data());
    if(!fpPbfu || fpPbfu->IsZombie()){
        cout << "File pPb full not found!" << endl;
        return;
    }

    TCanvas* canvas       = new TCanvas("canvas", "", 1000, 1000);
    double leftMargin   = 0.1;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    //TH1D *hDummy = new TH1D("hDummy","",170,-20,150);

    TLegend *legend =  GetAndSetLegend2(0.73,0.74-(4)*textSize,0.88,0.74,textSize);
    TLegend *legendmean = GetAndSetLegend2(0.13,0.28-(3)*textSize,0.33,0.28,textSize);
    // loop over radii
    for(int radius = minradius; radius <= maxradiuspp; radius++){
            TString dirnamech     = Form("JetSpectrum_FullJets_R0%i_INT7_%s", radius, suffixcharged.Data());
            TString dirnamefu     = Form("JetSpectrum_FullJets_R0%i_INT7_%s", radius, suffixfull.Data());

            TDirectory *dirppch = (TDirectory*)fppch->Get(dirnamech.Data());
            TList *listppch     = (TList*)dirppch->Get(dirnamech.Data());
            TH2D *hDpT2Dppch    = (TH2D*)listppch->FindObject("hRhoVsDeltaPtRC");
            TH1D *hDpTppch      = (TH1D*)hDpT2Dppch->ProjectionX(Form("hDpT_pp_ch_%i",radius),1,100);
            hDpTppch->Scale(1./hDpTppch->Integral());
            SetStyleHistoTH1ForGraphs(hDpTppch,"","#delta#it{p}_{T}^{RC} (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);
            TH1D *hRhoppch      = (TH1D*)hDpT2Dppch->ProjectionY(Form("hRho_pp_ch_%i",radius),1,hDpT2Dppch->GetNbinsX()-1);
            hRhoppch->Scale(1./hRhoppch->Integral());
            SetStyleHistoTH1ForGraphs(hRhoppch,"","#rho (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);

            vecDpTppch.push_back(hDpTppch);
            vecRhoppch.push_back(hRhoppch);

            TDirectory *dirppfu = (TDirectory*)fppfu->Get(dirnamefu.Data());
            TList *listppfu     = (TList*)dirppfu->Get(dirnamefu.Data());
            TH2D *hDpT2Dppfu    = (TH2D*)listppfu->FindObject("hRhoVsDeltaPtRC");
            TH1D *hDpTppfu      = (TH1D*)hDpT2Dppfu->ProjectionX(Form("hDpT_pp_fu_%i",radius),1,100);
            hDpTppfu->Scale(1./hDpTppfu->Integral());
            SetStyleHistoTH1ForGraphs(hDpTppfu,"","#delta#it{p}_{T}^{RC} (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);
            TH1D *hRhoppfu      = (TH1D*)hDpT2Dppfu->ProjectionY(Form("hRho_pp_fu_%i",radius),1,hDpT2Dppfu->GetNbinsX()-1);
            hRhoppfu->Scale(1./hRhoppfu->Integral());
            SetStyleHistoTH1ForGraphs(hRhoppfu,"","#rho (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);

            vecDpTppfu.push_back(hDpTppfu);
            vecRhoppfu.push_back(hRhoppfu);

            if(radius == maxradiuspp) continue;

            TDirectory *dirpPbch = (TDirectory*)fpPbch->Get(dirnamech.Data());
            TList *listpPbch     = (TList*)dirpPbch->Get(dirnamech.Data());
            TH2D *hDpT2DpPbch    = (TH2D*)listpPbch->FindObject("hRhoVsDeltaPtRC");
            TH1D *hDpTpPbch      = (TH1D*)hDpT2DpPbch->ProjectionX(Form("hDpT_pPb_ch_%i",radius),1,100);
            hDpTpPbch->Scale(1./hDpTpPbch->Integral());
            SetStyleHistoTH1ForGraphs(hDpTpPbch,"","#delta#it{p}_{T}^{RC} (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);
            TH1D *hRhopPbch      = (TH1D*)hDpT2DpPbch->ProjectionY(Form("hRho_pPb_ch_%i",radius),1,hDpT2DpPbch->GetNbinsX()-1);
            hRhopPbch->Scale(1./hRhopPbch->Integral());
            SetStyleHistoTH1ForGraphs(hRhopPbch,"","#rho (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);

            vecDpTpPbch.push_back(hDpTpPbch);
            vecRhopPbch.push_back(hRhopPbch);

            TDirectory *dirpPbfu = (TDirectory*)fpPbfu->Get(dirnamefu.Data());
            TList *listpPbfu     = (TList*)dirpPbfu->Get(dirnamefu.Data());
            TH2D *hDpT2DpPbfu    = (TH2D*)listpPbfu->FindObject("hRhoVsDeltaPtRC");
            TH1D *hDpTpPbfu      = (TH1D*)hDpT2DpPbfu->ProjectionX(Form("hDpT_pPb_fu_%i",radius),1,100);
            hDpTpPbfu->Scale(1./hDpTpPbfu->Integral());
            SetStyleHistoTH1ForGraphs(hDpTpPbfu,"","#delta#it{p}_{T}^{RC} (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);
            TH1D *hRhopPbfu      = (TH1D*)hDpT2DpPbfu->ProjectionY(Form("hRho_pPb_fu_%i",radius),1,hDpT2DpPbfu->GetNbinsX()-1);
            hRhopPbfu->Scale(1./hRhopPbfu->Integral());
            SetStyleHistoTH1ForGraphs(hRhopPbfu,"","#rho (GeV/#it{c})","Probability Distribution",textSize,0.035,textSize,0.035,1,1.2);

            vecDpTpPbfu.push_back(hDpTpPbfu);
            vecRhopPbfu.push_back(hRhopPbfu);
    }

    for(int radius = minradius; radius <= maxradiuspp; radius++){
        vecDpTppch.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecDpTppch.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecDpTppch.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecDpTppch.at(radius-minradius)->SetMarkerSize(1.5);
        //vecDpTppch.at(radius-minradius)->Draw(radius==minradius ? "p,e" : "p,e,same");
        if(radius==minradius) vecDpTppch.at(radius-minradius)->Draw("p,e");
        else vecDpTppch.at(radius-minradius)->Draw("p,e,same");

        legend->AddEntry(vecDpTppch.at(radius-minradius),Form("pp, R = 0.%i",radius),"p");
    }
    legend->Draw();
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Charged (TPC) Rho",0.93,0.75,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotDpT_pp_charged.%s",outputdir.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradiuspp; radius++){
        vecDpTppfu.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecDpTppfu.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecDpTppfu.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecDpTppfu.at(radius-minradius)->SetMarkerSize(1.5);
        if(radius==minradius) vecDpTppfu.at(radius-minradius)->Draw("p,e");
        else vecDpTppfu.at(radius-minradius)->Draw("p,e,same");
        legend->AddEntry(vecDpTppfu.at(radius-minradius),Form("pp, R = 0.%i",radius),"p");
    }
    legend->Draw();
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full (EMCal) Rho",0.93,0.75,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotDpT_pp_full.%s",outputdir.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradiuspPb; radius++){
        vecDpTpPbch.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecDpTpPbch.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecDpTpPbch.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecDpTpPbch.at(radius-minradius)->SetMarkerSize(1.5);
        if(radius==minradius) vecDpTpPbch.at(radius-minradius)->Draw("p,e");
        else vecDpTpPbch.at(radius-minradius)->Draw("p,e,same");
        legend->AddEntry(vecDpTpPbch.at(radius-minradius),Form("pPb, R = 0.%i",radius),"p");
    }
    legend->Draw();
    drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Charged (TPC) Rho",0.93,0.75,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotDpT_pPb_charged.%s",outputdir.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradiuspPb; radius++){
        vecDpTpPbfu.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecDpTpPbfu.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecDpTpPbfu.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecDpTpPbfu.at(radius-minradius)->SetMarkerSize(1.5);
        if(radius==minradius) vecDpTpPbfu.at(radius-minradius)->Draw("p,e");
        else vecDpTpPbfu.at(radius-minradius)->Draw("p,e,same");
        legend->AddEntry(vecDpTpPbfu.at(radius-minradius),Form("pPb, R = 0.%i",radius),"p");
    }
    legend->Draw();
    drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full (EMCal) Rho",0.93,0.75,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotDpT_pPb_full.%s",outputdir.Data(),fileType.Data()));
    legend->Clear();

    ///////////////////////////////////////////////////////////////////////////////////////////////

    for(int radius = minradius; radius <= maxradiuspp; radius++){
        vecRhoppch.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecRhoppch.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecRhoppch.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecRhoppch.at(radius-minradius)->SetMarkerSize(1.5);
        if(radius==minradius) vecRhoppch.at(radius-minradius)->Draw("p,e");
        else vecRhoppch.at(radius-minradius)->Draw("p,e,same");
        legend->AddEntry(vecRhoppch.at(radius-minradius),Form("pp, R = 0.%i",radius),"p");
    }
    legend->Draw();
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Charged (TPC) Rho",0.93,0.75,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotRho_pp_charged.%s",outputdir.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradiuspp; radius++){
        vecRhoppfu.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecRhoppfu.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecRhoppfu.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecRhoppfu.at(radius-minradius)->SetMarkerSize(1.5);
        if(radius==minradius) vecRhoppfu.at(radius-minradius)->Draw("p,e");
        else vecRhoppfu.at(radius-minradius)->Draw("p,e,same");
        legend->AddEntry(vecRhoppfu.at(radius-minradius),Form("pp, R = 0.%i",radius),"p");
    }
    legend->Draw();
    drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full (EMCal) Rho",0.93,0.75,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotRho_pp_full.%s",outputdir.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradiuspPb; radius++){
        vecRhopPbch.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecRhopPbch.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecRhopPbch.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecRhopPbch.at(radius-minradius)->SetMarkerSize(1.5);
        if(radius==minradius) vecRhopPbch.at(radius-minradius)->Draw("p,e");
        else vecRhopPbch.at(radius-minradius)->Draw("p,e,same");
        legend->AddEntry(vecRhopPbch.at(radius-minradius),Form("pPb, R = 0.%i",radius),"p");
    }
    legend->Draw();
    drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Charged (TPC) Rho",0.93,0.75,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotRho_pPb_charged.%s",outputdir.Data(),fileType.Data()));
    legend->Clear();

    for(int radius = minradius; radius <= maxradiuspPb; radius++){
        vecRhopPbfu.at(radius-minradius)->SetMarkerColor(colors[radius-minradius]);
        vecRhopPbfu.at(radius-minradius)->SetLineColor(colors[radius-minradius]);
        vecRhopPbfu.at(radius-minradius)->SetMarkerStyle(styles[radius-minradius]);
        vecRhopPbfu.at(radius-minradius)->SetMarkerSize(1.5);
        if(radius==minradius) vecRhopPbfu.at(radius-minradius)->Draw("p,e");
        else vecRhopPbfu.at(radius-minradius)->Draw("p,e,same");
        legend->AddEntry(vecRhopPbfu.at(radius-minradius),Form("pPb, R = 0.%i",radius),"p");
    }
    legend->Draw();
    drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
    drawLatexAdd("Full (EMCal) Rho",0.93,0.75,textSize,kFALSE, kFALSE, true);
    canvas->SaveAs(Form("%s/plotRho_pPb_full.%s",outputdir.Data(),fileType.Data()));
    legend->Clear();

    ///////////////////////////////////////////////////////////////////////////////////////////////

    for(int radius = minradius; radius <= maxradiuspPb; radius++){
        vecDpTppch.at(radius-minradius)->SetMarkerColor(colors[0]);
        vecDpTppch.at(radius-minradius)->SetLineColor(colors[0]);
        vecDpTppch.at(radius-minradius)->SetMarkerStyle(styles[0]);
        vecDpTppch.at(radius-minradius)->SetMarkerSize(1.5);
        vecDpTpPbch.at(radius-minradius)->SetMarkerColor(colors[1]);
        vecDpTpPbch.at(radius-minradius)->SetLineColor(colors[1]);
        vecDpTpPbch.at(radius-minradius)->SetMarkerStyle(styles[1]);
        vecDpTpPbch.at(radius-minradius)->SetMarkerSize(1.5);
        vecDpTppfu.at(radius-minradius)->SetMarkerColor(colors[2]);
        vecDpTppfu.at(radius-minradius)->SetLineColor(colors[2]);
        vecDpTppfu.at(radius-minradius)->SetMarkerStyle(styles[2]);
        vecDpTppfu.at(radius-minradius)->SetMarkerSize(1.5);
        vecDpTpPbfu.at(radius-minradius)->SetMarkerColor(colors[3]);
        vecDpTpPbfu.at(radius-minradius)->SetLineColor(colors[3]);
        vecDpTpPbfu.at(radius-minradius)->SetMarkerStyle(styles[3]);
        vecDpTpPbfu.at(radius-minradius)->SetMarkerSize(1.5);
        vecDpTppch.at(radius-minradius)->Draw("p,e");
        legend->AddEntry(vecDpTppch.at(radius-minradius),"pp Charged","p");
        vecDpTppfu.at(radius-minradius)->Draw( "p,e,same");
        legend->AddEntry(vecDpTppfu.at(radius-minradius),"pp Full","p");
        vecDpTpPbch.at(radius-minradius)->Draw( "p,e,same");
        legend->AddEntry(vecDpTpPbch.at(radius-minradius),"pPb Charged","p");
        vecDpTpPbfu.at(radius-minradius)->Draw( "p,e,same");
        legend->AddEntry(vecDpTpPbfu.at(radius-minradius),"pPb Full","p");
        legend->Draw();
        drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("R = 0.%i", radius),0.93,0.78,textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/plotDpT_pp_pPb_charged_full_R0%i.%s",outputdir.Data(),radius,fileType.Data()));
        legend->Clear();
    }

    for(int radius = minradius; radius <= maxradiuspPb; radius++){
        vecRhoppch.at(radius-minradius)->SetMarkerColor(colors[0]);
        vecRhoppch.at(radius-minradius)->SetLineColor(colors[0]);
        vecRhoppch.at(radius-minradius)->SetMarkerStyle(styles[0]);
        vecRhoppch.at(radius-minradius)->SetMarkerSize(1.5);
        vecRhopPbch.at(radius-minradius)->SetMarkerColor(colors[1]);
        vecRhopPbch.at(radius-minradius)->SetLineColor(colors[1]);
        vecRhopPbch.at(radius-minradius)->SetMarkerStyle(styles[1]);
        vecRhopPbch.at(radius-minradius)->SetMarkerSize(1.5);
        vecRhoppfu.at(radius-minradius)->SetMarkerColor(colors[2]);
        vecRhoppfu.at(radius-minradius)->SetLineColor(colors[2]);
        vecRhoppfu.at(radius-minradius)->SetMarkerStyle(styles[2]);
        vecRhoppfu.at(radius-minradius)->SetMarkerSize(1.5);
        vecRhopPbfu.at(radius-minradius)->SetMarkerColor(colors[3]);
        vecRhopPbfu.at(radius-minradius)->SetLineColor(colors[3]);
        vecRhopPbfu.at(radius-minradius)->SetMarkerStyle(styles[3]);
        vecRhopPbfu.at(radius-minradius)->SetMarkerSize(1.5);
        vecRhoppch.at(radius-minradius)->Draw("p,e");
        legend->AddEntry(vecRhoppch.at(radius-minradius),"pp Charged","p");
        vecRhoppfu.at(radius-minradius)->Draw( "p,e,same");
        legend->AddEntry(vecRhoppfu.at(radius-minradius),"pp Full","p");
        vecRhopPbch.at(radius-minradius)->Draw( "p,e,same");
        legend->AddEntry(vecRhopPbch.at(radius-minradius),"pPb Charged","p");
        vecRhopPbfu.at(radius-minradius)->Draw( "p,e,same");
        legend->AddEntry(vecRhopPbfu.at(radius-minradius),"pPb Full","p");
        legend->Draw();
        drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.93,0.9, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("R = 0.%i", radius),0.93,0.78,textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/plotRho_pp_pPb_charged_full_R0%i.%s",outputdir.Data(),radius,fileType.Data()));
        legend->Clear();
    }
}
