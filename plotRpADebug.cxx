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
    vector<double> detlevelbin = getJetPtBinningNonLinSmearPoor();
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
            TH1D *spec1d    = (TH1D*)spec2d->ProjectionY(Form("dataspec1d_R0%i",radius),1,1);
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

            TString dirname     = Form("JetSpectrum_%sJets_R0%i_%s_nodownscalecorr", jetType.Data(), radius, triggers.at(t).Data());

            TDirectory *dir = (TDirectory*)fmc->Get(dirname.Data());
            if(!dir){
                dirname     = Form("JetSpectrum_%sJets_R0%i_%s_default", jetType.Data(), radius, triggers.at(t).Data());
                dir = (TDirectory*)fmc->Get(dirname.Data());
            }
            TList *list     = (TList*)dir->Get(dirname.Data());
            TH1D *events    = (TH1D*)list->FindObject("hClusterCounter");
            TH2D *spec2d    = (TH2D*)list->FindObject("hJetSpectrum");
            TH1D *clus1d    = (TH1D*)list->FindObject("hClusterEnergy1D");
            TH1D *spec1d    = (TH1D*)spec2d->ProjectionY(Form("dataspec1d_R0%i",radius),1,1);
            TH1D *rspec     = (TH1D*)spec1d->Rebin(detlevelbin.size()-1, Form("rmcspec_%i_%s",radius,triggers.at(t).Data()), detlevelbin.data());
            TH1D *rclus     = (TH1D*)spec1d->Rebin(detlevelbin.size()-1, Form("rmcclus_%i_%s",radius,triggers.at(t).Data()), detlevelbin.data());
            rspec->Scale(1.,"width");
            rspec->Scale(1/events->GetBinContent(1));
            rclus->Scale(1.,"width");
            rclus->Scale(1/events->GetBinContent(1));
            vecSpecMC[radius-minradius].push_back(rspec);
            vecClusMC[radius-minradius].push_back(rclus);
        }
    }

    TCanvas* canvas       = new TCanvas("canvas","",10,10,750,500);
    double leftMargin   = 0.1;
    double rightMargin  = 0.02;
    double topMargin    = 0.04;
    double bottomMargin = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogy();

    TLegend *legend =  GetAndSetLegend2(0.72,(0.82-(2)*textSize),0.82,0.82,textSize,1);

    for(int radius = minradius; radius <= maxradius; radius++){
/*        // Plot data jets for all pp triggers in one plot
        for(int i=0; i<3; i++){
            vecSpecData[radius-minradius].at(i)->SetMarkerStyle(styles[i]);
            vecSpecData[radius-minradius].at(i)->SetMarkerColor(colors[i]);
            vecSpecData[radius-minradius].at(i)->SetLineColor(colors[i]);
            vecSpecData[radius-minradius].at(i)->GetXaxis()->SetRangeUser(20,240);
            //vecSpecData[radius-minradius].at(i)->GetYaxis()->SetRangeUser(1e-9,1);
            SetStyleHistoTH1ForGraphs(vecSpecData[radius-minradius].at(i),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
            if(i==0) vecSpecData[radius-minradius].at(i)->Draw("p,e");
            else vecSpecData[radius-minradius].at(i)->Draw("p,e,same");
            legend->AddEntry(vecSpecData[radius-minradius].at(i), triggers.at(i).Data(), "p");
        }
        legend->Draw();
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/JetSpectra_pp_R0%i_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot data jets for all pPb triggers in one plot
        for(int i=3; i<6; i++){
            vecSpecData[radius-minradius].at(i)->SetMarkerStyle(styles[i]);
            vecSpecData[radius-minradius].at(i)->SetMarkerColor(colors[i]);
            vecSpecData[radius-minradius].at(i)->SetLineColor(colors[i]);
            vecSpecData[radius-minradius].at(i)->GetXaxis()->SetRangeUser(20,240);
            //vecSpecData[radius-minradius].at(i)->GetYaxis()->SetRangeUser(1e-9,1);
            SetStyleHistoTH1ForGraphs(vecSpecData[radius-minradius].at(i),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
            if(i==3) vecSpecData[radius-minradius].at(i)->Draw("p,e");
            else vecSpecData[radius-minradius].at(i)->Draw("p,e,same");
            legend->AddEntry(vecSpecData[radius-minradius].at(i), triggers.at(i).Data(), "p");
        }
        legend->Draw();
        drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/JetSpectra_pPb_R0%i_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();
*/
        // Plot simulation jets for all pp triggers in one plot
        for(int i=0; i<3; i++){
            vecSpecMC[radius-minradius].at(i)->SetMarkerStyle(styles[i]);
            vecSpecMC[radius-minradius].at(i)->SetMarkerColor(colors[i]);
            vecSpecMC[radius-minradius].at(i)->SetLineColor(colors[i]);
            vecSpecMC[radius-minradius].at(i)->GetXaxis()->SetRangeUser(20,240);
            //vecSpecMC[radius-minradius].at(i)->GetYaxis()->SetRangeUser(1e-9,1);
            SetStyleHistoTH1ForGraphs(vecSpecMC[radius-minradius].at(i),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
            if(i==3) vecSpecMC[radius-minradius].at(i)->Draw("p,e");
            else vecSpecMC[radius-minradius].at(i)->Draw("p,e,same");
            legend->AddEntry(vecSpecMC[radius-minradius].at(i), triggers.at(i).Data(), "p");
        }
        legend->Draw();
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/JetSpectra_pp_R0%i_Sim.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot simulation jets for all pPb triggers in one plot
        for(int i=3; i<6; i++){
            vecSpecMC[radius-minradius].at(i)->SetMarkerStyle(styles[i]);
            vecSpecMC[radius-minradius].at(i)->SetMarkerColor(colors[i]);
            vecSpecMC[radius-minradius].at(i)->SetLineColor(colors[i]);
            vecSpecMC[radius-minradius].at(i)->GetXaxis()->SetRangeUser(20,240);
            //vecSpecMC[radius-minradius].at(i)->GetYaxis()->SetRangeUser(1e-9,1);
            SetStyleHistoTH1ForGraphs(vecSpecMC[radius-minradius].at(i),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
            if(i==3) vecSpecMC[radius-minradius].at(i)->Draw("p,e");
            else vecSpecMC[radius-minradius].at(i)->Draw("p,e,same");
            legend->AddEntry(vecSpecMC[radius-minradius].at(i), triggers.at(i).Data(), "p");
        }
        legend->Draw();
        drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/JetSpectra_pPb_R0%i_Sim.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
        // Plot data clusters for all pp triggers in one plot
        for(int i=0; i<3; i++){
            vecClusData[radius-minradius].at(i)->SetMarkerStyle(styles[i]);
            vecClusData[radius-minradius].at(i)->SetMarkerColor(colors[i]);
            vecClusData[radius-minradius].at(i)->SetLineColor(colors[i]);
            vecClusData[radius-minradius].at(i)->GetXaxis()->SetRangeUser(20,240);
            //vecClusData[radius-minradius].at(i)->GetYaxis()->SetRangeUser(1e-9,1);
            SetStyleHistoTH1ForGraphs(vecClusData[radius-minradius].at(i),"","E^{cluster}","#frac{1}{N^{trig}} #frac{dN}{dE^{cluster}}",0.03,0.04,0.03,0.04,1,1.1);
            if(i==0) vecClusData[radius-minradius].at(i)->Draw("p,e");
            else vecClusData[radius-minradius].at(i)->Draw("p,e,same");
            legend->AddEntry(vecClusData[radius-minradius].at(i), triggers.at(i).Data(), "p");
        }
        legend->Draw();
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/ClusterSpectra_pp_R0%i_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot data clusters for all pPb triggers in one plot
        for(int i=3; i<6; i++){
            vecClusData[radius-minradius].at(i)->SetMarkerStyle(styles[i]);
            vecClusData[radius-minradius].at(i)->SetMarkerColor(colors[i]);
            vecClusData[radius-minradius].at(i)->SetLineColor(colors[i]);
            vecClusData[radius-minradius].at(i)->GetXaxis()->SetRangeUser(20,240);
            //vecClusData[radius-minradius].at(i)->GetYaxis()->SetRangeUser(1e-9,1);
            SetStyleHistoTH1ForGraphs(vecClusData[radius-minradius].at(i),"","E^{cluster}","#frac{1}{N^{trig}} #frac{dN}{dE^{cluster}}",0.03,0.04,0.03,0.04,1,1.1);
            if(i==3) vecClusData[radius-minradius].at(i)->Draw("p,e");
            else vecClusData[radius-minradius].at(i)->Draw("p,e,same");
            legend->AddEntry(vecClusData[radius-minradius].at(i), triggers.at(i).Data(), "p");
        }
        legend->Draw();
        drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/ClusterSpectra_pPb_R0%i_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();
*/
        // Plot simulation clusters for all pp triggers in one plot
        for(int i=0; i<3; i++){
            vecClusMC[radius-minradius].at(i)->SetMarkerStyle(styles[i]);
            vecClusMC[radius-minradius].at(i)->SetMarkerColor(colors[i]);
            vecClusMC[radius-minradius].at(i)->SetLineColor(colors[i]);
            vecClusMC[radius-minradius].at(i)->GetXaxis()->SetRangeUser(20,240);
            //vecClusMC[radius-minradius].at(i)->GetYaxis()->SetRangeUser(1e-9,1);
            SetStyleHistoTH1ForGraphs(vecClusMC[radius-minradius].at(i),"","E^{cluster}","#frac{1}{N^{trig}} #frac{dN}{dE^{cluster}}",0.03,0.04,0.03,0.04,1,1.1);
            if(i==0) vecClusMC[radius-minradius].at(i)->Draw("p,e");
            else vecClusMC[radius-minradius].at(i)->Draw("p,e,same");
            legend->AddEntry(vecClusMC[radius-minradius].at(i), triggers.at(i).Data(), "p");
        }
        legend->Draw();
        drawLatexAdd("pp #sqrt{#it{s}_{NN}} = 8 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/ClusterSpectra_pp_R0%i_Sim.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot simulation clusters for all pPb triggers in one plot
        for(int i=3; i<6; i++){
            vecClusMC[radius-minradius].at(i)->SetMarkerStyle(styles[i]);
            vecClusMC[radius-minradius].at(i)->SetMarkerColor(colors[i]);
            vecClusMC[radius-minradius].at(i)->SetLineColor(colors[i]);
            vecClusMC[radius-minradius].at(i)->GetXaxis()->SetRangeUser(20,240);
            //vecClusMC[radius-minradius].at(i)->GetYaxis()->SetRangeUser(1e-9,1);
            SetStyleHistoTH1ForGraphs(vecClusMC[radius-minradius].at(i),"","E^{cluster}","#frac{1}{N^{trig}} #frac{dN}{dE^{cluster}}",0.03,0.04,0.03,0.04,1,1.1);
            if(i==3) vecClusMC[radius-minradius].at(i)->Draw("p,e");
            else vecClusMC[radius-minradius].at(i)->Draw("p,e,same");
            legend->AddEntry(vecClusMC[radius-minradius].at(i), triggers.at(i).Data(), "p");
        }
        legend->Draw();
        drawLatexAdd("p--Pb #sqrt{#it{s}_{NN}} = 8.16 TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/ClusterSpectra_pPb_R0%i_Sim.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Plot R_pPb for the INT7 trigger in data
        TH1D *dummyINT7data = (TH1D*)vecSpecData[radius-minradius].at(0)->Clone("RpA_INT7_data");
        dummyINT7data->Divide(vecSpecData[radius-minradius].at(3),vecSpecData[radius-minradius].at(0));
        dummyINT7data->Scale(RpA_scale);
        dummyINT7data->SetMarkerStyle(styles[0]);
        dummyINT7data->SetLineColor(colors[0]);
        dummyINT7data->SetMarkerColor(colors[0]);
        dummyINT7data->GetXaxis()->SetRangeUser(20,240);
        //dummyINT7data->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(dummyINT7data,"","#it{p}_{T} (GeV/c)","#it{R}_{pPb}",0.03,0.04,0.03,0.04,1,1.2);
        dummyINT7data->Draw("p,e");
        TLine *lINT7data = new TLine(20,1,240,1);
        lINT7data->SetLineColor(14);
        lINT7data->SetLineWidth(3);
        lINT7data->SetLineStyle(7);
        lINT7data->Draw("same");
        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data - Min Bias Trigger",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/RpA_R0%i_INT7_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot R_pPb for the L1 jet trigger in data
        TH1D *dummyL1data = (TH1D*)vecSpecData[radius-minradius].at(2)->Clone("RpA_L1Jet_data");
        dummyL1data->Divide(vecSpecData[radius-minradius].at(5),vecSpecData[radius-minradius].at(2));
        dummyL1data->Scale(RpA_scale);
        dummyL1data->SetMarkerStyle(styles[0]);
        dummyL1data->SetLineColor(colors[0]);
        dummyL1data->SetMarkerColor(colors[0]);
        dummyL1data->GetXaxis()->SetRangeUser(20,240);
        //dummyL1data->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(dummyL1data,"","#it{p}_{T} (GeV/c)","#it{R}_{pPb} (Sorta)",0.03,0.04,0.03,0.04,1,1.2);
        dummyL1data->Draw("p,e");
        TLine *lL1data = new TLine(20,1,240,1);
        lL1data->SetLineColor(14);
        lL1data->SetLineWidth(3);
        lL1data->SetLineStyle(7);
        lL1data->Draw("same");
        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data - L1 Jet Trigger",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/RpA_R0%i_L1_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot R_pPb for the INT7 trigger in simulation
        TH1D *dummyINT7MC = (TH1D*)vecSpecMC[radius-minradius].at(0)->Clone("RpA_INT7_MC");
        dummyINT7MC->Divide(vecSpecMC[radius-minradius].at(3),vecSpecMC[radius-minradius].at(0));
        dummyINT7MC->Scale(RpA_scale);
        dummyINT7MC->SetMarkerStyle(styles[0]);
        dummyINT7MC->SetLineColor(colors[0]);
        dummyINT7MC->SetMarkerColor(colors[0]);
        dummyINT7MC->GetXaxis()->SetRangeUser(20,240);
        //dummyINT7MC->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(dummyINT7MC,"","#it{p}_{T} (GeV/c)","#it{R}_{pPb} (Sorta)",0.03,0.04,0.03,0.04,1,1.2);
        dummyINT7MC->Draw("p,e");
        TLine *lINT7MC = new TLine(20,1,240,1);
        lINT7MC->SetLineColor(14);
        lINT7MC->SetLineWidth(3);
        lINT7MC->SetLineStyle(7);
        lINT7MC->Draw("same");
        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation - Min Bias Trigger",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/RpA_R0%i_INT7_MC.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot R_pPb for the L1 jet trigger in simulation
        TH1D *dummyL1MC = (TH1D*)vecSpecMC[radius-minradius].at(2)->Clone("RpA_L1Jet_MC");
        dummyL1MC->Divide(vecSpecMC[radius-minradius].at(5),vecSpecMC[radius-minradius].at(2));
        dummyL1MC->Scale(RpA_scale);
        dummyL1MC->SetMarkerStyle(styles[0]);
        dummyL1MC->SetLineColor(colors[0]);
        dummyL1MC->SetMarkerColor(colors[0]);
        dummyL1MC->GetXaxis()->SetRangeUser(20,240);
        //dummyL1MC->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(dummyL1MC,"","#it{p}_{T} (GeV/c)","#it{R}_{pPb} (Sorta)",0.03,0.04,0.03,0.04,1,1.2);
        dummyL1MC->Draw("p,e");
        TLine *lL1MC = new TLine(20,1,240,1);
        lL1MC->SetLineColor(14);
        lL1MC->SetLineWidth(3);
        lL1MC->SetLineStyle(7);
        lL1MC->Draw("same");
        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation - L1 Jet Trigger",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/RpA_R0%i_L1_MC.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Plot INT7 trigger from pp and pPb in data in one plot
        vecSpecData[radius-minradius].at(0)->SetMarkerStyle(styles[0]);
        vecSpecData[radius-minradius].at(0)->SetMarkerColor(colors[0]);
        vecSpecData[radius-minradius].at(0)->SetLineColor(colors[0]);
        vecSpecData[radius-minradius].at(0)->Scale(xsec_pp);
        vecSpecData[radius-minradius].at(0)->GetXaxis()->SetRangeUser(20,240);
        //vecSpecData[radius-minradius].at(0)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpecData[radius-minradius].at(0),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpecData[radius-minradius].at(0)->Draw("p,e");
        legend->AddEntry(vecSpecData[radius-minradius].at(0), "pp * x-sec", "p");
        vecSpecData[radius-minradius].at(3)->SetMarkerStyle(styles[1]);
        vecSpecData[radius-minradius].at(3)->SetMarkerColor(colors[1]);
        vecSpecData[radius-minradius].at(3)->SetLineColor(colors[1]);
        vecSpecData[radius-minradius].at(3)->Scale(xsec_pPb/nucfactor);
        vecSpecData[radius-minradius].at(3)->GetXaxis()->SetRangeUser(20,240);
        //vecSpecData[radius-minradius].at(3)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpecData[radius-minradius].at(3),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpecData[radius-minradius].at(3)->Draw("p,e,same");
        legend->AddEntry(vecSpecData[radius-minradius].at(3), "p--Pb * x-sec/208", "p");
        legend->Draw();
        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data - Min Bias Trigger",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/JetSpectra_INT7_R0%i_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot L1 jet trigger from pp and pPb in data in one plot
        vecSpecData[radius-minradius].at(2)->SetMarkerStyle(styles[0]);
        vecSpecData[radius-minradius].at(2)->SetMarkerColor(colors[0]);
        vecSpecData[radius-minradius].at(2)->SetLineColor(colors[0]);
        vecSpecData[radius-minradius].at(2)->Scale(xsec_pp);
        vecSpecData[radius-minradius].at(2)->GetXaxis()->SetRangeUser(20,240);
        //vecSpecData[radius-minradius].at(2)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpecData[radius-minradius].at(2),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpecData[radius-minradius].at(2)->Draw("p,e");
        legend->AddEntry(vecSpecData[radius-minradius].at(2), "pp * x-sec", "p");
        vecSpecData[radius-minradius].at(5)->SetMarkerStyle(styles[1]);
        vecSpecData[radius-minradius].at(5)->SetMarkerColor(colors[1]);
        vecSpecData[radius-minradius].at(5)->SetLineColor(colors[1]);
        vecSpecData[radius-minradius].at(5)->Scale(xsec_pPb/nucfactor);
        vecSpecData[radius-minradius].at(5)->GetXaxis()->SetRangeUser(20,240);
        //vecSpecData[radius-minradius].at(5)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpecData[radius-minradius].at(5),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpecData[radius-minradius].at(5)->Draw("p,e,same");
        legend->AddEntry(vecSpecData[radius-minradius].at(5), "p--Pb * x-sec/208", "p");
        legend->Draw();
        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("ALICE Data - L1 Jet Trigger",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/JetSpectra_L1_R0%i_Data.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot INT7 trigger from pp and pPb in MC in one plot
        vecSpecMC[radius-minradius].at(0)->SetMarkerStyle(styles[0]);
        vecSpecMC[radius-minradius].at(0)->SetMarkerColor(colors[0]);
        vecSpecMC[radius-minradius].at(0)->SetLineColor(colors[0]);
        vecSpecMC[radius-minradius].at(0)->Scale(xsec_pp);
        vecSpecMC[radius-minradius].at(0)->GetXaxis()->SetRangeUser(20,240);
        //vecSpecMC[radius-minradius].at(0)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpecMC[radius-minradius].at(0),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpecMC[radius-minradius].at(0)->Draw("p,e");
        legend->AddEntry(vecSpecMC[radius-minradius].at(0), "pp * x-sec", "p");
        vecSpecMC[radius-minradius].at(3)->SetMarkerStyle(styles[1]);
        vecSpecMC[radius-minradius].at(3)->SetMarkerColor(colors[1]);
        vecSpecMC[radius-minradius].at(3)->SetLineColor(colors[1]);
        vecSpecMC[radius-minradius].at(3)->Scale(xsec_pPb/nucfactor);
        vecSpecMC[radius-minradius].at(3)->GetXaxis()->SetRangeUser(20,240);
        //vecSpecMC[radius-minradius].at(3)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpecMC[radius-minradius].at(3),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpecMC[radius-minradius].at(3)->Draw("p,e,same");
        legend->AddEntry(vecSpecMC[radius-minradius].at(3), "p--Pb * x-sec/208", "p");
        legend->Draw();
        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation - Min Bias Trigger",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/JetSpectra_INT7_R0%i_MC.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();

        // Plot L1 jet trigger from pp and pPb in MC in one plot
        vecSpecMC[radius-minradius].at(2)->SetMarkerStyle(styles[0]);
        vecSpecMC[radius-minradius].at(2)->SetMarkerColor(colors[0]);
        vecSpecMC[radius-minradius].at(2)->SetLineColor(colors[0]);
        vecSpecMC[radius-minradius].at(2)->Scale(xsec_pp);
        vecSpecMC[radius-minradius].at(2)->GetXaxis()->SetRangeUser(20,240);
        //vecSpecMC[radius-minradius].at(2)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpecMC[radius-minradius].at(2),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpecMC[radius-minradius].at(2)->Draw("p,e");
        legend->AddEntry(vecSpecMC[radius-minradius].at(2), "pp * x-sec", "p");
        vecSpecMC[radius-minradius].at(5)->SetMarkerStyle(styles[1]);
        vecSpecMC[radius-minradius].at(5)->SetMarkerColor(colors[1]);
        vecSpecMC[radius-minradius].at(5)->SetLineColor(colors[1]);
        vecSpecMC[radius-minradius].at(5)->Scale(xsec_pPb/nucfactor);
        vecSpecMC[radius-minradius].at(5)->GetXaxis()->SetRangeUser(20,240);
        //vecSpecMC[radius-minradius].at(5)->GetYaxis()->SetRangeUser(1e-9,1);
        SetStyleHistoTH1ForGraphs(vecSpecMC[radius-minradius].at(5),"","p_{T}^{jet}","#frac{1}{N^{trig}} #frac{dN}{dp_{T}^{jet}}",0.03,0.04,0.03,0.04,1,1.1);
        vecSpecMC[radius-minradius].at(5)->Draw("p,e,same");
        legend->AddEntry(vecSpecMC[radius-minradius].at(5), "p--Pb * x-sec/208", "p");
        legend->Draw();
        drawLatexAdd("pp [p--Pb] #sqrt{#it{s}_{NN}} = 8 [8.16] TeV",0.93,0.90, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("Full Jets, Anti-#it{k}_{T}",0.93,0.86, textSize,kFALSE, kFALSE, true);
        drawLatexAdd("PYTHIA8 Simulation - L1 Jet Trigger",0.93,0.82, textSize,kFALSE, kFALSE, true);
        canvas->SaveAs(Form("%s/JetSpectra_L1_R0%i_MC.%s", outputdir.Data(), radius, fileType.Data()));
        legend->Clear();
    }
}
