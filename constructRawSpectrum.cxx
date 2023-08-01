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

void constructRawSpectrum(TString int7file, TString ej2file, TString ej1file, TString mcfileEJ, TString output, TString filetype, int radius){
    double rfactorEJ2 = 2012.38;
    double rfactorEJ1 = 2.66;
    double textSize = 0.04;

    vector<double> binningJets     = getJetPtBinningNonLinSmear8TeV();
    vector<double> binningClusters = getJetPtBinningRejectionFactorsFine();

    int styles[10] = {8,21,33,34,41,43,45,47,48,49};
    int colors[10] = {1,2,4,8,9,30,40,41,46,49};

    vector<TString> triggers = {"INT7", "EJ2", "EJ1"};
    vector<TH1D*> jetsMC;
    vector<TH1D*> clustersMC;

    TFile *fint7 = TFile::Open(int7file);
    TFile *fej2 = TFile::Open(ej2file);
    TFile *fej1 = TFile::Open(ej1file);
    TFile *fmcej = TFile::Open(mcfileEJ);
    
    vector<TFile*> files{fint7, fej2, fej1};
    vector<TString> histosName{"hClusterCounter", "hJetSpectrum", "hClusterEnergy1D", "fNormalisationHist"};
    vector<TH1D*> histos[3];

    // Get hClusterCounter, hJetSpectrum, hClusterEnergy1D from the TList within the TDirectory for the given radius and each trigger
    for(int t = 0; t < triggers.size(); t++){
        int file_t = 0;
        if(t == 0) file_t = 0;
        else if(t > 0 && t < 4) file_t = 1;
        else file_t = 2;

        TString dirname = Form("JetSpectrum_FullJets_R0%i_%s_default", radius, triggers.at(t).Data());
        TDirectory *dir = (TDirectory*)files.at(file_t)->Get(dirname.Data());
        TList *list = (TList*)dir->Get(dirname.Data());
        for(int h = 0; h < histosName.size(); h++){
            if(histosName.at(h)=="hJetSpectrum"){
                TH2D *spectrum2D = (TH2D*)list->FindObject(histosName.at(h).Data());
                TH1D *spectrum1D = (TH1D*)spectrum2D->ProjectionY(Form("hJetSpectrum1D_%s", triggers.at(t).Data()), 1, 1);
                TH1D *spectrumRebinned = (TH1D*)spectrum1D->Rebin(binningJets.size()-1, Form("spectrumrebin_%s",triggers.at(t).Data()), binningJets.data());
                spectrumRebinned->Scale(1.,"width");
                histos[t].push_back(spectrumRebinned);
            }else if(histosName.at(h)=="hClusterEnergy1D"){
                TH1D *clusters = (TH1D*)list->FindObject(histosName.at(h).Data());
                TH1D *clustersRebinned = (TH1D*)clusters->Rebin(binningClusters.size()-1, Form("clustersrebin_%s",triggers.at(t).Data()), binningClusters.data());
                clustersRebinned->Scale(1.,"width");
                histos[t].push_back(clustersRebinned);
            }
            else histos[t].push_back((TH1D*)list->FindObject(histosName.at(h).Data()));
        }
        // Get the MC jets and clusters to construct the trigger efficiency
        
            TDirectory *dirmc = (TDirectory*)fmcej->Get(dirname.Data());
            TList *listmc = (TList*)dirmc->Get(dirname.Data());
            TH2D *spectrum2Dmc = (TH2D*)listmc->FindObject("hJetSpectrum");
            TH1D *spectrum1Dmc = (TH1D*)spectrum2Dmc->ProjectionY(Form("hJetSpectrum1D_%s", triggers.at(t).Data()), 1, 1);
            TH1D *spectrumRebinnedmc = (TH1D*)spectrum1Dmc->Rebin(binningJets.size()-1, Form("spectrumrebin_%s",triggers.at(t).Data()), binningJets.data());
            spectrumRebinnedmc->Scale(1.,"width");
            jetsMC.push_back(spectrumRebinnedmc);

            TH1D *clustersmc = (TH1D*)listmc->FindObject("hClusterEnergy1D");
            TH1D *clustersRebinnedmc = (TH1D*)clustersmc->Rebin(binningClusters.size()-1, Form("clustersrebin_%s",triggers.at(t).Data()), binningClusters.data());
            clustersRebinnedmc->Scale(1.,"width");
            clustersMC.push_back(clustersRebinnedmc);
        
    }

    // Scale the hJetSpectrum and hClusterEnergy1D histograms by the number of events and vertex finding efficiency
    for(int t = 0; t < triggers.size(); t++){
        double vtxfeff  = histos[t].at(3)->GetBinContent(histos[t].at(3)->GetXaxis()->FindBin("Vertex reconstruction and quality")) / histos[t].at(3)->GetBinContent(histos[t].at(3)->GetXaxis()->FindBin("Event selection"));
        histos[t].at(1)->Scale(vtxfeff/histos[t].at(0)->GetBinContent(1));
        histos[t].at(2)->Scale(vtxfeff/histos[t].at(0)->GetBinContent(1));
    }

    // Scale the hJetSpectrum and hClusterEnergy1D histograms by trigger efficiency
    
    TH1D *jetefficiencyEJ2 = (TH1D*)jetsMC.at(1)->Clone("jetefficiencyEJ2");
    jetefficiencyEJ2->Divide(jetsMC.at(0));
    TH1D *jetefficiencyEJ1 = (TH1D*)jetsMC.at(2)->Clone("jetefficiencyEJ1");
    jetefficiencyEJ1->Divide(jetsMC.at(0));

    
    TH1D *clusterefficiencyEJ2 = (TH1D*)clustersMC.at(1)->Clone("clusterefficiencyEJ2");
    clusterefficiencyEJ2->Divide(clustersMC.at(0));
    TH1D *clusterefficiencyEJ1 = (TH1D*)clustersMC.at(2)->Clone("clusterefficiencyEJ1");
    clusterefficiencyEJ1->Divide(clustersMC.at(0));

    
    histos[1].at(1)->Divide(jetefficiencyEJ2);
    histos[2].at(1)->Divide(jetefficiencyEJ1);

   
    histos[1].at(2)->Divide(clusterefficiencyEJ2);
    histos[2].at(2)->Divide(clusterefficiencyEJ1);

    // Scale the hJetSpectrum and hClusterEnergy1D histograms by the rejection factors
    
    histos[1].at(1)->Scale(1./rfactorEJ2); // Scale the EJ2 jet spectrum
    histos[2].at(1)->Scale(1./(rfactorEJ1*rfactorEJ2)); // Scale the EJ1 jet spectrum

    
    histos[1].at(2)->Scale(1./rfactorEJ2); // Scale the EJ2 cluster spectrum
    histos[2].at(2)->Scale(1./(rfactorEJ1*rfactorEJ2)); // Scale the EJ1 cluster spectrum

    // Set up canvas for plotting
    TCanvas *c   = new TCanvas("c", "", 1200, 800);
    DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);
    c->SetLogy(1);
    c->SetLogx(0);

    // Make dummy histogram for axes
    TH1D *h = new TH1D("h","",260,0,260);
    SetStyleHistoTH1ForGraphs(h,"","p_{T} (GeV/c)","Raw Jet Spectra",0.03,0.04,0.03,0.04,1,1.2);
    //h->GetXaxis()->SetMoreLogLabels();
    h->GetYaxis()->SetRangeUser(1e-10,2e-3);
    h->Draw("axis");

    // Make legend
    TLegend *legend =  GetAndSetLegend2(0.8,(0.9-(5)*textSize),0.95,0.9,textSize,1);

    // Plot the raw jet spectrum
    for(int t = 0; t < triggers.size(); t++){
        histos[t].at(1)->SetMarkerStyle(styles[t]);
        histos[t].at(1)->SetMarkerColor(colors[t]);
        histos[t].at(1)->SetLineColor(colors[t]);
        histos[t].at(1)->Draw("p,e,same");
        legend->AddEntry(histos[t].at(1), triggers.at(t).Data(), "p");
    }

    legend->Draw("same");

    // Save the plot
    c->SaveAs(Form("%s/rawjetspectra_alltriggers_R0%i.%s", output.Data(), radius, filetype.Data()));

    legend->Clear();
    h->GetYaxis()->SetTitle("Raw Cluster Spectra");
    h->GetYaxis()->SetRangeUser(1e-10,1);
    h->GetXaxis()->SetRangeUser(0,210);
    h->Draw("axis");

    // Plot the raw cluster spectrum
    for(int t = 0; t < triggers.size(); t++){
        histos[t].at(2)->SetMarkerStyle(styles[t]);
        histos[t].at(2)->SetMarkerColor(colors[t]);
        histos[t].at(2)->SetLineColor(colors[t]);
        histos[t].at(2)->Draw("p,e,same");
        legend->AddEntry(histos[t].at(2), triggers.at(t).Data(), "p");
    }

    legend->Draw("same");

    // Save the plot
    c->SaveAs(Form("%s/rawclusterspectra_alltriggers_R0%i.%s", output.Data(), radius, filetype.Data()));

    c->SetLogy(0);

    // Make dummy histogram for axes
    TH1D *h2 = new TH1D("h2","",260,0,260);
    SetStyleHistoTH1ForGraphs(h2,"","p_{T} (GeV/c)","Ratio",0.03,0.04,0.03,0.04,1,1.2);
    h2->GetYaxis()->SetRangeUser(0.2,1.8);
    h2->GetXaxis()->SetRangeUser(10,100);
    h2->Draw("axis");

    // Raw jet spectrum ratio for EJ2/INT7
    TH1D *hJetSpectrumRatioEJ2INT7 = (TH1D*)histos[1].at(1)->Clone("hJetSpectrumRatioEJ2INT7");
    hJetSpectrumRatioEJ2INT7->Divide(histos[0].at(1));
    hJetSpectrumRatioEJ2INT7->SetMarkerStyle(styles[1]);
    hJetSpectrumRatioEJ2INT7->SetMarkerColor(colors[1]);
    hJetSpectrumRatioEJ2INT7->SetLineColor(colors[1]);
    hJetSpectrumRatioEJ2INT7->Draw("p,e,same");
    legend->Clear();
    legend->AddEntry(hJetSpectrumRatioEJ2INT7, "EJ2/INT7", "p");
    legend->Draw("same");
    DrawGammaLines(0.,260.,1.,1.,8.,16,9);
    c->SaveAs(Form("%s/rawjetspectra_ratio_EJ2INT7_R0%i.%s", output.Data(), radius, filetype.Data()));

    h2->Draw("axis");

    // Raw jet spectrum ratio for EJ1/EJ2
    TH1D *hJetSpectrumRatioEJ1EJ2 = (TH1D*)histos[2].at(1)->Clone("hJetSpectrumRatioEJ1EJ2");
    hJetSpectrumRatioEJ1EJ2->Divide(histos[1].at(1));
    hJetSpectrumRatioEJ1EJ2->SetMarkerStyle(styles[2]);
    hJetSpectrumRatioEJ1EJ2->SetMarkerColor(colors[2]);
    hJetSpectrumRatioEJ1EJ2->SetLineColor(colors[2]);
    hJetSpectrumRatioEJ1EJ2->Draw("p,e,same");
    legend->Clear();
    legend->AddEntry(hJetSpectrumRatioEJ1EJ2, "EJ1/EJ2", "p");
    legend->Draw("same");
    DrawGammaLines(0.,260.,1.,1.,8.,16,9);
    c->SaveAs(Form("%s/rawjetspectra_ratio_EJ1EJ2_R0%i.%s", output.Data(), radius, filetype.Data()));

    h2->Draw("axis");

    // Raw jet spectrum ratio for EJ1/INT7
    TH1D *hJetSpectrumRatioEJ1INT7 = (TH1D*)histos[2].at(1)->Clone("hJetSpectrumRatioEJ1INT7");
    hJetSpectrumRatioEJ1INT7->Divide(histos[0].at(1));
    hJetSpectrumRatioEJ1INT7->SetMarkerStyle(styles[2]);
    hJetSpectrumRatioEJ1INT7->SetMarkerColor(colors[2]);
    hJetSpectrumRatioEJ1INT7->SetLineColor(colors[2]);
    hJetSpectrumRatioEJ1INT7->Draw("p,e,same");
    legend->Clear();
    legend->AddEntry(hJetSpectrumRatioEJ1INT7, "EJ1/INT7", "p");
    legend->Draw("same");
    DrawGammaLines(0.,260.,1.,1.,8.,16,9);
    c->SaveAs(Form("%s/rawjetspectra_ratio_EJ1INT7_R0%i.%s", output.Data(), radius, filetype.Data()));

    h2->Draw("axis");

    c->SetLogx(1);

    // Raw cluster spectrum ratio for EJ2/INT7
    TH1D *hClusterSpectrumRatioEJ2INT7 = (TH1D*)histos[1].at(2)->Clone("hClusterSpectrumRatioEJ2INT7");
    hClusterSpectrumRatioEJ2INT7->Divide(histos[0].at(2));
    hClusterSpectrumRatioEJ2INT7->SetMarkerStyle(styles[1]);
    hClusterSpectrumRatioEJ2INT7->SetMarkerColor(colors[1]);
    hClusterSpectrumRatioEJ2INT7->SetLineColor(colors[1]);
    hClusterSpectrumRatioEJ2INT7->Draw("p,e,same");
    legend->Clear();
    legend->AddEntry(hClusterSpectrumRatioEJ2INT7, "EJ2/INT7", "p");
    legend->Draw("same");
    DrawGammaLines(0.,260.,1.,1.,8.,16,9);
    c->SaveAs(Form("%s/rawclusterspectra_ratio_EJ2INT7_R0%i.%s", output.Data(), radius, filetype.Data()));

    h2->Draw("axis");

    // Raw cluster spectrum ratio for EJ1/EJ2
    TH1D *hClusterSpectrumRatioEJ1EJ2 = (TH1D*)histos[2].at(2)->Clone("hClusterSpectrumRatioEJ1EJ2");
    hClusterSpectrumRatioEJ1EJ2->Divide(histos[1].at(2));
    hClusterSpectrumRatioEJ1EJ2->SetMarkerStyle(styles[2]);
    hClusterSpectrumRatioEJ1EJ2->SetMarkerColor(colors[2]);
    hClusterSpectrumRatioEJ1EJ2->SetLineColor(colors[2]);
    hClusterSpectrumRatioEJ1EJ2->Draw("p,e,same");
    legend->Clear();
    legend->AddEntry(hClusterSpectrumRatioEJ1EJ2, "EJ1/EJ2", "p");
    legend->Draw("same");
    DrawGammaLines(0.,260.,1.,1.,8.,16,9);
    c->SaveAs(Form("%s/rawclusterspectra_ratio_EJ1EJ2_R0%i.%s", output.Data(), radius, filetype.Data()));

}