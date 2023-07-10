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

void extractEnergyScaleSlices(TString file, TString outputdir, TString fileType, int minradius = 2, int maxradius = 6)
{
    Double_t textSize     = 0.03;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14] = {1,2,3,4,6,7,8,9,28,30,40,41,46,49};

    vector<TH1D*> vecSlices[maxradius-minradius+1];

    gSystem->Exec("mkdir -p "+outputdir+"/EnergyScale/Slices/AllRadii");

    const Int_t nPtBins     = 16;
    Int_t binsPt[17]        = {10,12,14,16,20,25,30,35,40,50,60,70,80,100,120,160,200};

    TFile *f = TFile::Open(file);
    if(!f || f->IsZombie()){
        cout << "MC file not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        TString dirname = Form("EnergyScaleResults_%sJet_R0%i_INT7_nodownscalecorr", jetType.Data(), radius);
        TString listname = Form("EnergyScaleHists_%sJet_R0%i_INT7_nodownscalecorr", jetType.Data(), radius);
        TDirectory *dir = (TDirectory*)f->Get(dirname.Data());
        if(!dir){
            dirname     = Form("EnergyScaleResults_%sJet_R0%i_INT7_default", jetType.Data(), radius);
            listname = Form("EnergyScaleHists_%sJet_R0%i_INT7_default", jetType.Data(), radius);
            dir = (TDirectory*)f->Get(dirname.Data());
        }
        TList *list = (TList*)dir->Get(listname.Data());
        TH2D *EnergyScale2D = (TH2D*)list->FindObject("hJetEnergyScale");
        for(int binPt = 0; binPt < nPtBins; binPt++){
            TH1D *EnergyScale1D = (TH1D*)EnergyScale2D->ProjectionY(Form("EnergyScale_R0%i_%i-%iGeV",radius,binsPt[binPt],binsPt[binPt+1]),binsPt[binPt],binsPt[binPt+1]);
            vecSlices[radius-minradius].push_back(EnergyScale1D);
        }
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.075;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLine * l1 = new TLine (0,0,0,4.3);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);

    TLegend *legend =  GetAndSetLegend2(0.11,0.68,0.31,0.68+(maxradius-minradius+1)*textSize,textSize,1);

    for(int binPt = 0; binPt < nPtBins; binPt++){
        legend->Clear();
        for(int radius = minradius; radius <= maxradius; radius++){
            TH1D *tempSlice = vecSlices[radius-minradius].at(binPt);
            tempSlice->Rebin(4);
            tempSlice->Scale(1/tempSlice->Integral());
            tempSlice->GetXaxis()->SetRangeUser(-1.,0.4);
            tempSlice->GetYaxis()->SetRangeUser(0,0.3);
            SetStyleHistoTH1ForGraphs(tempSlice,"","<(#it{p}_{T}^{det} - #it{p}_{T}^{part})/#it{p}_{T}^{part}","Prob/Bin(0.04)",0.03,0.04,0.03,0.04,1,0.85);
            tempSlice->SetMarkerColor(colors[radius-minradius]);
            tempSlice->SetLineColor(colors[radius-minradius]);
            tempSlice->SetMarkerStyle(styles[radius-minradius]);
            Double_t mean = tempSlice->GetMean();
            if(radius == minradius) tempSlice->Draw("p,e");
            else tempSlice->Draw("p,e,same");
            DrawGammaLines(mean,mean,0,0.3,1.,colors[radius-minradius],9);
            legend->AddEntry(tempSlice, Form("#it{R} = 0.%i - Mean = %f",radius,mean), "p");
        }
        legend->Draw("same");
        l1->Draw("same");
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.12,0.88, 0.03, false, false, false);
        drawLatexAdd(Form("#it{p}_{T}^{part} = %i - %i GeV/#it{c}", binsPt[binPt], binsPt[binPt+1]),0.12,0.84, 0.03,false, false, false);
        canvas->SaveAs(Form("%s/EnergyScale/Slices/AllRadii/EnergyScale_AllRadii_%i-%iGeV.%s", outputdir.Data(),binsPt[binPt],binsPt[binPt+1],fileType.Data()));
    }
}
