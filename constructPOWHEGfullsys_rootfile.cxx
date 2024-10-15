#include <iostream>
#include <TFile.h>
#include <TH1.h>
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

void constructPOWHEGfullsys_rootfile(TString inputstat, TString inputsyst, TString output, int minradius = 2, int maxradius = 5, TString binning = "fine") {
    vector<double> trueRebin;
    if(binning == "fine")   trueRebin = getJetPtBinningNonLinTrue8TeV_Prelim();
    if(binning == "coarse") trueRebin = getJetPtBinningNonLinTrue8TeVCourse();

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
    if(binning == "fine") fout = TFile::Open(Form("%s", output.Data()), "RECREATE");
    if(binning == "coarse") fout = TFile::Open(Form("%s/POWHEGfullsysCoarse.root", output.Data()), "RECREATE");
    if (!fout) {
        std::cerr << "Error creating output file!" << std::endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        fout->mkdir(Form("R0%i", radius));
        fout->cd(Form("R0%i", radius));

        TH1D *hStat = (TH1D*)fstat->Get(Form("InclusiveJetXSection_R%i",radius));
        TH1D *hEvtStat = (TH1D*)fstat->Get("hNEvent");
        hStat->Scale(1./hEvtStat->GetBinContent(1));

        TH1D *hStatRebinned = (TH1D*)hStat->Rebin(trueRebin.size()-1, Form("hStat_R0%i",radius), trueRebin.data());
        double drad = (0.1)*((double)radius);
        hStatRebinned->Scale(1.,"width");
        hStatRebinned->Scale(1./(1.4 - 2*drad));
        hStatRebinned->SetName("spectrumStat");
        hStatRebinned->Write();

        vector<TH1D*> hMufMur;
        vector<TH1D*> hPDF;

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
                TDirectory* dir = static_cast<TDirectory*>(obj);
                TH1D *hSyst = (TH1D*)dir->Get(Form("InclusiveJetXSection_R0%i",radius));
                hSyst->Scale(1./hEvtSyst->GetBinContent(1));

                TH1D *hSystRebinned = (TH1D*)hSyst->Rebin(trueRebin.size()-1, Form("hSyst_R0%i",radius), trueRebin.data());
                double drad = (0.1)*((double)radius);
                hSystRebinned->Scale(1.,"width");
                hSystRebinned->Scale(1./(1.4 - 2*drad));
                dummy->Divide(hSystRebinned);

                if(((TString)dir->GetName()).Contains("muf")) hMufMur.push_back(dummy);
                else if(((TString)dir->GetName()).Contains("pdf")) hPDF.push_back(dummy);
                else cout << "Unknown systematic: " << dir->GetName() << endl;
            }
        }


        // Calculate maximum difference between muf and mur variations
        TH1D *hMufMurMax = (TH1D*)hStatRebinned->Clone(Form("hMufMur_R0%i",radius));
        for(int bin = 1; bin <= hMufMurMax->GetNbinsX(); bin++){
            double max = 0;
            for(int i = 0; i < hMufMur.size(); i++){
                if(std::abs(hMufMur[i]->GetBinContent(bin) - 1) > max) max = std::abs(hMufMur[i]->GetBinContent(bin) - 1);
            }
            hMufMurMax->SetBinError(bin, max*(hMufMurMax->GetBinContent(bin)));
        }
        hMufMurMax->SetName("spectrumSyst_MurMuf");
        hMufMurMax->Write();

        // Calculate PDF uncertainty as the RMS of all PDF variations
        TH1D *hPDFRMS = (TH1D*)hStatRebinned->Clone(Form("hPDF_R0%i",radius));
        for(int bin = 1; bin <= hPDFRMS->GetNbinsX(); bin++){
            double sum = 0;
            for(int i = 0; i < hPDF.size(); i++){
                sum += pow(hPDF[i]->GetBinContent(bin) - 1, 2);
            }
            hPDFRMS->SetBinError(bin, sqrt(sum)*hPDFRMS->GetBinContent(bin));
        }
        hPDFRMS->SetName("spectrumSyst_PDF");
        hPDFRMS->Write();

        // Calculate total systematic uncertainty
        TH1D *hSystTotal = (TH1D*)hStatRebinned->Clone(Form("hSystTotal_R0%i",radius));
        for(int bin = 1; bin <= hSystTotal->GetNbinsX(); bin++){
            double sum = 0;
            sum += pow(hMufMurMax->GetBinError(bin)/hMufMurMax->GetBinContent(bin), 2);
            sum += pow(hPDFRMS->GetBinError(bin)/hPDFRMS->GetBinContent(bin), 2);
            hSystTotal->SetBinError(bin, sqrt(sum)*hSystTotal->GetBinContent(bin));
        }
        hSystTotal->SetName("spectrumSyst");
        hSystTotal->Write();
    }

    // PDF uncertainty is the sum of all squares of the individual PDF uncertainties
    // Factorization/Renormalization scale uncertainty (muf/mur) is the max dirrerence of all variations to the default

    // Close the files
    fstat->Close();
    fsyst->Close();
    fout->Close();
}