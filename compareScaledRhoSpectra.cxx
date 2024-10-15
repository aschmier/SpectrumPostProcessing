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

void compareScaledRhoSpectra(TString nChScaled, TString nChUnscaled, TString nFull, TString output, TString filetype, TString system){
    double textSize = 0.04;

    vector<double> binning = getJetPtBinningNonLinSmear8TeV();

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    double compymin = 0.71;
    double compymax = 1.29;
    double sysymin = 0.94;
    double sysymax = 1.06;

    int minradius = 2;
    int maxradius;
    if(system=="pp") maxradius = 5;
    else if(system=="pPb") maxradius = 4;
    else{
        cout << "Invalid system" << endl;
        return;
    }

    vector<TString> triggers;
    if(system=="pp") triggers = {"INT7", "EMC7", "EJE"};
    else if(system=="pPb") triggers = {"INT7", "EJ2", "EJ1"};
    else{
        cout << "Invalid system" << endl;
        return;
    }

    TFile *fChSc = TFile::Open(nChScaled);
    TFile *fChUn = TFile::Open(nChUnscaled);
    TFile *fFull = TFile::Open(nFull);

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int t = 0; t < triggers.size(); t++){
            TDirectory *dDefault = (TDirectory*)fChSc->Get(Form("JetSpectrum_FullJets_R0%i_%s_DefaultScaleVal", radius, triggers.at(t).Data()));
            TDirectory *dHigh    = (TDirectory*)fChSc->Get(Form("JetSpectrum_FullJets_R0%i_%s_HighScaleVal", radius, triggers.at(t).Data()));
            TDirectory *dLow     = (TDirectory*)fChSc->Get(Form("JetSpectrum_FullJets_R0%i_%s_LowScaleVal", radius, triggers.at(t).Data()));
            TDirectory *dChUn    = (TDirectory*)fChUn->Get(Form("JetSpectrum_FullJets_R0%i_%s_default", radius, triggers.at(t).Data()));
            TDirectory *dFull    = (TDirectory*)fFull->Get(Form("JetSpectrum_FullJets_R0%i_%s_default", radius, triggers.at(t).Data()));

            TList *lDefault = (TList*)dDefault->Get(Form("JetSpectrum_FullJets_R0%i_%s_DefaultScaleVal", radius, triggers.at(t).Data()));
            TList *lHigh    = (TList*)dHigh->Get(Form("JetSpectrum_FullJets_R0%i_%s_HighScaleVal", radius, triggers.at(t).Data()));
            TList *lLow     = (TList*)dLow->Get(Form("JetSpectrum_FullJets_R0%i_%s_LowScaleVal", radius, triggers.at(t).Data()));
            TList *lChUn    = (TList*)dChUn->Get(Form("JetSpectrum_FullJets_R0%i_%s_default", radius, triggers.at(t).Data()));
            TList *lFull    = (TList*)dFull->Get(Form("JetSpectrum_FullJets_R0%i_%s_default", radius, triggers.at(t).Data()));

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

            TH2D *spectrumChUn2D = (TH2D*)lChUn->FindObject("hJetSpectrum");
            TH1D *spectrumChUn1D = (TH1D*)spectrumChUn2D->ProjectionY(Form("hJetSpectrum1D_R0%i_%s_ChUn", radius, triggers.at(t).Data()), 1, 1);
            TH1D *spectrumChUnRebinned = (TH1D*)spectrumChUn1D->Rebin(binning.size()-1, Form("spectrumrebin_R0%i_%s_ChUn", radius, triggers.at(t).Data()), binning.data());
            spectrumChUnRebinned->Scale(1.,"width");

            TH2D *spectrumFull2D = (TH2D*)lFull->FindObject("hJetSpectrum");
            TH1D *spectrumFull1D = (TH1D*)spectrumFull2D->ProjectionY(Form("hJetSpectrum1D_R0%i_%s_Full", radius, triggers.at(t).Data()), 1, 1);
            TH1D *spectrumFullRebinned = (TH1D*)spectrumFull1D->Rebin(binning.size()-1, Form("spectrumrebin_R0%i_%s_Full", radius, triggers.at(t).Data()), binning.data());
            spectrumFullRebinned->Scale(1.,"width");

            TH1D *rDefaultFull = (TH1D*)spectrumDefaultRebinned->Clone(Form("rDefaultFull_R0%i_%s", radius, triggers.at(t).Data()));
            rDefaultFull->Divide(spectrumFullRebinned);
            rDefaultFull->SetMarkerStyle(styles[0]);
            rDefaultFull->SetMarkerColor(colors[0]);
            rDefaultFull->SetLineColor(colors[0]);

            TH1D *rHighFull = (TH1D*)spectrumHighRebinned->Clone(Form("rHighFull_R0%i_%s", radius, triggers.at(t).Data()));
            rHighFull->Divide(spectrumFullRebinned);
            rHighFull->SetMarkerStyle(styles[1]);
            rHighFull->SetMarkerColor(colors[1]);
            rHighFull->SetLineColor(colors[1]);

            TH1D *rLowFull = (TH1D*)spectrumLowRebinned->Clone(Form("rLowFull_R0%i_%s", radius, triggers.at(t).Data()));
            rLowFull->Divide(spectrumFullRebinned);
            rLowFull->SetMarkerStyle(styles[2]);
            rLowFull->SetMarkerColor(colors[2]);
            rLowFull->SetLineColor(colors[2]);

            TH1D *rChUnFull = (TH1D*)spectrumChUnRebinned->Clone(Form("rChUnFull_R0%i_%s", radius, triggers.at(t).Data()));
            rChUnFull->Divide(spectrumFullRebinned);
            rChUnFull->SetMarkerStyle(styles[3]);
            rChUnFull->SetMarkerColor(colors[3]);
            rChUnFull->SetLineColor(colors[3]);

            TCanvas *c   = new TCanvas(Form("c_%i_%s", radius, triggers.at(t).Data()), "", 1200, 800);
            DrawPaperCanvasSettings(c,0.08,0.025,0.025,0.1);
            gStyle->SetOptStat(0);
            //c->SetLogx(0);

            TH1D *h = new TH1D(Form("h_%i_%s", radius, triggers.at(t).Data()),"",260,0,260);
            SetStyleHistoTH1ForGraphs(h,"","p_{T} (GeV/c)","d#sigma(#rho^{ch,scale}) / d#sigma(#rho^{full})",0.03,0.04,0.03,0.04,1,0.8);
            h->GetYaxis()->SetRangeUser(compymin,compymax);
            h->Draw("axis");

            rDefaultFull->Draw("p,e,same");
            rHighFull->Draw("p,e,same");
            rLowFull->Draw("p,e,same");
            rChUnFull->Draw("p,e,same");

            TLegend *legend =  GetAndSetLegend2(0.6,(0.93-(5)*textSize),0.8,0.93,textSize,1);
            legend->AddEntry(rDefaultFull, "#rho^{ch} x 1.305 (Default)", "p");
            legend->AddEntry(rHighFull, "#rho^{ch} x 1.395 (High Var)", "p");
            legend->AddEntry(rLowFull, "#rho^{ch} x 1.275 (Low Var)", "p");
            legend->AddEntry(rChUnFull, "#rho^{ch} x 1 (No Scaling)", "p");
            legend->Draw("same");

            DrawGammaLines(0.,260.,1.,1.,5.,16,9);

            if(triggers.at(t) == "INT7"){
                DrawGammaLines(20.,20.,compymin,compymax,5.,16,9);
                DrawGammaLines(30.,30.,compymin,compymax,5.,16,9);
            }else if(triggers.at(t) == "EMC7" || triggers.at(t) == "EJ2"){
                DrawGammaLines(30.,30.,compymin,compymax,5.,16,9);
                DrawGammaLines(50.,50.,compymin,compymax,5.,16,9);
            }else if(triggers.at(t) == "EJE" || triggers.at(t) == "EJ1"){ 
                DrawGammaLines(50.,50.,compymin,compymax,5.,16,9);
                DrawGammaLines(240.,240.,compymin,compymax,5.,16,9);
            }

            drawLatexAdd(Form("Trigger = %s, #it{R} = 0.%i", triggers.at(t).Data(), radius),0.63,0.24, textSize,kFALSE, kFALSE, false);
            if(system == "pp") drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.63,0.19, textSize,kFALSE, kFALSE, false);
            if(system == "pPb") drawLatexAdd("p-Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.63,0.19, textSize,kFALSE, kFALSE, false);
            drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.63,0.14, textSize,kFALSE, kFALSE, false);

            c->SaveAs(Form("%s/scaledChRhoComp_R0%i_%s.%s", output.Data(), radius, triggers.at(t).Data(), filetype.Data()));

            TH1D *rHighDefault = (TH1D*)spectrumHighRebinned->Clone(Form("rHighDefault_R0%i_%s", radius, triggers.at(t).Data()));
            rHighDefault->Divide(spectrumDefaultRebinned);
            rHighDefault->SetMarkerStyle(styles[1]);
            rHighDefault->SetMarkerColor(colors[1]);
            rHighDefault->SetLineColor(colors[1]);

            TH1D *rLowDefault = (TH1D*)spectrumLowRebinned->Clone(Form("rLowDefault_R0%i_%s", radius, triggers.at(t).Data()));
            rLowDefault->Divide(spectrumDefaultRebinned);
            rLowDefault->SetMarkerStyle(styles[2]);
            rLowDefault->SetMarkerColor(colors[2]);
            rLowDefault->SetLineColor(colors[2]);

            SetStyleHistoTH1ForGraphs(h,"","p_{T} (GeV/c)","d#sigma(#rho^{ch,var}) / d#sigma(#rho^{ch,def})",0.03,0.04,0.03,0.04,1,0.8);
            h->GetYaxis()->SetRangeUser(sysymin,sysymax);
            h->Draw("axis");

            for(int b = 1; b < rHighDefault->GetNbinsX()+1; b++){
                rHighDefault->SetBinError(b,0);
            }
            for(int b = 1; b < rLowDefault->GetNbinsX()+1; b++){
                rLowDefault->SetBinError(b,0);
            }

            rHighDefault->SetMarkerSize(1.5);
            rLowDefault->SetMarkerSize(2);

            rHighDefault->Draw("p,same");
            rLowDefault->Draw("p,same");

            legend =  GetAndSetLegend2(0.48,(0.93-(3)*textSize),0.68,0.93,textSize,1);
            legend->AddEntry(rHighDefault, "#rho^{ch} x 1.395 / #rho^{ch} x 1.305 (High Var)", "p");
            legend->AddEntry(rLowDefault, "#rho^{ch} x 1.275 / #rho^{ch} x 1.305 (Low Var)", "p");
            legend->Draw("same");

            DrawGammaLines(0.,260.,1.,1.,5.,16,9);

            if(triggers.at(t) == "INT7"){
                DrawGammaLines(20.,20.,sysymin,sysymax,5.,16,9);
                DrawGammaLines(30.,30.,sysymin,sysymax,5.,16,9);
            }else if(triggers.at(t) == "EMC7" || triggers.at(t) == "EJ2"){
                DrawGammaLines(30.,30.,sysymin,sysymax,5.,16,9);
                DrawGammaLines(50.,50.,sysymin,sysymax,5.,16,9);
            }else if(triggers.at(t) == "EJE" || triggers.at(t) == "EJ1"){
                DrawGammaLines(50.,50.,sysymin,sysymax,5.,16,9);
                DrawGammaLines(240.,240.,sysymin,sysymax,5.,16,9);
            }

            drawLatexAdd(Form("Trigger = %s, #it{R} = 0.%i", triggers.at(t).Data(), radius),0.63,0.24, textSize,kFALSE, kFALSE, false);
            if(system == "pp") drawLatexAdd("pp #sqrt{#it{s}} = 8 TeV",0.63,0.19, textSize,kFALSE, kFALSE, false);
            if(system == "pPb") drawLatexAdd("p-Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.63,0.19, textSize,kFALSE, kFALSE, false);
            drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.63,0.14, textSize,kFALSE, kFALSE, false);

            c->SaveAs(Form("%s/scaledChRhoComp_R0%i_%s_Systematics.%s", output.Data(), radius, triggers.at(t).Data(), filetype.Data()));
        }
    }
}