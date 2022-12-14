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

void extractEnergyScaleSlices(TString file, TString outputdir, TString fileType)
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 6;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14] = {1,2,3,4,6,7,8,9,28,30,40,41,46,49};

    vector<TH1D*> vecSlices[maxradius-minradius+1];

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

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int binPt = 0; binPt < nPtBins; binPt++){
            TH1D *tempSlice = vecSlices[radius-minradius].at(binPt);
            tempSlice->Rebin(4);
            tempSlice->Scale(1/tempSlice->Integral());
            tempSlice->GetXaxis()->SetRangeUser(-1,0.4);
            tempSlice->GetYaxis()->SetRangeUser(0,0.3);
            SetStyleHistoTH1ForGraphs(tempSlice,"","<(#it{p}_{T}^{det} - #it{p}_{T}^{part})/#it{p}_{T}^{part}","Prob/Bin(0.04)",0.03,0.04,0.03,0.04,1,0.85);
            tempSlice->SetMarkerColor(colors[0]);
            tempSlice->SetLineColor(colors[0]);
            tempSlice->SetMarkerStyle(styles[0]);
            tempSlice->Draw("p,e");
            l1->Draw("same");
            drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.12,0.88, 0.03, false, false, false);
            drawLatexAdd(Form("%s Jets, #it{R}=0.%i", jetType.Data(), radius),0.12,0.84, 0.03, false, false, false);
            drawLatexAdd(Form("#it{p}_{T}^{part} = %i - %i GeV", binsPt[binPt], binsPt[binPt+1]),0.12,0.8, 0.03,false, false, false);
            canvas->SaveAs(Form("%s/EnergyScale/Slices/%s.%s", outputdir.Data(),tempSlice->GetName(),fileType.Data()));
        }
    }
}
