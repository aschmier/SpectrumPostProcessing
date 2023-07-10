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

void plotTriggerBias(TString mb_file, TString emc7_file, TString eje_file, TString mc_file, TString outputdir, TString fileType, TString system, int minradius = 2, int maxradius = 6)
{
    Double_t textSize     = 0.03;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    vector<TFile*> files;
    vector<TString> triggers;
    if(system=="pp") triggers = {"INT7","EMC7","EJE"};
    if(system=="pPb") triggers = {"INT7","EJ2","EJ1"};
    vector<TH2D*> vecNEF[maxradius-minradius+1];
    vector<TH2D*> vecZch[maxradius-minradius+1];
    vector<TH2D*> vecZne[maxradius-minradius+1];
    vector<TH2D*> vecNEF_MC[maxradius-minradius+1];
    vector<TH2D*> vecZch_MC[maxradius-minradius+1];
    vector<TH2D*> vecZne_MC[maxradius-minradius+1];

    vector<double> detLevelRebin = getRebin();

    gSystem->Exec("mkdir -p "+outputdir+"/TriggerBias/NEF");
    gSystem->Exec("mkdir -p "+outputdir+"/TriggerBias/Zne");
    gSystem->Exec("mkdir -p "+outputdir+"/TriggerBias/Zch");

    const Int_t nPtBins     = 6;
    Int_t binsPt[7]        = {6, 10, 30, 60, 100, 160, 240};
    Double_t binsPtdoub[7]        = {6., 10., 30., 60., 100., 160., 240.};

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

    TFile *fmc = TFile::Open(mc_file);
    if(!fmc || fmc->IsZombie()){
        cout << "MC file not found!" << endl;
        return;
    }

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
            TH2D *NEF2d = (TH2D*)list->FindObject("hQANEFPt");
            TH2D *Zch2d = (TH2D*)list->FindObject("hQAZchPt");
            TH2D *Zne2d = (TH2D*)list->FindObject("hQAZnePt");
            NEF2d->Scale(1/events->GetBinContent(1));
            Zch2d->Scale(1/events->GetBinContent(1));
            Zne2d->Scale(1/events->GetBinContent(1));
            vecNEF[radius-minradius].push_back(NEF2d);
            vecZch[radius-minradius].push_back(Zch2d);
            vecZne[radius-minradius].push_back(Zne2d);

            TDirectory *dirMC = (TDirectory*)fmc->Get(dirname.Data());
            TList *listMC = (TList*)dirMC->Get(dirname.Data());
            TH1D *eventsMC = (TH1D*)listMC->FindObject("hClusterCounter");
            TH2D *NEF2dMC = (TH2D*)listMC->FindObject("hQANEFPt");
            TH2D *Zch2dMC = (TH2D*)listMC->FindObject("hQAZchPt");
            TH2D *Zne2dMC = (TH2D*)listMC->FindObject("hQAZnePt");
            NEF2dMC->Scale(1/eventsMC->GetBinContent(1));
            Zch2dMC->Scale(1/eventsMC->GetBinContent(1));
            Zne2dMC->Scale(1/eventsMC->GetBinContent(1));
            vecNEF_MC[radius-minradius].push_back(NEF2dMC);
            vecZch_MC[radius-minradius].push_back(Zch2dMC);
            vecZne_MC[radius-minradius].push_back(Zne2dMC);
        }
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.075;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend =  GetAndSetLegend2(0.1,0.65,0.4,0.65+((3)*textSize*1.5),textSize);
    bool draw   = false;
    int drawnum = 0;

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int binPt = 0; binPt < nPtBins; binPt++){
            drawnum = 0;
            for(int trigger = 0; trigger < triggers.size(); trigger++){
                if(triggers.at(trigger) == "INT7" && binsPt[binPt] >= 60) draw = false;
                else if(triggers.at(trigger) == "EMC7" && binsPt[binPt] >= 100) draw = false;
                else draw = true;
                TH1D *tempNEF = vecNEF[radius-minradius].at(trigger)->ProjectionY(Form("hNEF_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                TH1D *tempNEF_MC = vecNEF_MC[radius-minradius].at(trigger)->ProjectionY(Form("hNEF_MC_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                TH1D *hNEF = (TH1D*)tempNEF->Rebin(detLevelRebin.size()-1, Form("hNEF_R0%i_%s_ptBin%i",radius,triggers.at(trigger).Data(),binPt), detLevelRebin.data());
                hNEF->Scale(1.,"width");
                TH1D *hNEF_MC = (TH1D*)tempNEF_MC->Rebin(detLevelRebin.size()-1, Form("hNEF_R0%i_%s_ptBin%i_MC",radius,triggers.at(trigger).Data(),binPt), detLevelRebin.data());
                hNEF_MC->Scale(1.,"width");
                hNEF->Divide(hNEF,hNEF_MC);
                hNEF->GetYaxis()->SetRangeUser(0,12);
                hNEF->GetXaxis()->SetRangeUser(0,1);
                SetStyleHistoTH1ForGraphs(hNEF,"","#it{NEF}","Data/MC",0.03,0.04,0.03,0.04,1,0.7);
                hNEF->SetMarkerStyle(styles[trigger]);
                hNEF->SetMarkerColor(colors[trigger]);
                hNEF->SetLineColor(colors[trigger]);
                if(draw == true && drawnum++ == 0) hNEF->Draw("p,e");
                else hNEF->Draw("p,e,same");
                legend->AddEntry(hNEF,triggers.at(trigger).Data(),"p");
            }

            legend->Draw();
            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.12,0.88, 0.03, false, false, false);
            drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.12,0.84, 0.03, false, false, false);
            drawLatexAdd(Form("#it{p}_{T}^{jet} = %i - %i GeV/#it{c}", binsPt[binPt], binsPt[binPt+1]),0.12,0.8, 0.03, false, false, false);
            canvas->SaveAs(Form("%s/TriggerBias/NEF/hNEF_ptBin%i_R0%i.%s", outputdir.Data(),binPt,radius,fileType.Data()));

            legend->Clear();

            drawnum = 0;
            for(int trigger = 0; trigger < triggers.size(); trigger++){
                if(triggers.at(trigger) == "INT7" && binsPt[binPt] >= 60) draw = false;
                else if(triggers.at(trigger) == "EMC7" && binsPt[binPt] >= 100) draw = false;
                else draw = true;
                TH1D *tempZch = vecZch[radius-minradius].at(trigger)->ProjectionY(Form("hZch_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                TH1D *tempZch_MC = vecZch_MC[radius-minradius].at(trigger)->ProjectionY(Form("hZch_MC_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                TH1D *hZch = (TH1D*)tempZch->Rebin(detLevelRebin.size()-1, Form("hZch_R0%i_%s_ptBin%i",radius,triggers.at(trigger).Data(),binPt), detLevelRebin.data());
                hZch->Scale(1.,"width");
                TH1D *hZch_MC = (TH1D*)tempZch_MC->Rebin(detLevelRebin.size()-1, Form("hZch_R0%i_%s_ptBin%i_MC",radius,triggers.at(trigger).Data(),binPt), detLevelRebin.data());
                hZch_MC->Scale(1.,"width");
                hZch->Divide(hZch,hZch_MC);
                hZch->GetYaxis()->SetRangeUser(0,12);
                hZch->GetXaxis()->SetRangeUser(0,1);
                SetStyleHistoTH1ForGraphs(hZch,"","#it{Z}_{ch}","Data/MC",0.03,0.04,0.03,0.04,1,0.7);
                hZch->SetMarkerStyle(styles[trigger]);
                hZch->SetMarkerColor(colors[trigger]);
                hZch->SetLineColor(colors[trigger]);
                if(trigger==0) hZch->Draw("p,e");
                else hZch->Draw("p,e,same");
                legend->AddEntry(hZch,triggers.at(trigger).Data(),"p");
            }
            legend->Draw();
            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.12,0.88, 0.03, false, false, false);
            drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.12,0.84, 0.03, false, false, false);
            drawLatexAdd(Form("#it{p}_{T}^{jet} = %i - %i GeV/#it{c}", binsPt[binPt], binsPt[binPt+1]),0.12,0.8, 0.03, false, false, false);
            canvas->SaveAs(Form("%s/TriggerBias/Zch/hZch_ptBin%i_R0%i.%s", outputdir.Data(),binPt,radius,fileType.Data()));

            legend->Clear();

            drawnum = 0;
            for(int trigger = 0; trigger < triggers.size(); trigger++){
                if(triggers.at(trigger) == "INT7" && binsPt[binPt] >= 60) draw = false;
                else if(triggers.at(trigger) == "EMC7" && binsPt[binPt] >= 100) draw = false;
                else draw = true;
                TH1D *tempZne = vecZne[radius-minradius].at(trigger)->ProjectionY(Form("hZne_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                TH1D *tempZne_MC = vecZne_MC[radius-minradius].at(trigger)->ProjectionY(Form("hZne_MC_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                TH1D *hZne = (TH1D*)tempZne->Rebin(detLevelRebin.size()-1, Form("hZne_R0%i_%s_ptBin%i",radius,triggers.at(trigger).Data(),binPt), detLevelRebin.data());
                hZne->Scale(1.,"width");
                TH1D *hZne_MC = (TH1D*)tempZne_MC->Rebin(detLevelRebin.size()-1, Form("hZne_R0%i_%s_ptBin%i_MC",radius,triggers.at(trigger).Data(),binPt), detLevelRebin.data());
                hZne_MC->Scale(1.,"width");
                hZne->Divide(hZne,hZne_MC);
                hZne->GetYaxis()->SetRangeUser(0,12);
                hZne->GetXaxis()->SetRangeUser(0,1);
                SetStyleHistoTH1ForGraphs(hZne,"","#it{Z}_{ne}","Data/MC",0.03,0.04,0.03,0.04,1,0.7);
                hZne->SetMarkerStyle(styles[trigger]);
                hZne->SetMarkerColor(colors[trigger]);
                hZne->SetLineColor(colors[trigger]);
                if(trigger==0) hZne->Draw("p,e");
                else hZne->Draw("p,e,same");
                legend->AddEntry(hZne,triggers.at(trigger).Data(),"p");
            }
            legend->Draw();
            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.12,0.88, 0.03, false, false, false);
            drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.12,0.84, 0.03, false, false, false);
            drawLatexAdd(Form("#it{p}_{T}^{jet} = %i - %i GeV/#it{c}", binsPt[binPt], binsPt[binPt+1]),0.12,0.8, 0.03, false, false, false);
            canvas->SaveAs(Form("%s/TriggerBias/Zne/hZne_ptBin%i_R0%i.%s", outputdir.Data(),binPt,radius,fileType.Data()));

            legend->Clear();
        }
    }

}
