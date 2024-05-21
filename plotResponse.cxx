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

void plotResponse(TString file, TString output, TString fileType, TString system = "pp")
{
// Run systematics and plot rejection factor
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 5;

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};

    vector<TH2D*> vecResponse;

    TFile *f = TFile::Open(file);
    if(!f || f->IsZombie()){
        cout << "File not found!" << endl;
        return;
    }

    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.1,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogz();

    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *dRadius   = (TDirectory*)f->Get(Form("R0%i",radius));
        TDirectory *dResponse = (TDirectory*)dRadius->Get("response");
        TH2D *response  = (TH2D*)dResponse->Get(Form("Rawresponse_R0%i_fine_rebinned_standard",radius));
        response->Scale(1.,"width");

        response->GetXaxis()->SetRangeUser(0,240);
        response->GetYaxis()->SetRangeUser(0,320);
        response->GetZaxis()->SetRangeUser(2e-4,1e4);
        SetStyleHistoTH2ForGraphs(response,"#it{p}_{T}^{det} (GeV/#it{c})","#it{p}_{T}^{part}",textSize,0.04,textSize,0.04,1,1);
        response->Draw("colz");

        if(system=="pp") drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.86,0.24, textSize,kFALSE, kFALSE, true);
        if(system=="pPb") drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.86,0.2, textSize,kFALSE, kFALSE, true);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, R = 0.%i",radius),0.86,0.16, textSize,kFALSE, kFALSE, true);

        c->SaveAs(Form("%s/Response_R0%i.%s",output.Data(),radius,fileType.Data()));
    }
}
