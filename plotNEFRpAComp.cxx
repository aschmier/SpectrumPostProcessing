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
//#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"
//#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningZg.C"

std::vector<double> getRebin(){
  std::vector<double> result;
  result.push_back(0.);
  for(double d = 0.1; d <= 1; d += 0.1) result.push_back(d);
  return result;
}

void plotNEFRpAComp(TString mb_file, TString emc7_file, TString eje_file, TString mc_file, TString pPb_file, TString pPb_file_mc, TString outputdir, int minradius = 2, int maxradius = 6)
{
    Double_t textSize     = 0.03;
    TString jetType = "Full";
    TString fileType = "pdf";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    vector<TFile*> files_pp;
    vector<TString> triggers_pp{"INT7","EMC7","EJE"};
    vector<TString> triggers_pPb{"INT7","EJ2","EJ1"};
    vector<TH2D*> vecNEFpp[maxradius-minradius+1];
    vector<TH2D*> vecNEFpp_MC[maxradius-minradius+1];
    vector<TH2D*> vecNEFpPb[maxradius-minradius+1];
    vector<TH2D*> vecNEFpPb_MC[maxradius-minradius+1];

    vector<double> detLevelRebin = getRebin();

    const Int_t nPtBins     = 6;
    Int_t binsPt[7]        = {6, 10, 30, 60, 100, 200, 350};
    Double_t binsPtdoub[7]        = {6., 10., 30., 60., 100., 200., 350.};

    TFile *fmb = TFile::Open(mb_file);
    if(!fmb || fmb->IsZombie()){
        cout << "INT7 file not found!" << endl;
        return;
    }
    files_pp.push_back(fmb);

    TFile *femc7 = TFile::Open(emc7_file);
    if(!femc7 || femc7->IsZombie()){
        cout << "EMC7 file not found!" << endl;
        return;
    }
    files_pp.push_back(femc7);

    TFile *feje = TFile::Open(eje_file);
    if(!feje || feje->IsZombie()){
        cout << "EJE file not found!" << endl;
        return;
    }
    files_pp.push_back(feje);

    TFile *fmc = TFile::Open(mc_file);
    if(!fmc || fmc->IsZombie()){
        cout << "MC file not found!" << endl;
        return;
    }

    TFile *fppb = TFile::Open(pPb_file);
    if(!fppb || fppb->IsZombie()){
        cout << "pPb file not found!" << endl;
        return;
    }

    TFile *fppbmc = TFile::Open(pPb_file_mc);
    if(!fppbmc || fppbmc->IsZombie()){
        cout << "pPb MC file not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        // Store pp histos
        for(int trigger = 0; trigger < triggers_pp.size(); trigger++){
            TString dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers_pp.at(trigger).Data());
            TDirectory *dir = (TDirectory*)files_pp.at(trigger)->Get(dirname.Data());
            if(!dir){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers_pp.at(trigger).Data());
                dir = (TDirectory*)files_pp.at(trigger)->Get(dirname.Data());
            }
            TList *list = (TList*)dir->Get(dirname.Data());
            TH1D *events = (TH1D*)list->FindObject("hClusterCounter");
            TH2D *NEF2d = (TH2D*)list->FindObject("hQANEFPt");
            //NEF2d->Scale(1/events->GetBinContent(1));
            vecNEFpp[radius-minradius].push_back(NEF2d);

            dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers_pp.at(trigger).Data());
            TDirectory *dirMC = (TDirectory*)fmc->Get(dirname.Data());
            if(!dirMC){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers_pp.at(trigger).Data());
                dirMC = (TDirectory*)fmc->Get(dirname.Data());
            }
            TList *listMC = (TList*)dirMC->Get(dirname.Data());
            TH1D *eventsMC = (TH1D*)listMC->FindObject("hClusterCounter");
            TH2D *NEF2dMC = (TH2D*)listMC->FindObject("hQANEFPt");
            //NEF2dMC->Scale(1/eventsMC->GetBinContent(1));
            vecNEFpp_MC[radius-minradius].push_back(NEF2dMC);
        }

        // Store pPb histos
        for(int trigger = 0; trigger < triggers_pPb.size(); trigger++){
            TString dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers_pPb.at(trigger).Data());
            TDirectory *dir = (TDirectory*)fppb->Get(dirname.Data());
            if(!dir){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers_pPb.at(trigger).Data());
                dir = (TDirectory*)fppb->Get(dirname.Data());
            }
            TList *list = (TList*)dir->Get(dirname.Data());
            TH1D *events = (TH1D*)list->FindObject("hClusterCounter");
            TH2D *NEF2d = (TH2D*)list->FindObject("hQANEFPt");
            //NEF2d->Scale(1/events->GetBinContent(1));
            vecNEFpPb[radius-minradius].push_back(NEF2d);

            dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers_pPb.at(trigger).Data());
            TDirectory *dirMC = (TDirectory*)fppbmc->Get(dirname.Data());
            if(!dirMC){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers_pPb.at(trigger).Data());
                dirMC = (TDirectory*)fppbmc->Get(dirname.Data());
            }
            TList *listMC = (TList*)dirMC->Get(dirname.Data());
            TH1D *eventsMC = (TH1D*)listMC->FindObject("hClusterCounter");
            TH2D *NEF2dMC = (TH2D*)listMC->FindObject("hQANEFPt");
            //NEF2dMC->Scale(1/eventsMC->GetBinContent(1));
            vecNEFpPb_MC[radius-minradius].push_back(NEF2dMC);
        }
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.075;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend =  GetAndSetLegend2(0.77,0.65,0.97,0.65+((3)*textSize*1.5),textSize);
    for(int radius = minradius; radius <= maxradius; radius++){
        for(int binPt = 0; binPt < nPtBins; binPt++){
            for(int trigger = 0; trigger < triggers_pp.size(); trigger++){
                bool draw = true;
                if(triggers_pp.at(trigger) == "INT7" && binsPt[binPt] >= 60) draw = false;
                else if(triggers_pp.at(trigger) == "EMC7" && binsPt[binPt] >= 100) draw = false;
                TH1D *tempNEFpp = vecNEFpp[radius-minradius].at(trigger)->ProjectionY(Form("hNEFpp_%s_R0%i_%i-%iGeV", triggers_pp.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                tempNEFpp->Scale(1/tempNEFpp->Integral());
                TH1D *hNEFpp = (TH1D*)tempNEFpp->Rebin(detLevelRebin.size()-1, Form("hNEFpp_R0%i_%s_ptBin%i",radius,triggers_pp.at(trigger).Data(),binPt), detLevelRebin.data());
                TH1D *tempNEFpPb = vecNEFpPb[radius-minradius].at(trigger)->ProjectionY(Form("hNEFpPb_%s_R0%i_%i-%iGeV", triggers_pPb.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                tempNEFpPb->Scale(1/tempNEFpPb->Integral());
                TH1D *hNEFpPb = (TH1D*)tempNEFpPb->Rebin(detLevelRebin.size()-1, Form("hNEFpPb_R0%i_%s_ptBin%i",radius,triggers_pPb.at(trigger).Data(),binPt), detLevelRebin.data());
                hNEFpp->Scale(1.,"width");
                hNEFpPb->Scale(1.,"width");
                hNEFpp->GetYaxis()->SetRangeUser(0,8);
                hNEFpp->GetXaxis()->SetRangeUser(0,1);
                SetStyleHistoTH1ForGraphs(hNEFpp,"","#it{NEF}","1/N^{trig} dN^{jet}/dNEF",0.03,0.04,0.03,0.04,1,0.7);
                hNEFpp->SetMarkerStyle(styles[0]);
                hNEFpp->SetMarkerColor(colors[0]);
                hNEFpp->SetLineColor(colors[0]);
                hNEFpPb->SetMarkerStyle(styles[1]);
                hNEFpPb->SetMarkerColor(colors[1]);
                hNEFpPb->SetLineColor(colors[1]);
                if(draw == true){
                    hNEFpp->Draw("p,e");
                    hNEFpPb->Draw("p,e,same");

                    legend->AddEntry(hNEFpp,Form("pp - %s",triggers_pp.at(trigger).Data()),"p");
                    legend->AddEntry(hNEFpPb,Form("pPb - %s",triggers_pPb.at(trigger).Data()),"p");

                    legend->Draw();
                    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.9,0.88, 0.03, false, false, true);
                    drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.9,0.84, 0.03, false, false, true);
                    drawLatexAdd(Form("#it{p}_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.9,0.8, 0.03, false, false, true);
                    canvas->SaveAs(Form("%s/Data/hNEF_%i-%iGeV_R0%i_%s_%s.%s", outputdir.Data(),binsPt[binPt],binsPt[binPt+1],radius,triggers_pp.at(trigger).Data(),triggers_pPb.at(trigger).Data(),fileType.Data()));
                }
                legend->Clear();
            }

            for(int trigger = 0; trigger < triggers_pp.size(); trigger++){
                bool draw = true;
                if(triggers_pp.at(trigger) == "INT7" && binsPt[binPt] >= 60) draw = false;
                else if(triggers_pp.at(trigger) == "EMC7" && binsPt[binPt] >= 100) draw = false;
                TH1D *tempNEFpp = vecNEFpp_MC[radius-minradius].at(trigger)->ProjectionY(Form("hNEFppMC_%s_R0%i_%i-%iGeV", triggers_pp.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                tempNEFpp->Scale(1/tempNEFpp->Integral());
                TH1D *hNEFpp = (TH1D*)tempNEFpp->Rebin(detLevelRebin.size()-1, Form("hNEFppMC_R0%i_%s_ptBin%i",radius,triggers_pp.at(trigger).Data(),binPt), detLevelRebin.data());
                TH1D *tempNEFpPb = vecNEFpPb_MC[radius-minradius].at(trigger)->ProjectionY(Form("hNEFpPbMC_%s_R0%i_%i-%iGeV", triggers_pPb.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                tempNEFpPb->Scale(1/tempNEFpPb->Integral());
                TH1D *hNEFpPb = (TH1D*)tempNEFpPb->Rebin(detLevelRebin.size()-1, Form("hNEFpPbMC_R0%i_%s_ptBin%i",radius,triggers_pPb.at(trigger).Data(),binPt), detLevelRebin.data());
                hNEFpp->Scale(1.,"width");
                hNEFpPb->Scale(1.,"width");
                hNEFpp->GetYaxis()->SetRangeUser(0,8);
                hNEFpp->GetXaxis()->SetRangeUser(0,1);
                SetStyleHistoTH1ForGraphs(hNEFpp,"","#it{NEF}","1/N^{trig} dN^{jet}/dNEF",0.03,0.04,0.03,0.04,1,0.7);
                hNEFpp->SetMarkerStyle(styles[0]);
                hNEFpp->SetMarkerColor(colors[0]);
                hNEFpp->SetLineColor(colors[0]);
                hNEFpPb->SetMarkerStyle(styles[1]);
                hNEFpPb->SetMarkerColor(colors[1]);
                hNEFpPb->SetLineColor(colors[1]);
                if(draw == true){
                    hNEFpp->Draw("p,e");
                    hNEFpPb->Draw("p,e,same");
                    legend->AddEntry(hNEFpp,Form("pp - %s",triggers_pp.at(trigger).Data()),"p");
                    legend->AddEntry(hNEFpPb,Form("pPb - %s",triggers_pPb.at(trigger).Data()),"p");

                    legend->Draw();
                    drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.9,0.88, 0.03, false, false, true);
                    drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.9,0.84, 0.03, false, false, true);
                    drawLatexAdd(Form("#it{p}_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.9,0.8, 0.03, false, false, true);
                    canvas->SaveAs(Form("%s/Sim/hNEF_%i-%iGeV_R0%i_%s_%s.%s", outputdir.Data(),binsPt[binPt],binsPt[binPt+1],radius,triggers_pp.at(trigger).Data(),triggers_pPb.at(trigger).Data(),fileType.Data()));
                }
                legend->Clear();
            }

        }
    }

}
