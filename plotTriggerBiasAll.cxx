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

void plotTriggerBiasAll(TString mb_file, TString emc7_file, TString eje_file, TString outputdir, TString fileType, int minradius = 2, int maxradius = 6)
{
    Double_t textSize     = 0.03;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    vector<TString> files;
    vector<TString> triggers{"INT7","EMC7","EJE"};
    vector<TH2D*> vecNEF[maxradius-minradius+1];
    vector<TH2D*> vecZch[maxradius-minradius+1];
    vector<TH2D*> vecZne[maxradius-minradius+1];

    vector<double> detLevelRebin = getRebin();

    const Int_t nPtBins     = 7;
    Int_t binsPt[8]        = {6, 10, 20, 30, 60, 100, 200, 350};
    Double_t binsPtdoub[8]        = {6., 10., 20., 30., 60., 100., 200., 350.};

    files.push_back(mb_file);
    files.push_back(emc7_file);
    files.push_back(eje_file);


    for(int radius = minradius; radius <= maxradius; radius++){
        for(int trigger = 0; trigger < triggers.size(); trigger++){

            TFile *f = TFile::Open(files.at(trigger).Data());
            if(!f || f->IsZombie()){
                cout << "File not found!" << endl;
                return;
            }

            TString dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers.at(trigger).Data());
            TDirectory *dir = (TDirectory*)f->Get(dirname.Data());
            TList *list = (TList*)dir->Get(dirname.Data());
            TH1D *events = (TH1D*)list->FindObject("hClusterCounter");
            TH2D *NEF2d = (TH2D*)list->FindObject("hQANEFPt");
            TH2D *Zch2d = (TH2D*)list->FindObject("hQAZchPt");
            TH2D *Zne2d = (TH2D*)list->FindObject("hQAZnePt");
            vecNEF[radius-minradius].push_back(NEF2d);
            vecZch[radius-minradius].push_back(Zch2d);
            vecZne[radius-minradius].push_back(Zne2d);
        }
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.08;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend =  GetAndSetLegend2(0.6,0.78,0.9,0.78+((3)*textSize*1.5),textSize);

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int binPt = 0; binPt < nPtBins; binPt++){
            for(int trigger = 0; trigger < triggers.size(); trigger++){
                TH1D *hNEF = vecNEF[radius-minradius].at(trigger)->ProjectionY(Form("hNEF_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                hNEF->Rebin(5);
                hNEF->Scale(1.,"width");
                hNEF->Scale(1./hNEF->Integral());
                hNEF->GetYaxis()->SetRangeUser(0,0.1);
                hNEF->GetXaxis()->SetRangeUser(0,1);
                SetStyleHistoTH1ForGraphs(hNEF,"","#it{NEF}","1/#it{N}_{jet} d#it{N}_{jet}/d#it{NEF}",0.03,0.04,0.03,0.04,1,0.9);
                hNEF->SetMarkerStyle(styles[trigger]);
                hNEF->SetMarkerColor(colors[trigger]);
                hNEF->SetLineColor(colors[trigger]);
                if(trigger==0) hNEF->Draw("p,e");
                else hNEF->Draw("p,e,same");
                legend->AddEntry(hNEF,triggers.at(trigger).Data(),"p");
            }

            legend->Draw();
            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.94,0.88, 0.03, false, false, true);
            drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.94,0.84, 0.03, false, false, true);
            drawLatexAdd(Form("#it{p}_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.94,0.8, 0.03, false, false, true);
            canvas->SaveAs(Form("%s/NEF/All/hNEF_%i-%iGeV_R0%i.%s", outputdir.Data(),binsPt[binPt],binsPt[binPt+1],radius,fileType.Data()));

            legend->Clear();

            for(int trigger = 0; trigger < triggers.size(); trigger++){
                TH1D *hZch = vecZch[radius-minradius].at(trigger)->ProjectionY(Form("hZch_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                hZch->Rebin(5);
                hZch->Scale(1.,"width");
                hZch->Scale(1./hZch->Integral());
                hZch->GetYaxis()->SetRangeUser(0,0.1);
                hZch->GetXaxis()->SetRangeUser(0,1);
                SetStyleHistoTH1ForGraphs(hZch,"","#it{Z}_{ch}","1/#it{N}_{jet} d#it{N}_{jet}/d#it{Z}_{ch}",0.03,0.04,0.03,0.04,1,0.7);
                hZch->SetMarkerStyle(styles[trigger]);
                hZch->SetMarkerColor(colors[trigger]);
                hZch->SetLineColor(colors[trigger]);
                if(trigger==0) hZch->Draw("p,e");
                else hZch->Draw("p,e,same");
                legend->AddEntry(hZch,triggers.at(trigger).Data(),"p");
            }
            legend->Draw();
            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.94,0.88, 0.03, false, false, true);
            drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.94,0.84, 0.03, false, false, true);
            drawLatexAdd(Form("#it{p}_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.94,0.8, 0.03, false, false, true);
            canvas->SaveAs(Form("%s/Zch/All/hZch_%i-%iGeV_R0%i.%s", outputdir.Data(),binsPt[binPt],binsPt[binPt+1],radius,fileType.Data()));

            legend->Clear();

            for(int trigger = 0; trigger < triggers.size(); trigger++){
                TH1D *hZne = vecZne[radius-minradius].at(trigger)->ProjectionY(Form("hZne_%s_R0%i_%i-%iGeV", triggers.at(trigger).Data(), radius, binsPt[binPt], binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
                hZne->Rebin(5);
                hZne->Scale(1.,"width");
                hZne->Scale(1./hZne->Integral());
                hZne->GetYaxis()->SetRangeUser(0,0.1);
                hZne->GetXaxis()->SetRangeUser(0,1);
                SetStyleHistoTH1ForGraphs(hZne,"","#it{Z}_{ne}","1/#it{N}_{jet} d#it{N}_{jet}/d#it{Z}_{ne}",0.03,0.04,0.03,0.04,1,0.7);
                hZne->SetMarkerStyle(styles[trigger]);
                hZne->SetMarkerColor(colors[trigger]);
                hZne->SetLineColor(colors[trigger]);
                if(trigger==0) hZne->Draw("p,e");
                else hZne->Draw("p,e,same");
                legend->AddEntry(hZne,triggers.at(trigger).Data(),"p");
            }
            legend->Draw();
            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.94,0.88, 0.03, false, false, true);
            drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.94,0.84, 0.03, false, false, true);
            drawLatexAdd(Form("#it{p}_{T}^{jet} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.94,0.8, 0.03, false, false, true);
            canvas->SaveAs(Form("%s/Zne/All/hZne_%i-%iGeV_R0%i.%s", outputdir.Data(),binsPt[binPt],binsPt[binPt+1],radius,fileType.Data()));

            legend->Clear();
        }
    }

}
