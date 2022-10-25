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

void plotTriggerEfficiencies(TString mc_file, TString outputdir, TString fileType)
{
    Double_t textSize     = 0.03;
    int minradius = 2;
    int maxradius = 6;
    TString jetType = "Full";

    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    vector<TString> triggers{"INT7","EMC7","EJE"};
    vector<TH2D*> vecSpectrumMC[maxradius-minradius+1];

    TFile *fmc = TFile::Open(mc_file);
    if(!fmc || fmc->IsZombie()){
        cout << "MC file not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int trigger = 0; trigger < triggers.size(); trigger++){
            TString dirname = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers.at(trigger).Data());
            TDirectory *dirMC = (TDirectory*)fmc->Get(dirname.Data());
            TList *listMC = (TList*)dirMC->Get(dirname.Data());
            TH1D *eventsMC = (TH1D*)listMC->FindObject("hClusterCounter");
            TH2D *spectrum2dMC = (TH2D*)listMC->FindObject("hJetSpectrum");
            spectrum2dMC->Scale(1/eventsMC->GetBinContent(1));
            vecSpectrumMC[radius-minradius].push_back(spectrum2dMC);        }
    }

    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    Double_t leftMargin         = 0.075;
    Double_t rightMargin        = 0.02;
    Double_t topMargin          = 0.04;
    Double_t bottomMargin       = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

    TLegend *legend     =  GetAndSetLegend2(0.47,0.15,0.95,0.15+((maxradius-minradius+1)*textSize*1.5)/2,textSize,2);

    for(int radius = minradius; radius <= maxradius; radius++){
        TH1D *tempSpectrumMB   = (TH1D*)vecSpectrumMC[radius-minradius].at(0)->ProjectionY(Form("hSpectrum_INT7_R0%i", radius),1,1);
        TH1D *tempSpectrumEMC7 = (TH1D*)vecSpectrumMC[radius-minradius].at(1)->ProjectionY(Form("hEfficiency_EMC7_R0%i", radius),1,1);
        SetStyleHistoTH1ForGraphs(tempSpectrumEMC7,"","p_{T}^{jet} [GeV/c]","EMC7 Trigger Efficiency (EMC7^{MC}/INT7^{MC})",0.03,0.04,0.03,0.04,1,0.7);
        tempSpectrumEMC7->Divide(tempSpectrumEMC7,tempSpectrumMB);
        legend->AddEntry(tempSpectrumEMC7, Form("#it{R}=0.%i",radius), "p");
        tempSpectrumEMC7->SetMarkerColor(colors[radius-minradius]);
        tempSpectrumEMC7->SetLineColor(colors[radius-minradius]);
        tempSpectrumEMC7->SetMarkerStyle(styles[radius-minradius]);
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        if(radius == minradius) tempSpectrumEMC7->Draw("p,e");
        else tempSpectrumEMC7->Draw("p,e,same");
    }
    legend->Draw();
    canvas->SaveAs(Form("%s/TriggerEfficiency/hEfficiency_EMC7.%s", outputdir.Data(),fileType.Data()));

    legend->Clear();

    for(int radius = minradius; radius <= maxradius; radius++){
        TH1D *tempSpectrumMB   = (TH1D*)vecSpectrumMC[radius-minradius].at(0)->ProjectionY(Form("hSpectrum_INT7_R0%i", radius),1,1);
        TH1D *tempSpectrumEJE  = (TH1D*)vecSpectrumMC[radius-minradius].at(2)->ProjectionY(Form("hEfficiency_EJE_R0%i", radius),1,1);
        SetStyleHistoTH1ForGraphs(tempSpectrumEJE,"","p_{T}^{jet} [GeV/c]","EJE Trigger Efficiency (EJE^{MC}/INT7^{MC})",0.03,0.04,0.03,0.04,1,0.7);
        tempSpectrumEJE->Divide(tempSpectrumEJE,tempSpectrumMB);
        legend->AddEntry(tempSpectrumEJE, Form("#it{R}=0.%i",radius), "p");
        tempSpectrumEJE->SetMarkerColor(colors[radius-minradius]);
        tempSpectrumEJE->SetLineColor(colors[radius-minradius]);
        tempSpectrumEJE->SetMarkerStyle(styles[radius-minradius]);
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        if(radius == minradius) tempSpectrumEJE->Draw("p,e");
        else tempSpectrumEJE->Draw("p,e,same");
    }
    legend->Draw();
    canvas->SaveAs(Form("%s/TriggerEfficiency/hEfficiency_EJE.%s", outputdir.Data(),fileType.Data()));
}
