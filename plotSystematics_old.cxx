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

namespace fs = std::filesystem;

void plotSystematics(TString originalFile, TString type, Int_t radius, TString fileType, TString out)
{
    //////////////////////////// Define Variables //////////////////////////////

    Double_t textSize     = 0.03;
    Int_t regnumBayes     = 4;
    Int_t regnumSVD       = 6;
    TString typeCAP;

    Double_t minPt = 20;
    Double_t maxPt = 320;

    int stylesfilled[14] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22};
    int stylesempty[14] = {4,25,27,28,35,36,38,40,42,44,46,26,30,32};
    Color_t colors[15] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4, kRed-1, kGray+1};

    int varNSys = 13;
    if(radius != 2 && radius != 4) varNSys = varNSys - 1;
    if(radius != 2) varNSys = varNSys - 2;
    const int nSys = varNSys;
    vector<TString> plot_names;
    vector<TString> systematics_names;
    vector<TString> variation_original;
    vector<TString> fitfunc;

    if(radius == 2){
        plot_names.insert(plot_names.end(), {"Regularization","Bin Variation","Clusterizer","Max Track p_{T}","Hadronic Correction","Seed/Cell Energy","Tracking Efficiency","Priors","Trigger Swap p_{T}","RF Binning","RF Fit","Unfolding Method","Truncation"});
        systematics_names.insert(systematics_names.end(), {"reg","binvar","clusterizer","maxtrackpt","hadcorr","seed","tracking","priors","triggerswap","rfbinvar","rffit","unfoldingtype","truncation"});
        variation_original.insert(variation_original.end(), {Form("reg=%i",regnumBayes),"default","Clusterizerv2","200GeV","F=1","300MeV","100%","default","(30GeV,60GeV)","(Low=default,High=default)","(Low=1.5GeV,High=6GeV)","Bayes","Default"});
        fitfunc.insert(fitfunc.end(), {"pol0","pol0","pol1","binwise","pol1","pol0","binwise","pol1","binwise","pol1","pol1","pol0","pol0"});
    }else if(radius == 4){
        plot_names.insert(plot_names.end(), {"Regularization","Bin Variation","Clusterizer","Max Track p_{T}","Hadronic Correction","Seed/Cell Energy","Tracking Efficiency","Priors","Trigger Swap p_{T}","Unfolding Method","Truncation"});
        systematics_names.insert(systematics_names.end(), {"reg","binvar","clusterizer","maxtrackpt","hadcorr","seed","tracking","priors","triggerswap","unfoldingtype","truncation"});
        variation_original.insert(variation_original.end(), {Form("reg=%i",regnumBayes),"default","Clusterizerv2","200GeV","F=1","300MeV","100%","default","(30GeV,60GeV)","Bayes","Default"});
        fitfunc.insert(fitfunc.end(), {"pol0","pol0","pol1","binwise","pol1","pol0","binwise","pol1","binwise","pol0","pol0"});
    }else{
        plot_names.insert(plot_names.end(), {"Regularization","Bin Variation","Clusterizer","Hadronic Correction","Seed/Cell Energy","Tracking Efficiency","Priors","Trigger Swap p_{T}","Unfolding Method","Truncation"});
        systematics_names.insert(systematics_names.end(), {"reg","binvar","clusterizer","hadcorr","seed","tracking","priors","triggerswap","unfoldingtype","truncation"});
        variation_original.insert(variation_original.end(), {Form("reg=%i",regnumBayes),"default","Clusterizerv2","F=1","300MeV","100%","default","(30GeV,60GeV)","Bayes",""});
        fitfunc.insert(fitfunc.end(), {"pol0","pol0","pol1","pol1","pol0","binwise","pol1","binwise","pol0","pol0"});
    }
    cout << plot_names.size() << " " << systematics_names.size() << " " << variation_original.size() << " " << fitfunc.size() << " " << endl;

    TString systematicsFile;

    vector<TString> files[nSys];
    vector<TString> variations[nSys];
    vector<TH1D*> syshistos[nSys];
    vector<TH1D*> vecSys;

    if(type == "bayes") typeCAP = "Bayes";
    else if(type == "svd") typeCAP = "SVD";
    else if(type == "Bayes"){ typeCAP = type; type = "bayes"; }
    else if(type == "SVD"){ typeCAP = type; type = "svd"; }
    else{cout << "Invalid unfolding type!" << endl; return;}

    //////////////////////////// Collect the Data //////////////////////////////

    // Open the default file and get default histogram
    TFile *fOrig = TFile::Open(originalFile);
    if(!fOrig || fOrig->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    TDirectory *dRadOrig = (TDirectory*)fOrig->Get(Form("R0%i",radius));
    TDirectory *dRegOrig    = (TDirectory*)dRadOrig->Get(Form("reg%i",regnumBayes));
    TH1D *normUnfoldedOrig = (TH1D*)dRegOrig->Get(Form("normalized_reg%i",regnumBayes));

    // Regularization variation
    TDirectory* dRegOrig_high = (TDirectory*)dRadOrig->Get(Form("reg%i",regnumBayes+1));
    TH1D *specOrig_high      = (TH1D*)dRegOrig_high->Get(Form("normalized_reg%i",regnumBayes+1));
    TDirectory* dRegOrig_low  = (TDirectory*)dRadOrig->Get(Form("reg%i",regnumBayes-1));
    TH1D *specOrig_low       = (TH1D*)dRegOrig_low->Get(Form("normalized_reg%i",regnumBayes-1));

    files[0].push_back(originalFile);
    variations[0].push_back(Form("reg=%i",regnumBayes+1));
    variations[0].push_back(Form("reg=%i",regnumBayes-1));
    syshistos[0].push_back(specOrig_high);
    syshistos[0].push_back(specOrig_low);

    // Save the file names and variations for each uncertainty into vectors
    for(int name=1; name<nSys; name++){
        if(systematics_names[name] == "maxtrackpt"){
            if(radius == 2 || radius == 4) systematicsFile = Form("input_txt_files/%s_%s_R0%i.txt", systematics_names[name].Data(), type.Data(), radius);
            else continue;
        }else if(systematics_names[name].Contains("rf")){
            if(radius == 2) systematicsFile = Form("input_txt_files/%s_%s.txt", systematics_names.at(name).Data(), type.Data());
            else continue;
        }
        else systematicsFile = Form("input_txt_files/%s_%s.txt", systematics_names.at(name).Data(), type.Data());
        ifstream sysfile(systematicsFile);
        if(!sysfile){ cout << "Systematics file " << systematicsFile << " not found!" << endl; return; }
        TString sfile, var;

        while(sysfile >> sfile >> var){
            sfile = "Unfolding/UnfoldingResults/" + sfile;
            files[name].push_back(sfile);
            variations[name].push_back(var);
        }
    }

    // Get the variation histos for all systematics
    for(int name=1; name<nSys; name++){
        for(int i = 0; i < files[name].size(); i++){
            TFile *fVar = TFile::Open(files[name].at(i));
            if(!fVar || fVar->IsZombie()){
                cout << "Error: File " << files[name].at(i) << " not found!" << endl;
                return;
            }
            TDirectory *regDirSys = (TDirectory*)fVar->Get(Form("R0%i",radius));
            TDirectory *regSys;
            TH1D *normUnfoldedSys;
            if(systematics_names[name] == "unfoldingtype"){
                regSys    = (TDirectory*)regDirSys->Get(Form("reg%i",regnumSVD));
                normUnfoldedSys = (TH1D*)regSys->Get(Form("normalized_reg%i",regnumSVD));
            }else{
                regSys    = (TDirectory*)regDirSys->Get(Form("reg%i",regnumBayes));
                normUnfoldedSys = (TH1D*)regSys->Get(Form("normalized_reg%i",regnumBayes));
            }
            syshistos[name].push_back(normUnfoldedSys);
        }
    }

    const char* nameOutput = Form("/home/austin/alice/SystematicsRootFiles/systematics_R0%i.root",radius);
    TFile* fOutput = new TFile(nameOutput,"RECREATE");

    ////////////////////////// Get Statistical Error ///////////////////////////

    TH1D *statError = (TH1D*)normUnfoldedOrig->Clone("ratio");
    TH1D *normUnfoldedOrig_e0 = (TH1D*)normUnfoldedOrig->Clone("zero_error_orig");
    for(int i = 0; i < normUnfoldedOrig_e0->GetNbinsX(); i++) normUnfoldedOrig_e0->SetBinError(i,0);
    statError->Divide(normUnfoldedOrig,normUnfoldedOrig_e0);
    SetStyleHistoTH1ForGraphs(statError,"","p_{T} (GeV/c)","Var/Orig",0.03,0.04,0.03,0.04,1,1.2);
    statError->GetXaxis()->SetRangeUser(minPt,maxPt);
    statError->GetYaxis()->SetRangeUser(0.85,1.15);
    statError->SetFillColor(17);
    statError->SetMarkerStyle(21);
    statError->SetMarkerSize(2.5);
    statError->SetMarkerColor(17);
    //statError->Sumw2();

    //////////////////////////// Plot Systematics //////////////////////////////

    // Declare canvas and legend for plotting
    TCanvas *c;
    TLegend *legend;
    gStyle->SetOptStat(0);

    // Make a line at 1
    TLine *line = new TLine (minPt,1,maxPt,1);
    line->SetLineColor(14);
    line->SetLineWidth(3);
    line->SetLineStyle(7);

    // Start systematics plotting loop
    for(int name=0; name<nSys; name++){
        // Set up canvas and legend
        c = new TCanvas(Form("c_%i",name), "", 1200, 800);
        DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
        legend =  GetAndSetLegend2(0.12,0.14,0.75,0.14+((syshistos[name].size()+2)*1.5*textSize)/2,textSize,2);

        // Redefine variables for every loop
        Double_t val;
        Double_t err;
        vector<Double_t> binvals[syshistos[name].size()];
        vector<Double_t> binerrs[syshistos[name].size()];
        vector<Double_t> avgbinvals;
        vector<Double_t> avgbinerrs;

        // Draw statistical error and add to legend
        statError->Draw("E2");
        legend->AddEntry(statError, "Statistical Uncertainty", "p");

        // Format and plot systematics
        for(int j = 0; j < syshistos[name].size(); j++){
            TH1D *sysRatio = (TH1D*)normUnfoldedOrig->Clone(Form("syshist_%i",j));
            sysRatio->Divide(syshistos[name].at(j),normUnfoldedOrig,1,1,"B");
            sysRatio->SetMarkerStyle(stylesfilled[j+1]);
            sysRatio->SetLineColor(colors[j+1]);
            sysRatio->SetLineWidth(2);
            sysRatio->SetMarkerSize(2.5);
            sysRatio->SetMarkerColor(colors[j+1]);
            for(int k = 0; k < sysRatio->GetNbinsX(); k++){
                val = 1+abs(sysRatio->GetBinContent(k)-1);
                err = sysRatio->GetBinError(k);
                binvals[j].push_back(val);
                binerrs[j].push_back(err);
            }
            sysRatio->Draw("same");
            legend->AddEntry(sysRatio, Form("%s/%s",variations[name].at(j).Data(),variation_original[name].Data()), "p");
        }

        // Get average of all variations for each systematic, format, and plot
        TH1D *sysAvg = (TH1D*)normUnfoldedOrig->Clone("syshist_avg");
        for(int l = 0; l < binvals[0].size(); l++){
            Double_t valsum = 0;
            Double_t avg = 0;
            Double_t errsum = 0;
            Double_t toterr = 0;
            for(int m = 0; m < syshistos[name].size(); m++){
                valsum += binvals[m].at(l);
                errsum += pow(binerrs[m].at(l),2);
            }
            avg = valsum/syshistos[name].size();
            avgbinvals.push_back(avg);
            toterr = sqrt(errsum)/syshistos[name].size();
            avgbinerrs.push_back(toterr);

        }
        for(int k = 0; k < sysAvg->GetNbinsX(); k++){
            sysAvg->SetBinContent(k,avgbinvals.at(k));
            sysAvg->SetBinError(k,avgbinerrs.at(k));
        }
        SetStyleHistoTH1ForGraphs(sysAvg,"","p_{T} (GeV/c)","Systematic Uncertainty (%)",0.03,0.04,0.03,0.04,1,1.2);
        sysAvg->SetMarkerStyle(stylesempty[0]);
        sysAvg->SetLineColor(colors[0]);
        sysAvg->SetLineWidth(2);
        sysAvg->SetMarkerSize(2.5);
        sysAvg->SetMarkerColor(colors[0]);
        sysAvg->Draw("same");
        legend->AddEntry(sysAvg, "Avg. Uncertainty (Abs. Val.)", "p");

        // Draw line at 1 and legend
        line->Draw();
        legend->Draw();

        // Add text to plot and save
        drawLatexAdd(Form("%s Unfolding, Reg %i",typeCAP.Data(),regnumBayes),0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("%s",plot_names[name].Data()),0.95,0.79, 0.03,kFALSE, kFALSE, kTRUE);

        TString savestring1 = Form("%s/Systematics/R0%i/%s_%s_R0%i_reg%i.%s",out.Data(),radius,systematics_names[name].Data(),type.Data(),radius,regnumBayes,fileType.Data());

        //if (!fs::is_directory(savestring1 || !fs::exists(savestring1)) { // Check if src folder exists
        //    fs::create_directory(savestring1); // create src folder
        //}

        c->SaveAs(savestring1);
        c->Clear();
        legend->Clear();

        ///////////////////// Fit and Smooth Systematics ///////////////////////

        // Set up legend
        if(fitfunc[name] == "") legend = GetAndSetLegend2(0.14,0.95,0.44,0.95-(((6)+2)*1.5*textSize)/2,textSize,2);
        else legend = GetAndSetLegend2(0.14,0.95,0.44,0.95+(((1)+2)*1.5*textSize)/2,textSize,2);

        // Make all systematics with respect to zero instead of 1
        for(int i=1; i<sysAvg->GetNbinsX()+1; i++){
            sysAvg->SetBinContent(i,(sysAvg->GetBinContent(i)-1));
        }
        sysAvg->Scale(100);

        // Set up axes
        sysAvg->GetXaxis()->SetRangeUser(minPt,maxPt);
        if(radius <= 3){
            if(systematics_names[name] == "tracking") sysAvg->GetYaxis()->SetRangeUser(0,13);
            else sysAvg->GetYaxis()->SetRangeUser(0,3);
        }else if(radius == 4){
            if(systematics_names[name] == "tracking" || systematics_names[name].Contains("rf")) sysAvg->GetYaxis()->SetRangeUser(0,14);
            else sysAvg->GetYaxis()->SetRangeUser(0,5);
        }else{
            if(systematics_names[name] == "tracking" || systematics_names[name].Contains("rf")) sysAvg->GetYaxis()->SetRangeUser(0,17);
            else sysAvg->GetYaxis()->SetRangeUser(0,15);
        }

        TF1  *sysFit;
        TF1  *pol0;
        TF1  *pol1;
        TF1  *pol2;
        TF1  *pol3;
        TF1  *exp0;
        TH1D *smooth;

        pol0 = new TF1("pol0","[0]",minPt,maxPt);
        pol1 = new TF1("pol1","[0]+[1]*x",minPt,maxPt);
        pol2 = new TF1("pol2","[0]+[1]*x+[2]*x*x",minPt,maxPt);
        pol3 = new TF1("pol3","[0]+[1]*x+[2]*x*x+[3]*x*x*x",minPt,maxPt);
        exp0 = new TF1("exp0","[0]+[1]/expo([2]*x)",minPt,maxPt);
        if(fitfunc[name] == ""){
            pol0->SetLineColor(colors[2]);
            pol1->SetLineColor(colors[3]);
            pol2->SetLineColor(colors[4]);
            pol3->SetLineColor(colors[5]);
            exp0->SetLineColor(colors[6]);

            sysAvg->Fit(pol0, "QRMEX+", "", minPt, maxPt);
            legend->AddEntry(pol0, "pol0", "l");
            sysAvg->Fit(pol1, "QRMEX+", "", minPt, maxPt);
            legend->AddEntry(pol1, "pol1", "l");
            sysAvg->Fit(pol2, "QRMEX+", "", minPt, maxPt);
            legend->AddEntry(pol2, "pol2", "l");
            sysAvg->Fit(pol3, "QRMEX+", "", minPt, maxPt);
            legend->AddEntry(pol3, "pol3", "l");
            sysAvg->Fit(exp0, "QRMEX+", "", minPt, maxPt);
            legend->AddEntry(exp0, "exp0", "l");
        }else{
            if(fitfunc[name] == "pol0")      sysFit = (TF1*)pol0->Clone(Form("sysfit_%i",name));
            else if(fitfunc[name] == "pol1") sysFit = (TF1*)pol1->Clone(Form("sysfit_%i",name));
            else if(fitfunc[name] == "pol2") sysFit = (TF1*)pol2->Clone(Form("sysfit_%i",name));
            else if(fitfunc[name] == "pol3") sysFit = (TF1*)pol3->Clone(Form("sysfit_%i",name));
            else if(fitfunc[name] == "exp0") sysFit = (TF1*)exp0->Clone(Form("sysfit_%i",name));
            else if(fitfunc[name] == "binwise") cout << "Using binwise errors for " << systematics_names[name] << endl;
            else{ cout << "Invalid fit function!" << endl; return; }
            sysFit->SetLineColor(colors[name+1]);

            smooth = (TH1D*)sysAvg->Clone(Form("smooth_%s",systematics_names[name].Data()));
            if(fitfunc[name] != "binwise"){
                sysAvg->Fit(sysFit, "QRMEX+", "", minPt, maxPt);
                for(int bin=1; bin<sysAvg->GetNbinsX()+1; bin++){
                    smooth->SetBinContent(bin,sysFit->Eval(sysAvg->GetXaxis()->GetBinCenter(bin)));
                }
            }
            for(int bin=1; bin<sysAvg->GetNbinsX()+1; bin++){
                smooth->SetBinError(bin,0);
            }
            smooth->SetMarkerStyle(stylesfilled[name+1]);
            smooth->SetLineColor(colors[name+1]);
            smooth->SetLineWidth(2);
            smooth->SetMarkerSize(2.5);
            smooth->SetMarkerColor(colors[name+1]);
        }

        sysAvg->Draw("p");
        if(fitfunc[name] != "") smooth->Draw("p,same");
        legend->Draw();
        if(fitfunc[name] != "") smooth->Write();
        else sysAvg->Write();

        drawLatexAdd(Form("%s Unfolding, Reg %i",typeCAP.Data(),regnumBayes),0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.87, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.95,0.83, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("Variation: %s",plot_names[name].Data()),0.95,0.79, 0.03,kFALSE, kFALSE, kTRUE);

        TString savestring2 = Form("%s/Systematics/R0%i/fits/%s_%s_R0%i_reg%i.%s",out.Data(),radius,systematics_names[name].Data(),type.Data(),radius,regnumBayes,fileType.Data());

        c->SaveAs(savestring2);
        vecSys.push_back(smooth);
        c->Clear();
        legend->Clear();

    }

    Double_t sysUpperRange;
    if(radius <= 4) sysUpperRange = 13;
    else sysUpperRange = 23;

    //.12,.63
    legend =  GetAndSetLegend2(0.5,0.57,.93,0.57-((nSys+2)*1.5*textSize)/2,textSize,2);
    TH1D *hTotal = (TH1D*)vecSys.at(0)->Clone("hTotal");
    hTotal->GetYaxis()->SetRangeUser(0,sysUpperRange);
    for(int j=1; j<hTotal->GetNbinsX()+1; j++){
        hTotal->SetBinContent(j,0);
    }
    for(int i=0; i<vecSys.size(); i++){
        vecSys.at(i)->GetYaxis()->SetRangeUser(0,sysUpperRange);
        if(fitfunc[i] == "binwise"){
            legend->AddEntry(vecSys.at(i), plot_names[i], "p");
            if(i==0) vecSys.at(i)->Draw("p");
            else vecSys.at(i)->Draw("p,same");
        }else{
            vecSys.at(i)->SetLineStyle(((i+1)%10));
            vecSys.at(i)->SetLineWidth(4);
            legend->AddEntry(vecSys.at(i), plot_names[i], "l");
            if(i==0) vecSys.at(i)->Draw("l");
            else vecSys.at(i)->Draw("l,same");
        }
        for(int j=1; j<hTotal->GetNbinsX()+1; j++){
            hTotal->SetBinContent(j,hTotal->GetBinContent(j) + pow(vecSys.at(i)->GetBinContent(j),2));
        }
    }
    for(int j=1; j<hTotal->GetNbinsX()+1; j++){
        hTotal->SetBinContent(j,sqrt(hTotal->GetBinContent(j)));
    }
    hTotal->SetMarkerStyle(stylesfilled[0]);
    hTotal->SetLineColor(colors[0]);
    hTotal->SetLineWidth(2);
    hTotal->SetMarkerSize(2.5);
    hTotal->SetMarkerColor(colors[0]);
    hTotal->Draw("p,same");
    legend->AddEntry(hTotal, "Total Sys. Unc.", "p");
    legend->Draw();
    hTotal->Write();

    drawLatexAdd(Form("%s Unfolding, Reg %i",typeCAP.Data(),regnumBayes),0.13,0.91, 0.03);
    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.13,0.87, 0.03);
    drawLatexAdd(Form("Full Jets, R = 0.%i",radius),0.13,0.83, 0.03);

    TString savestring3 = Form("%s/Systematics/TotalSystematics_R0%i.%s",out.Data(),radius,fileType.Data());
    //if (!fs::is_directory(savestring3) || !fs::exists(savestring3)) { // Check if src folder exists
    //    fs::create_directory(savestring3); // create src folder
    //}

    c->SaveAs(savestring3);

    fOutput->Close();
    delete fOutput;
}
