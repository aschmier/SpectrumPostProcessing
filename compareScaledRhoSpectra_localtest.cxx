#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TH2D.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"
#include "fstream"

void compareScaledRhoSpectra_localtest(TString nfile, TString output, TString filetype, TString system){
    double textSize = 0.04;

    vector<double> binning = getJetPtBinningNonLinSmear8TeV();

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    int minradius = 2;
    int maxradius = 2;
    //if(system=="pp") maxradius = 5;
    //else if(system=="pPb") maxradius = 4;
    //else{
    //    cout << "Invalid system" << endl;
    //    return;
    //}

    vector<TString> triggers;
    if(system=="pp") triggers = {"INT7", "EMC7", "EJE"};
    else if(system=="pPb") triggers = {"INT7", "EJ2", "EJ1"};
    else{
        cout << "Invalid system" << endl;
        return;
    }

    TFile *f = TFile::Open(nfile);

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int t = 0; t < triggers.size(); t++){
            TDirectory *dDefault = (TDirectory*)f->Get(Form("JetSpectrum_FullJets_R0%i_%s_DefaultScaleVal", radius, triggers.at(t).Data()));
            TDirectory *dHigh    = (TDirectory*)f->Get(Form("JetSpectrum_FullJets_R0%i_%s_HighScaleVal", radius, triggers.at(t).Data()));
            TDirectory *dLow     = (TDirectory*)f->Get(Form("JetSpectrum_FullJets_R0%i_%s_LowScaleVal", radius, triggers.at(t).Data()));
            TDirectory *dExtreme = (TDirectory*)f->Get(Form("JetSpectrum_FullJets_R0%i_%s_ExtremeScaleVal", radius, triggers.at(t).Data()));
            TDirectory *dUnit    = (TDirectory*)f->Get(Form("JetSpectrum_FullJets_R0%i_%s_UnitScaleVal", radius, triggers.at(t).Data()));
            TDirectory *dNoScale = (TDirectory*)f->Get(Form("JetSpectrum_FullJets_R0%i_%s_NoScaling", radius, triggers.at(t).Data()));

            TList *lDefault = (TList*)dDefault->Get(Form("JetSpectrum_FullJets_R0%i_%s_DefaultScaleVal", radius, triggers.at(t).Data()));
            TList *lHigh    = (TList*)dHigh->Get(Form("JetSpectrum_FullJets_R0%i_%s_HighScaleVal", radius, triggers.at(t).Data()));
            TList *lLow     = (TList*)dLow->Get(Form("JetSpectrum_FullJets_R0%i_%s_LowScaleVal", radius, triggers.at(t).Data()));
            TList *lExtreme = (TList*)dExtreme->Get(Form("JetSpectrum_FullJets_R0%i_%s_ExtremeScaleVal", radius, triggers.at(t).Data()));
            TList *lUnit    = (TList*)dUnit->Get(Form("JetSpectrum_FullJets_R0%i_%s_UnitScaleVal", radius, triggers.at(t).Data()));
            TList *lNoScale = (TList*)dNoScale->Get(Form("JetSpectrum_FullJets_R0%i_%s_NoScaling", radius, triggers.at(t).Data()));

            TH2D *spectrumDefault2D = (TH2D*)lDefault->FindObject("hJetSpectrum");
            TH1D *spectrumDefault1D = (TH1D*)spectrumDefault2D->ProjectionY(Form("hJetSpectrum1D_R0%i_%s_Default", radius, triggers.at(t).Data()), 1, 1);
            TH1D *spectrumDefaultRebinned = (TH1D*)spectrumDefault1D->Rebin(binning.size()-1, Form("spectrumrebin_R0%i_%s_Default", radius, triggers.at(t).Data()), binning.data());
            spectrumDefaultRebinned->Scale(1.,"width");

            TH2D *spectrumHigh2D = (TH2D*)lHigh->FindObject("hJetSpectrum");
            TH1D *spectrumHigh1D = (TH1D*)spectrumHigh2D->ProjectionY(Form("hJetSpectrum1D_R0%i_%s_High", radius, triggers.at(t).Data()), 1, 1);
            TH1D *spectrumHighRebinned = (TH1D*)spectrumHigh1D->Rebin(binning.size()-1, Form("spectrumrebin_R0%i_%s_High", radius, triggers.at(t).Data()), binning.data());
            spectrumHighRebinned->Scale(1.,"width");

            TH2D *spectrumLow2D = (TH2D*)lLow->FindObject("hJetSpectrum");
            TH1D *spectrumLow1D = (TH1D*)spectrumLow2D->ProjectionY(Form("hJetSpectrum1D_R0%i_%s_Low", radius, triggers.at(t).Data()), 1, 1);
            TH1D *spectrumLowRebinned = (TH1D*)spectrumLow1D->Rebin(binning.size()-1, Form("spectrumrebin_R0%i_%s_Low", radius, triggers.at(t).Data()), binning.data());
            spectrumLowRebinned->Scale(1.,"width");

            TH2D *spectrumExtreme2D = (TH2D*)lExtreme->FindObject("hJetSpectrum");
            TH1D *spectrumExtreme1D = (TH1D*)spectrumExtreme2D->ProjectionY(Form("hJetSpectrum1D_R0%i_%s_Extreme", radius, triggers.at(t).Data()), 1, 1);
            TH1D *spectrumExtremeRebinned = (TH1D*)spectrumExtreme1D->Rebin(binning.size()-1, Form("spectrumrebin_R0%i_%s_Extreme", radius, triggers.at(t).Data()), binning.data());
            spectrumExtremeRebinned->Scale(1.,"width");

            TH2D *spectrumUnit2D = (TH2D*)lUnit->FindObject("hJetSpectrum");
            TH1D *spectrumUnit1D = (TH1D*)spectrumUnit2D->ProjectionY(Form("hJetSpectrum1D_R0%i_%s_Unit", radius, triggers.at(t).Data()), 1, 1);
            TH1D *spectrumUnitRebinned = (TH1D*)spectrumUnit1D->Rebin(binning.size()-1, Form("spectrumrebin_R0%i_%s_Unit", radius, triggers.at(t).Data()), binning.data());
            spectrumUnitRebinned->Scale(1.,"width");

            TH2D *spectrumNoScale2D = (TH2D*)lNoScale->FindObject("hJetSpectrum");
            TH1D *spectrumNoScale1D = (TH1D*)spectrumNoScale2D->ProjectionY(Form("hJetSpectrum1D_R0%i_%s_NoScale", radius, triggers.at(t).Data()), 1, 1);
            TH1D *spectrumNoScaleRebinned = (TH1D*)spectrumNoScale1D->Rebin(binning.size()-1, Form("spectrumrebin_R0%i_%s_NoScale", radius, triggers.at(t).Data()), binning.data());
            spectrumNoScaleRebinned->Scale(1.,"width");

            TH1D *rDefaultNoScale = (TH1D*)spectrumDefaultRebinned->Clone(Form("rDefaultNoScale_R0%i_%s", radius, triggers.at(t).Data()));
            rDefaultNoScale->Divide(spectrumNoScaleRebinned);
            rDefaultNoScale->SetMarkerStyle(styles[0]);
            rDefaultNoScale->SetMarkerColor(colors[0]);
            rDefaultNoScale->SetLineColor(colors[0]);

            TH1D *rHighNoScale = (TH1D*)spectrumHighRebinned->Clone(Form("rHighNoScale_R0%i_%s", radius, triggers.at(t).Data()));
            rHighNoScale->Divide(spectrumNoScaleRebinned);
            rHighNoScale->SetMarkerStyle(styles[1]);
            rHighNoScale->SetMarkerColor(colors[1]);
            rHighNoScale->SetLineColor(colors[1]);

            TH1D *rLowNoScale = (TH1D*)spectrumLowRebinned->Clone(Form("rLowNoScale_R0%i_%s", radius, triggers.at(t).Data()));
            rLowNoScale->Divide(spectrumNoScaleRebinned);
            rLowNoScale->SetMarkerStyle(styles[2]);
            rLowNoScale->SetMarkerColor(colors[2]);
            rLowNoScale->SetLineColor(colors[2]);

            TH1D *rExtremeNoScale = (TH1D*)spectrumExtremeRebinned->Clone(Form("rExtremeNoScale_R0%i_%s", radius, triggers.at(t).Data()));
            rExtremeNoScale->Divide(spectrumNoScaleRebinned);
            rExtremeNoScale->SetMarkerStyle(styles[3]);
            rExtremeNoScale->SetMarkerColor(colors[3]);
            rExtremeNoScale->SetLineColor(colors[3]);

            TH1D *rUnitNoScale = (TH1D*)spectrumUnitRebinned->Clone(Form("rUnitNoScale_R0%i_%s", radius, triggers.at(t).Data()));
            rUnitNoScale->Divide(spectrumNoScaleRebinned);
            rUnitNoScale->SetMarkerStyle(styles[4]);
            rUnitNoScale->SetMarkerColor(colors[4]);
            rUnitNoScale->SetLineColor(colors[4]);

            TCanvas *c   = new TCanvas(Form("c_%i_%s", radius, triggers.at(t).Data()), "", 1200, 800);
            DrawPaperCanvasSettings(c,0.08,0.025,0.025,0.1);
            gStyle->SetOptStat(0);
            //c->SetLogx(0);

            TH1D *h = new TH1D(Form("h_%i_%s", radius, triggers.at(t).Data()),"",260,0,260);
            SetStyleHistoTH1ForGraphs(h,"","p_{T} (GeV/c)","#rho^{scaled}/#rho^{unscaled}",0.03,0.04,0.03,0.04,1,0.8);
            h->GetYaxis()->SetRangeUser(0.99,1.01);
            h->Draw("axis");

            rDefaultNoScale->Draw("p,e,same");
            rHighNoScale->Draw("p,e,same");
            rLowNoScale->Draw("p,e,same");
            rExtremeNoScale->Draw("p,e,same");
            rUnitNoScale->Draw("p,e,same");
            
            TLegend *legend =  GetAndSetLegend2(0.6,(0.93-(5)*textSize),0.8,0.93,textSize,1);
            legend->AddEntry(rDefaultNoScale, "#rho^{ch} x 1.305 (Default)", "p");
            legend->AddEntry(rHighNoScale, "#rho^{ch} x 1.395 (High)", "p");
            legend->AddEntry(rLowNoScale, "#rho^{ch} x 1.275 (Low)", "p");
            legend->AddEntry(rExtremeNoScale, "#rho^{ch} x 5.0 (Extreme)", "p");
            legend->AddEntry(rUnitNoScale, "#rho^{ch} x 1.0 (Unit)", "p");
            
            legend->Draw("same");

            DrawGammaLines(0.,260.,1.,1.,5.,16,9);

            if(triggers.at(t) == "INT7"){
                DrawGammaLines(20.,20.,0.71,1.29,5.,16,9);
                DrawGammaLines(30.,30.,0.71,1.29,5.,16,9);
            }else if(triggers.at(t) == "EMC7" || triggers.at(t) == "EJ2"){
                DrawGammaLines(30.,30.,0.71,1.29,5.,16,9);
                DrawGammaLines(50.,50.,0.71,1.29,5.,16,9);
            }else if(triggers.at(t) == "EJE" || triggers.at(t) == "EJ1"){ 
                DrawGammaLines(50.,50.,0.71,1.29,5.,16,9);
                DrawGammaLines(240.,240.,0.71,1.29,5.,16,9);
            }

            drawLatexAdd("Ratio of Raw Jet Spectra",0.63,0.29, textSize,kFALSE, kFALSE, false);
            drawLatexAdd(Form("Trigger = %s, #it{R} = 0.%i", triggers.at(t).Data(), radius),0.63,0.24, textSize,kFALSE, kFALSE, false);
            if(system == "pp") drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.63,0.19, textSize,kFALSE, kFALSE, false);
            if(system == "pPb") drawLatexAdd("p-Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.63,0.19, textSize,kFALSE, kFALSE, false);
            drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.63,0.14, textSize,kFALSE, kFALSE, false);

            c->SaveAs(Form("%s/scaledChRhoComp_R0%i_%s.%s", output.Data(), radius, triggers.at(t).Data(), filetype.Data()));
        }
    }
}