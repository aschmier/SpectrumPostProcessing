#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

std::vector<double> getBins(TH1 *fHistogram){
    std::vector<double> result;
    for(int ib = 0; ib < fHistogram->GetXaxis()->GetNbins(); ib++) {
        auto entries  = fHistogram->GetBinContent(ib+1);
        if(TMath::Abs(entries) > DBL_EPSILON) {
            // filter 0
            result.emplace_back(entries);
        }
    }
    return result;
}

double getMaxTrials(TH1 *hist){
    auto bins = getBins(hist);
    return *std::max_element(bins.begin(), bins.end());
}

void plotRpADebug(TString ppINT7, TString ppEMC7, TString ppEJE, TString ppMC, TString pAINT7, TString pAEJ2, TString pAEJ1, TString pAMC, TString outputdir)
{
    double textSize  = 0.04;
    int minradius    = 2;
    int maxradius    = 2;
    TString jetType  = "Full";
    TString fileType = "png";

    int styles[11]   = {4,25,27,28,35,36,38,40,42,44,46};
    int colors[14]   = {1,2,209,4,6,7,8,9,28,30,40,41,46,49};
    vector<TH1D*> vecSpecData[maxradius-minradius+1];
    vector<TH1D*> vecSpecMC[maxradius-minradius+1];
    vector<TH1D*> vecClusData[maxradius-minradius+1];
    vector<TH1D*> vecClusMC[maxradius-minradius+1];
    vector<double> detlevelbin = getJetPtBinningNonLinSmear8TeV();
    double xsec_pp   = 55.8;
    double xsec_pPb  = 2095.;
    double nucfactor = 208.;
    vector<TString> fileNamesData{ppINT7, ppEMC7, ppEJE, pAINT7, pAEJ2, pAEJ1};
    vector<TString> triggers{"INT7","EMC7","EJE","INT7","EJ2","EJ1"};

    double RpA_scale = (xsec_pPb)/(xsec_pp*nucfactor);

    for(int radius = minradius; radius <= maxradius; radius++){
        for(int t = 0; t < triggers.size(); t++){
            TFile *fdata = TFile::Open(fileNamesData.at(t));
            if(!fdata || fdata->IsZombie()){
                cout << "File not found!" << endl;
                return;
            }

            cout << "File Name -----> " << fileNamesData.at(t) << endl;

            TString dirname     = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers.at(t).Data());

            TDirectory *dir = (TDirectory*)fdata->Get(dirname.Data());
            if(!dir){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers.at(t).Data());
                dir = (TDirectory*)fdata->Get(dirname.Data());
            }
            TList *list     = (TList*)dir->Get(dirname.Data());
            TH1D *events    = (TH1D*)list->FindObject("hClusterCounter");
            TH2D *spec2d    = (TH2D*)list->FindObject("hJetSpectrum");
            TH1D *clus1d    = (TH1D*)list->FindObject("hClusterEnergy1D");
            TH1D *spec1d    = (TH1D*)spec2d->ProjectionY(Form("dataspec1d_R0%i_%s",radius,triggers.at(t).Data()),1,1);
            TH1D *rspec     = (TH1D*)spec1d->Rebin(detlevelbin.size()-1, Form("rdataspec_%i_%s",radius,triggers.at(t).Data()), detlevelbin.data());
            TH1D *rclus     = (TH1D*)clus1d->Rebin(detlevelbin.size()-1, Form("rdataclus_%i_%s",radius,triggers.at(t).Data()), detlevelbin.data());
            rspec->Scale(1.,"width");
            rspec->Scale(1/events->GetBinContent(1));
            rclus->Scale(1.,"width");
            rclus->Scale(1/events->GetBinContent(1));
            vecSpecData[radius-minradius].push_back(rspec);
            vecClusData[radius-minradius].push_back(rclus);
        }

        for(int t = 0; t < triggers.size(); t++){
            TFile *fmc;
            if(t < 3) fmc = TFile::Open(ppMC);
            else fmc = TFile::Open(pAMC);
            if(!fmc || fmc->IsZombie()){
                cout << "File not found!" << endl;
                return;
            }

            TString dirname        = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers.at(t).Data());
            TString dirnameEScale  = Form("EnergyScaleResults_%sJet_R0%i_INT7_nodownscalecorr", jetType.Data(), radius);
            TString listnameEScale = Form("EnergyScaleHists_%sJet_R0%i_INT7_nodownscalecorr", jetType.Data(), radius);

            TDirectory *dir = (TDirectory*)fmc->Get(dirname.Data());
            if(!dir){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers.at(t).Data());
                dir = (TDirectory*)fmc->Get(dirname.Data());
            }
            TDirectory *dirEScale = (TDirectory*)fmc->Get(dirnameEScale.Data());
            if(!dirEScale){
                dirnameEScale     = Form("EnergyScaleResults_%sJet_R0%i_INT7_default", jetType.Data(), radius);
                dirEScale = (TDirectory*)fmc->Get(dirnameEScale.Data());
                listnameEScale = Form("EnergyScaleHists_%sJet_R0%i_INT7_default", jetType.Data(), radius);
            }
            TList *list     = (TList*)dir->Get(dirname.Data());
            TH1D *events    = (TH1D*)list->FindObject("hClusterCounter");
            TH2D *spec2d    = (TH2D*)list->FindObject("hJetSpectrum");
            TH1D *clus1d    = (TH1D*)list->FindObject("hClusterEnergy1D");
            TH1D *spec1d    = (TH1D*)spec2d->ProjectionY(Form("dataspec1d_R0%i_%s",radius,triggers.at(t).Data()),1,1);
            TH1D *rspec     = (TH1D*)spec1d->Rebin(detlevelbin.size()-1, Form("rmcspec_%i_%s",radius,triggers.at(t).Data()), detlevelbin.data());
            TH1D *rclus     = (TH1D*)spec1d->Rebin(detlevelbin.size()-1, Form("rmcclus_%i_%s",radius,triggers.at(t).Data()), detlevelbin.data());
            TList *listEScale = (TList*)dirEScale->Get(listnameEScale.Data());
            TH1F *histtrials = (TH1F*)listEScale->FindObject("fHistTrials");
            double trials = getMaxTrials(histtrials);
            if(t==0) cout << "pp:" << endl;
            if(t==3) cout << "p--Pb:" << endl;
            cout << "Trigger: " << triggers.at(t).Data() << endl;
            cout << "Trials: " << trials << endl;
            cout << "Events: " << events->GetBinContent(1) << endl;
            rspec->Scale(1.,"width");
            //rspec->Scale(1/trials);
            rclus->Scale(1.,"width");
            //rclus->Scale(1/trials);
            vecSpecMC[radius-minradius].push_back(rspec);
            vecClusMC[radius-minradius].push_back(rclus);
        }
    }

    TCanvas* canvas       = new TCanvas("canvas","",10,10,750,500);
    double leftMargin   = 0.075;
    double rightMargin  = 0.01;
    double topMargin    = 0.01;
    double bottomMargin = 0.1;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->cd();

    TH1D *dummyRpA = new TH1D("dummyRpA","",350,0,350);
    dummyRpA->GetXaxis()->SetRangeUser(0,260);
    dummyRpA->GetYaxis()->SetRangeUser(0,2);
    SetStyleHistoTH1ForGraphs(dummyRpA,"","#it{p}_{T} (GeV/c)","#it{R}_{pPb}",textSize,textSize*(4/3),textSize,textSize*(4/3),1.1,0.9);

    TH1D *dummySpectraMB = new TH1D("dummySpectraMB","",350,0,350);
    dummySpectraMB->GetYaxis()->SetRangeUser(2e-9,2e-3);
    dummySpectraMB->GetXaxis()->SetRangeUser(0,260);
    SetStyleHistoTH1ForGraphs(dummySpectraMB,"","#it{p}_{T} (GeV/c)","Raw Yield / NEvents",textSize,textSize*(4/3),textSize,textSize*(4/3),1.1,1);

    TH1D *dummySpectraL1 = new TH1D("dummySpectra","",350,0,350);
    dummySpectraL1->GetYaxis()->SetRangeUser(7e-6,2);
    dummySpectraL1->GetXaxis()->SetRangeUser(0,260);
    SetStyleHistoTH1ForGraphs(dummySpectraL1,"","#it{p}_{T} (GeV/c)","Raw Yield / NEvents",textSize,textSize*(4/3),textSize,textSize*(4/3),1.1,1);

    TLine * l1 = new TLine (0,1,260,1);
    l1->SetLineColor(14);
    l1->SetLineWidth(3);
    l1->SetLineStyle(7);

    TLegend *legend =  GetAndSetLegend2(0.72,(0.8-(2)*textSize),0.82,0.8,textSize,1);

    for(int radius = minradius; radius <= maxradius; radius++){
        // Plot R_pPb for the INT7 trigger in data
        TH1D *dummyINT7data = (TH1D*)vecSpecData[radius-minradius].at(0)->Clone(Form("RpA_INT7_data_R0%i",radius));
        dummyINT7data->Divide(vecSpecData[radius-minradius].at(3),vecSpecData[radius-minradius].at(0));
        dummyINT7data->Sumw2();
        dummyINT7data->Scale(RpA_scale);
        dummyINT7data->SetMarkerStyle(styles[0]);
        dummyINT7data->SetLineColor(colors[0]);
        dummyINT7data->SetMarkerColor(colors[0]);
        dummyINT7data->GetXaxis()->SetRangeUser(20,240);

        dummyRpA->Draw("axis");
        dummyINT7data->Draw("p,e1,same");
        l1->Draw("same");

        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.96,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.96,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data - Min Bias Trigger",0.96,0.81, textSize,kFALSE, kFALSE, true);

        canvas->SaveAs(Form("%s/RpA_R0%i_INT7_Data.%s", outputdir.Data(), radius, fileType.Data()));

        // Plot R_pPb for the L1 jet trigger in data
        TH1D *dummyL1data = (TH1D*)vecSpecData[radius-minradius].at(2)->Clone(Form("RpA_L1Jet_data_R0%i",radius));
        dummyL1data->Divide(vecSpecData[radius-minradius].at(5),vecSpecData[radius-minradius].at(2));
        dummyL1data->Sumw2();
        dummyL1data->Scale(RpA_scale);
        dummyL1data->SetMarkerStyle(styles[0]);
        dummyL1data->SetLineColor(colors[0]);
        dummyL1data->SetMarkerColor(colors[0]);
        dummyL1data->GetXaxis()->SetRangeUser(20,240);

        dummyRpA->Draw("axis");
        dummyL1data->Draw("p,e1,same");
        l1->Draw("same");

        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.96,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.96,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data - L1 Jet Trigger",0.96,0.81, textSize,kFALSE, kFALSE, true);

        canvas->SaveAs(Form("%s/RpA_R0%i_L1_Data.%s", outputdir.Data(), radius, fileType.Data()));

        // Plot R_pPb for the INT7 trigger in simulation
        TH1D *dummyINT7MC = (TH1D*)vecSpecMC[radius-minradius].at(0)->Clone(Form("RpA_INT7_MC_R0%i",radius));
        dummyINT7MC->Divide(vecSpecMC[radius-minradius].at(3),vecSpecMC[radius-minradius].at(0));
        dummyINT7MC->Sumw2();
        //dummyINT7MC->Scale(RpA_scale);
        dummyINT7MC->SetMarkerStyle(styles[0]);
        dummyINT7MC->SetLineColor(colors[0]);
        dummyINT7MC->SetMarkerColor(colors[0]);
        dummyINT7MC->GetXaxis()->SetRangeUser(20,240);

        dummyRpA->Draw("axis");
        dummyINT7MC->Draw("p,e1,same");
        l1->Draw("same");

        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.96,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.96,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation - Min Bias Trigger",0.96,0.81, textSize,kFALSE, kFALSE, true);

        canvas->SaveAs(Form("%s/RpA_R0%i_INT7_MC.%s", outputdir.Data(), radius, fileType.Data()));

        // Plot R_pPb for the L1 jet trigger in simulation
        TH1D *dummyL1MC = (TH1D*)vecSpecMC[radius-minradius].at(2)->Clone(Form("RpA_L1Jet_MC_R0%i",radius));
        dummyL1MC->Divide(vecSpecMC[radius-minradius].at(5),vecSpecMC[radius-minradius].at(2));
        dummyL1MC->Sumw2();
        //dummyL1MC->Scale(RpA_scale);
        dummyL1MC->SetMarkerStyle(styles[0]);
        dummyL1MC->SetLineColor(colors[0]);
        dummyL1MC->SetMarkerColor(colors[0]);
        dummyL1MC->GetXaxis()->SetRangeUser(20,240);

        dummyRpA->Draw("axis");
        dummyL1MC->Draw("p,e1,same");
        l1->Draw("same");

        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.96,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.96,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation - L1 Jet Trigger",0.96,0.81, textSize,kFALSE, kFALSE, true);

        canvas->SaveAs(Form("%s/RpA_R0%i_L1_MC.%s", outputdir.Data(), radius, fileType.Data()));

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        canvas->SetLogy();

        TH1D *specINT7dataPP = (TH1D*)vecSpecData[radius-minradius].at(0)->Clone(Form("specINT7dataPP_R0%i",radius));
        TH1D *specINT7dataPPb = (TH1D*)vecSpecData[radius-minradius].at(3)->Clone(Form("specINT7dataPPb_R0%i",radius));

        TH1D *specL1dataPP = (TH1D*)vecSpecData[radius-minradius].at(2)->Clone(Form("specL1dataPP_R0%i",radius));
        TH1D *specL1dataPPb = (TH1D*)vecSpecData[radius-minradius].at(5)->Clone(Form("specL1dataPPb_R0%i",radius));

        TH1D *specINT7MCPP = (TH1D*)vecSpecMC[radius-minradius].at(0)->Clone(Form("specINT7MCPP_R0%i",radius));
        TH1D *specINT7MCPPb = (TH1D*)vecSpecMC[radius-minradius].at(3)->Clone(Form("specINT7MCPPb_R0%i",radius));

        TH1D *specL1MCPP = (TH1D*)vecSpecMC[radius-minradius].at(2)->Clone(Form("specL1MCPP_R0%i",radius));
        TH1D *specL1MCPPb = (TH1D*)vecSpecMC[radius-minradius].at(5)->Clone(Form("specL1MCPPb_R0%i",radius));

        // Plot INT7 trigger from pp and pPb in data in one plot
        specINT7dataPP->SetMarkerStyle(styles[0]);
        specINT7dataPP->SetMarkerColor(colors[0]);
        specINT7dataPP->SetLineColor(colors[0]);
        specINT7dataPP->Scale(xsec_pp);
        specINT7dataPP->GetXaxis()->SetRangeUser(20,240);
        legend->AddEntry(specINT7dataPP, "pp * x-sec", "p");

        specINT7dataPPb->SetMarkerStyle(styles[1]);
        specINT7dataPPb->SetMarkerColor(colors[1]);
        specINT7dataPPb->SetLineColor(colors[1]);
        specINT7dataPPb->Scale(xsec_pPb/nucfactor);
        specINT7dataPPb->GetXaxis()->SetRangeUser(20,240);
        legend->AddEntry(specINT7dataPPb, "p--Pb * x-sec/208", "p");

        dummySpectraMB->Draw("axis");
        specINT7dataPP->Draw("p,e1,same");
        specINT7dataPPb->Draw("p,e1,same");
        legend->Draw("same");

        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.96,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.96,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data - Min Bias Trigger",0.96,0.81, textSize,kFALSE, kFALSE, true);

        canvas->SaveAs(Form("%s/JetSpectra_INT7_R0%i_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot L1 jet trigger from pp and pPb in data in one plot
        specL1dataPP->SetMarkerStyle(styles[0]);
        specL1dataPP->SetMarkerColor(colors[0]);
        specL1dataPP->SetLineColor(colors[0]);
        specL1dataPP->Scale(xsec_pp);
        specL1dataPP->GetXaxis()->SetRangeUser(20,240);
        legend->AddEntry(specL1dataPP, "pp * x-sec", "p");

        specL1dataPPb->SetMarkerStyle(styles[1]);
        specL1dataPPb->SetMarkerColor(colors[1]);
        specL1dataPPb->SetLineColor(colors[1]);
        specL1dataPPb->Scale(xsec_pPb/nucfactor);
        specL1dataPPb->GetXaxis()->SetRangeUser(20,240);
        legend->AddEntry(specL1dataPPb, "p--Pb * x-sec/208", "p");

        dummySpectraL1->Draw("axis");
        specL1dataPP->Draw("p,e1,same");
        specL1dataPPb->Draw("p,e1,same");
        legend->Draw("same");

        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.96,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.96,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data - L1 Jet Trigger",0.96,0.81, textSize,kFALSE, kFALSE, true);

        canvas->SaveAs(Form("%s/JetSpectra_L1_R0%i_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot INT7 trigger from pp and pPb in MC in one plot
        specINT7MCPP->SetMarkerStyle(styles[0]);
        specINT7MCPP->SetMarkerColor(colors[0]);
        specINT7MCPP->SetLineColor(colors[0]);
        //specINT7MCPP->Scale(xsec_pp);
        specINT7MCPP->GetXaxis()->SetRangeUser(20,240);
        legend->AddEntry(specINT7MCPP, "pp MC", "p");

        specINT7MCPPb->SetMarkerStyle(styles[1]);
        specINT7MCPPb->SetMarkerColor(colors[1]);
        specINT7MCPPb->SetLineColor(colors[1]);
        //specINT7MCPPb->Scale(xsec_pPb/nucfactor);
        specINT7MCPPb->GetXaxis()->SetRangeUser(20,240);
        legend->AddEntry(specINT7MCPPb, "p--Pb MC", "p");

        dummySpectraMB->Draw("axis");
        specINT7MCPP->Draw("p,e1,same");
        specINT7MCPPb->Draw("p,e1,same");
        legend->Draw("same");

        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.96,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.96,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation - Min Bias Trigger",0.96,0.81, textSize,kFALSE, kFALSE, true);

        canvas->SaveAs(Form("%s/JetSpectra_INT7_R0%i_MC.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot L1 jet trigger from pp and pPb in MC in one plot
        specL1MCPP->SetMarkerStyle(styles[0]);
        specL1MCPP->SetMarkerColor(colors[0]);
        specL1MCPP->SetLineColor(colors[0]);
        //specL1MCPP->Scale(xsec_pp);
        specL1MCPP->GetXaxis()->SetRangeUser(20,240);
        legend->AddEntry(specL1MCPP, "pp MC", "p");

        specL1MCPPb->SetMarkerStyle(styles[1]);
        specL1MCPPb->SetMarkerColor(colors[1]);
        specL1MCPPb->SetLineColor(colors[1]);
        //specL1MCPPb->Scale(xsec_pPb/nucfactor);
        specL1MCPPb->GetXaxis()->SetRangeUser(20,240);
        legend->AddEntry(specL1MCPPb, "p--Pb MC", "p");

        dummySpectraL1->Draw("axis");
        specL1MCPP->Draw("p,e1,same");
        specL1MCPPb->Draw("p,e1,same");
        legend->Draw("same");

        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.96,0.91, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.96,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation - L1 Jet Trigger",0.96,0.81, textSize,kFALSE, kFALSE, true);

        canvas->SaveAs(Form("%s/JetSpectra_L1_R0%i_MC.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();
    }
}
