#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

TH1D *ProcessSimHisto(TH1D *spectrum, vector<double> vecBins, int r, TString outlier, int style, Color_t color, double size){
    TH1D *hRebinned = (TH1D*)spectrum->Rebin(vecBins.size()-1, Form("hSim_R0%i_%s",r,outlier.Data()), vecBins.data());
    double radius = (0.1)*((double)r);
    hRebinned->Scale(1.,"width");
    hRebinned->Scale(1./(1.4 - 2*radius));
    hRebinned->SetMarkerStyle(style);
    hRebinned->SetMarkerSize(2.5);
    hRebinned->SetMarkerColor(color);
    hRebinned->SetLineColor(color);
    hRebinned->SetMarkerSize(size);
    return hRebinned;
}

void constructPOWHEGfullsys(TString inputstat, TString inputsyst, TString output, int minradius = 2, int maxradius = 5, TString binning = "fine") {
    Double_t textSize     = 0.03;
    double alphasval = 0.25;
    int stylesempty[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};
    
    vector<double> trueRebin;
    if(binning == "fine")   trueRebin = getJetPtBinningNonLinTrue8TeV();
    if(binning == "coarse") trueRebin = getJetPtBinningNonLinTrue8TeVCourse();

    //for(int i = (trueRebin.size()-1); i >= 0; i--){
    //    if(trueRebin[i] > 350){
    //        trueRebin.pop_back();
    //    }
    //}


    gSystem->Exec("mkdir -p " + output);
    TFile* fstat = TFile::Open(inputstat, "READ");
    if (!fstat) {
        std::cout << "Error opening default input file!" << std::endl;
        return;
    }
    TFile* fsyst = TFile::Open(inputsyst, "READ");
    if (!fsyst) {
        std::cout << "Error opening systematics input file!" << std::endl;
        return;
    }
    TFile* fout;
    if(binning == "fine") fout = TFile::Open(Form("%s/POWHEGfullsys.root", output.Data()), "RECREATE");
    if(binning == "coarse") fout = TFile::Open(Form("%s/POWHEGfullsysCoarse.root", output.Data()), "RECREATE");
    if (!fout) {
        std::cerr << "Error creating output file!" << std::endl;
        return;
    }

    cout << "Writing uncertainties to file: " << fout->GetName() << endl;

    // Set up canvases and legends for plotting
    TCanvas *cSpectrum   = new TCanvas("c", "", 800, 1000);
    DrawPaperCanvasSettings(cSpectrum,0.15,0.025,0.025,0.08);
    cSpectrum->SetLogy();
    cSpectrum->SetLogx();

    gStyle->SetOptStat(0);

    // Set up dummy histos for plotting
    TH1D *dummySpectrumLogX = new TH1D("dummySpectrumLogX","",300,0,300);
    dummySpectrumLogX->GetXaxis()->SetRangeUser(18,260);
    SetStyleHistoTH1ForGraphs(dummySpectrumLogX,"","#it{p}_{T} (GeV/#it{c})","#frac{d^{2}#it{#sigma}}{d#it{p}_{T}d#it{#eta}} (mb (GeV/#it{c})^{-1})",textSize,0.035,textSize,0.035,1,1.9);
    dummySpectrumLogX->GetXaxis()->SetMoreLogLabels(true);

    // Set Up legends
    TLegend *legendErrorKey;
    TLegend *legendSpectrum;
    TLegend *legendSimNewer =  GetAndSetLegend2(0.19,0.2,0.39,0.2+2*textSize,textSize);

    for(int radius = minradius; radius <= maxradius; radius++){
        dummySpectrumLogX->Draw("axis");


        TH1D *hStat = (TH1D*)fstat->Get(Form("InclusiveJetXSection_R%i",radius));
        TH1D *hEvtStat = (TH1D*)fstat->Get("hNEvent");
        hStat->Scale(1./hEvtStat->GetBinContent(1));
        TH1D *hStatRebinned = (TH1D*)ProcessSimHisto( hStat, trueRebin, radius, "na", stylesempty[radius-minradius+2], colors[radius-minradius+2], sizesempty[radius-minradius+2] );
        hStatRebinned->SetName(Form("hStat_R0%i",radius));
        hStatRebinned->Write();
        cout << "Stat uncertainty for radius " << radius << " saved." << endl;
        hStatRebinned->Draw("p,e1,same");

        // Take ratio of radii to R = 0.2
        TH1D *hStatRebinnedRatio;
        if(radius != 2){
            TH1D *hStatRebinnedR02 = (TH1D*)fout->Get("hStat_R02");
            hStatRebinnedRatio = (TH1D*)hStatRebinnedR02->Clone(Form("hStat_Ratio_R0%i",radius));
            hStatRebinnedRatio->Divide(hStatRebinned);
            hStatRebinnedRatio->Write();
            cout << "Stat uncertainty for ratio of radii R = 0.2 to " << radius << " saved." << endl;
        }

        vector<TH1D*> hMufMur;
        vector<TH1D*> hPDF;
        vector<TH1D*> hMufMurRatio;
        vector<TH1D*> hPDFRatio;
        double sysPDF = 0;
        TH1D *hEvtSyst = (TH1D*)fsyst->Get("hNEvent");
        if(!hEvtSyst){
            std::cout << "Error: hNEvent not found in syst file!" << std::endl;
            return;
        }
        TIter next(fsyst->GetListOfKeys());
        TKey* key;
        while ((key = static_cast<TKey*>(next()))) {
            TObject* obj = key->ReadObj();
            if (obj->IsA()->InheritsFrom("TDirectoryFile")) {
                TH1D *dummy = (TH1D*)hStatRebinned->Clone(Form("dummy_R0%i_%s",radius, obj->GetName()));
                TH1D *dummyRatio;
                if(radius != 2) dummyRatio = (TH1D*)hStatRebinnedRatio->Clone(Form("dummy_Ratio_R0%i_%s",radius, obj->GetName()));
                TDirectory* dir = static_cast<TDirectory*>(obj);
                TH1D *hSyst = (TH1D*)dir->Get(Form("InclusiveJetXSection_R0%i",radius));
                hSyst->Scale(1./hEvtSyst->GetBinContent(1));
                TH1D *hSystRebinned = (TH1D*)ProcessSimHisto( hSyst, trueRebin, radius, "na", stylesfilled[radius-minradius+2], colors[radius-minradius+2], sizesfilled[radius-minradius+2] );
                hSystRebinned->SetName(Form("%s_R0%i", dir->GetName(),radius));
                if(((TString)dir->GetName()).Contains("muf")){
                    hSystRebinned->Draw("p,e2,same");
                    dummy->Divide(hSystRebinned);
                    hMufMur.push_back(dummy);
                    if(radius != 2){
                        TH1D *MufMurR02 = (TH1D*)fout->Get(Form("%s_R02", dir->GetName()));
                        TH1D *MufMurRatio = (TH1D*)MufMurR02->Clone(Form("%s_Ratio_R0%i", dir->GetName(),radius));
                        MufMurRatio->Divide(hSystRebinned);
                        dummyRatio->Divide(MufMurRatio);
                        if(!dummyRatio) cout << "Error: " << Form("%s_Ratio_R0%i", dir->GetName(),radius) << " not found!" << endl;
                        hMufMurRatio.push_back(dummyRatio);
                    }
                }else if(((TString)dir->GetName()).Contains("pdf")){
                    hSystRebinned->Draw("p,e2,same");
                    dummy->Divide(hSystRebinned);
                    hPDF.push_back(dummy);
                    if(radius != 2){
                        TH1D *PDFR02 = (TH1D*)fout->Get(Form("%s_R02", dir->GetName()));
                        TH1D *PDFRatio = (TH1D*)PDFR02->Clone(Form("%s_Ratio_R0%i", dir->GetName(),radius));
                        PDFRatio->Divide(hSystRebinned);
                        dummyRatio->Divide(PDFRatio);
                        if(!dummyRatio) cout << "Error: " << Form("%s_Ratio_R0%i", dir->GetName(),radius) << " not found!" << endl;
                        hPDFRatio.push_back(dummyRatio);
                    }
                }
            }
        }

        cSpectrum->SaveAs(Form("%s/POWHEGfullsys_R0%i.pdf", output.Data(), radius));

        // Calculate maximum difference between muf and mur variations
        TH1D *hMufMurMax = (TH1D*)hStatRebinned->Clone(Form("hMufMur_R0%i",radius));
        for(int bin = 1; bin <= hMufMurMax->GetNbinsX(); bin++){
            double max = 0;
            for(int i = 0; i < hMufMur.size(); i++){
                if(std::abs(hMufMur[i]->GetBinContent(bin) - 1) > max) max = std::abs(hMufMur[i]->GetBinContent(bin) - 1);
            }
            hMufMurMax->SetBinError(bin, max*(hMufMurMax->GetBinContent(bin)));
        }
        hMufMurMax->Write();
        cout << "muf/mur uncertainty for radius " << radius << " saved." << endl;

        // Calculate PDF uncertainty as the RMS of all PDF variations
        TH1D *hPDFRMS = (TH1D*)hStatRebinned->Clone(Form("hPDF_R0%i",radius));
        for(int bin = 1; bin <= hPDFRMS->GetNbinsX(); bin++){
            double sum = 0;
            for(int i = 0; i < hPDF.size(); i++){
                sum += pow(hPDF[i]->GetBinContent(bin) - 1, 2);
            }
            hPDFRMS->SetBinError(bin, sqrt(sum)*hPDFRMS->GetBinContent(bin));
        }
        hPDFRMS->Write();
        cout << "PDF uncertainty for radius " << radius << " saved." << endl;

        // Now do the same for the ratios
        if(radius == 2) continue;

        TH1D *hMufMurMaxRatio = (TH1D*)hStatRebinnedRatio->Clone(Form("hMufMur_Ratio_R0%i",radius));
        for(int bin = 1; bin <= hMufMurMaxRatio->GetNbinsX(); bin++){
            double max = 0;
            for(int i = 0; i < hMufMurRatio.size(); i++){
                if(std::abs(hMufMurRatio[i]->GetBinContent(bin) - 1) > max) max = std::abs(hMufMurRatio[i]->GetBinContent(bin) - 1);
            }
            hMufMurMaxRatio->SetBinError(bin, max*(hMufMurMaxRatio->GetBinContent(bin)));
        }
        hMufMurMaxRatio->GetYaxis()->SetRangeUser(0, 1);
        hMufMurMaxRatio->Write();
        cout << "muf/mur uncertainty for ratio of radii R = 0.2 to " << radius << " saved." << endl;

        TH1D *hPDFRMSRatio = (TH1D*)hStatRebinnedRatio->Clone(Form("hPDF_Ratio_R0%i",radius));
        for(int bin = 1; bin <= hPDFRMSRatio->GetNbinsX(); bin++){
            double sum = 0;
            for(int i = 0; i < hPDFRatio.size(); i++){
                sum += pow(hPDFRatio[i]->GetBinContent(bin) - 1, 2);
            }
            hPDFRMSRatio->SetBinError(bin, sqrt(sum)*hPDFRMSRatio->GetBinContent(bin));
        }
        hPDFRMSRatio->GetYaxis()->SetRangeUser(0, 1);
        hPDFRMSRatio->Write();
        cout << "PDF uncertainty for ratio of radii R = 0.2 to " << radius << " saved." << endl;
    }

    // PDF uncertainty is the sum of all squares of the individual PDF uncertainties
    // Factorization/Renormalization scale uncertainty (muf/mur) is the max dirrerence of all variations to the default

    // Close the files
    fstat->Close();
    fsyst->Close();
    fout->Close();
}