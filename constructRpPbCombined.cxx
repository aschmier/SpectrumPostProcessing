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
#include "/home/austin/alice/SpectrumPostProcessing/ParseHEPData.cxx"

void constructRpPbCombined(TString ppRootFile, TString pPbRootFile, TString scaleFactorRootFile, TString sysRootFile, TString atlasRpA, TString cmsRpA, TString phnxRpA, TString aliceRpA02, TString aliceRpA03, TString aliceRpA04, TString output, TString fileType, int minradius = 2, int maxradius = 4)
{
    Double_t textSize     = 0.03;

    int scale[5] = {1,3,10,30,100};
    int regnumBayes = 6;
    int styles[11] = {4,25,27,28,35,36,38,40,42,44,46};
    double sizesempty[11] = {1.1,1.,1.5,1.2,1.5,1.5,1.5,1.5,1.5,1.5,1.5};
    int stylesfilled[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    double sizesfilled[17] = {1.6,1.5,2,1.7,2,2,2,2,2,2,2,2,2,2,2,2,2};

    Color_t colorALICEfull = kGreen+2;
    Color_t colorATLAS     = kOrange+1;
    Color_t colorCMS       = kBlack;
    Color_t colorPHENIX    = kMagenta+2;
    Color_t colorALICEchj  = kBlue+2;

    double alphasval = 0.25;
    double nucfac = 208.;

    Color_t colorpp  = kGreen+2;
    Color_t colorpPb = kOrange+1;

    vector<TGraphAsymmErrors*> vecALICEchjSTAT;
    vector<TGraphAsymmErrors*> vecALICEchjSYST;
    vector<TString> vecALICEchjnames = {aliceRpA02, aliceRpA03, aliceRpA04};

    // Open root files
    TFile *fpp = TFile::Open(ppRootFile);
    if(!fpp || fpp->IsZombie()){
        cout << "pp file not found!" << endl;
        return;
    }

    TFile *fpPb = TFile::Open(pPbRootFile);
    if(!fpPb || fpPb->IsZombie()){
        cout << "Default file not found!" << endl;
        return;
    }

    TFile *fscale = TFile::Open(scaleFactorRootFile);
    if(!fscale || fscale->IsZombie()){
        cout << "Scale factor file not found!" << endl;
        return;
    }

    // ATLAS
    TGraphAsymmErrors *ATLAS_stat = ParseHEPData(atlasRpA, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
    TGraphAsymmErrors *ATLAS_syst = ParseHEPData(atlasRpA, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);
    if(ATLAS_stat->GetN() != ATLAS_syst->GetN() || ATLAS_stat->GetN() == 0){
        cout << "ATLAS data not found!" << endl;
        return;
    }

    for(int point = 0; point < ATLAS_stat->GetN(); point++){
        double staterroratlas = ATLAS_stat->GetErrorY(point);
        ATLAS_stat->SetPointEYhigh(point,staterroratlas);
        ATLAS_stat->SetPointEYlow(point,staterroratlas);
        ATLAS_stat->SetPointEXhigh(point,0);
        ATLAS_stat->SetPointEXlow(point,0);
        double syserroratlas = ATLAS_syst->GetErrorY(point);
        ATLAS_syst->SetPointEYhigh(point,syserroratlas);
        ATLAS_syst->SetPointEYlow(point,syserroratlas);
    }

    ATLAS_stat->SetMarkerStyle(stylesfilled[1+maxradius-minradius]);
    ATLAS_stat->SetMarkerSize(sizesfilled[1+maxradius-minradius]);
    ATLAS_stat->SetMarkerColor(colorATLAS);
    ATLAS_stat->SetLineColor(colorATLAS);

    ATLAS_syst->SetFillColorAlpha(colorATLAS,alphasval);
    ATLAS_syst->SetLineColor(colorATLAS);
    ATLAS_syst->SetFillStyle(1);

    // CMS
    TGraphAsymmErrors *CMS_stat = ParseHEPData(cmsRpA, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
    TGraphAsymmErrors *CMS_syst = ParseHEPData(cmsRpA, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);
    if(CMS_stat->GetN() != CMS_syst->GetN() || CMS_stat->GetN() == 0){
        cout << "CMS data not found!" << endl;
        return;
    }

    for(int point = 0; point < CMS_stat->GetN(); point++){
        double staterrorcms = CMS_stat->GetErrorY(point);
        CMS_stat->SetPointEYhigh(point,staterrorcms);
        CMS_stat->SetPointEYlow(point,staterrorcms);
        CMS_stat->SetPointEXhigh(point,0);
        CMS_stat->SetPointEXlow(point,0);
        double syserrorcms = CMS_syst->GetErrorY(point);
        CMS_syst->SetPointEYhigh(point,syserrorcms);
        CMS_syst->SetPointEYlow(point,syserrorcms);
    }

    CMS_stat->SetMarkerStyle(stylesfilled[2+maxradius-minradius]);
    CMS_stat->SetMarkerSize(sizesfilled[2+maxradius-minradius]);
    CMS_stat->SetMarkerColor(colorCMS);
    CMS_stat->SetLineColor(colorCMS);

    CMS_syst->SetFillColorAlpha(colorCMS,alphasval);
    CMS_syst->SetLineColor(colorCMS);
    CMS_syst->SetFillStyle(1);

    // PHNX
    TGraphAsymmErrors *PHNX_stat = ParseHEPData(phnxRpA, 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
    TGraphAsymmErrors *PHNX_syst = ParseHEPData(phnxRpA, 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);
    if(PHNX_stat->GetN() != PHNX_syst->GetN() || PHNX_stat->GetN() == 0){
        cout << "PHNX data not found!" << endl;
        return;
    }

    for(int point = 0; point < PHNX_stat->GetN(); point++){
        double staterrorphnx = PHNX_stat->GetErrorY(point);
        PHNX_stat->SetPointEYhigh(point,staterrorphnx);
        PHNX_stat->SetPointEYlow(point,staterrorphnx);
        PHNX_stat->SetPointEXhigh(point,0);
        PHNX_stat->SetPointEXlow(point,0);
        double syserrorphnx = PHNX_syst->GetErrorY(point);
        PHNX_syst->SetPointEYhigh(point,syserrorphnx);
        PHNX_syst->SetPointEYlow(point,syserrorphnx);
    }

    PHNX_stat->SetMarkerStyle(stylesfilled[3+maxradius-minradius]);
    PHNX_stat->SetMarkerSize(sizesfilled[3+maxradius-minradius]);
    PHNX_stat->SetMarkerColor(colorPHENIX);
    PHNX_stat->SetLineColor(colorPHENIX);

    PHNX_syst->SetFillColorAlpha(colorPHENIX,alphasval);
    PHNX_syst->SetLineColor(colorPHENIX);
    PHNX_syst->SetFillStyle(1);

    // ALICE
    for(int r = 0; r < 3; r++){
        TGraphAsymmErrors *ALICE_stat = ParseHEPData(vecALICEchjnames.at(r), 8, 0, 1, 2, 3, 4, 5, kFALSE, kTRUE);
        TGraphAsymmErrors *ALICE_syst = ParseHEPData(vecALICEchjnames.at(r), 8, 0, 1, 2, 3, 6, 7, kFALSE, kTRUE);
        if(ALICE_stat->GetN() != ALICE_syst->GetN() || ALICE_stat->GetN() == 0){
            cout << "ALICE data not found!" << endl;
            return;
        }

        for(int point = 0; point < ALICE_stat->GetN(); point++){
            ALICE_stat->SetPointX(point,ALICE_stat->GetPointX(point)*1.5);
            ALICE_syst->SetPointX(point,ALICE_syst->GetPointX(point)*1.5);
        }

        for(int point = 0; point < ALICE_stat->GetN(); point++){
            double staterroralice = ALICE_stat->GetErrorY(point);
            ALICE_stat->SetPointEYhigh(point,staterroralice);
            ALICE_stat->SetPointEYlow(point,staterroralice);
            ALICE_stat->SetPointEXhigh(point,0);
            ALICE_stat->SetPointEXlow(point,0);
            double syserroralice = ALICE_syst->GetErrorY(point);
            ALICE_syst->SetPointEYhigh(point,syserroralice);
            ALICE_syst->SetPointEYlow(point,syserroralice);
        }

        ALICE_stat->SetMarkerStyle(stylesfilled[4+maxradius-minradius]);
        ALICE_stat->SetMarkerSize(sizesfilled[4+maxradius-minradius]);
        ALICE_stat->SetMarkerColor(colorALICEchj);
        ALICE_stat->SetLineColor(colorALICEchj);

        ALICE_syst->SetFillColorAlpha(colorALICEchj,alphasval);
        ALICE_syst->SetLineColor(colorALICEchj);
        ALICE_syst->SetFillStyle(1);

        vecALICEchjSTAT.push_back(ALICE_stat);
        vecALICEchjSYST.push_back(ALICE_syst);
    }


    // Set up canvases and legends for plotting
    TCanvas *cRatio   = new TCanvas("c2", "", 800, 800);
    DrawPaperCanvasSettings(cRatio,0.09,0.02,0.02,0.09);
    cRatio->SetLogx();

    gStyle->SetOptStat(0);

    TLegend *legendRatio;

    // Set up dummy histo for plotting
    TH1D *dummy = new TH1D("dummy","",1399,1,1400);
    dummy->GetXaxis()->SetRangeUser(10,1400);
    SetStyleHistoTH1ForGraphs(dummy,"","#it{p}_{T} (GeV/#it{c})","#it{R}_{pPb}",textSize,0.035,textSize,0.035,1.15,1.2);

    //TLegend *legendErrorKey =  GetAndSetLegend2(0.19,0.11,0.39,0.11+(2)*textSize,textSize);
    //legendErrorKey->AddEntry(vecSpectrumSyspp.at(0), "Systematic Uncertainty", "f");
    //legendErrorKey->AddEntry(vecSpectrumpp.at(0), "Statistical Uncertainty", "e");

    TLegend *legendErrorKeyRatio =  GetAndSetLegend2(0.13,0.11,0.34,0.11+(2)*textSize,textSize);
    legendErrorKeyRatio->AddEntry(CMS_syst, "Systematic Uncertainty", "f");
    legendErrorKeyRatio->AddEntry(CMS_stat, "Statistical Uncertainty", "e");

    ///////////////
    // Plot RpPb //
    ///////////////
    for(int radius = minradius; radius <= maxradius; radius++){
        dummy->GetYaxis()->SetRangeUser(0.45,1.45);
        if(radius == 2) legendRatio       = GetAndSetLegend2(0.13,0.18,0.33,0.18+(2)*textSize*1.2,textSize);
        if(radius == 3) legendRatio       = GetAndSetLegend2(0.13,0.18,0.33,0.18+(4)*textSize*1.2,textSize);
        if(radius == 4) legendRatio       = GetAndSetLegend2(0.13,0.18,0.33,0.18+(3)*textSize*1.2,textSize);

        TDirectory *scalefacdir = (TDirectory*)fscale->Get(Form("R0%i",radius));
        TH1D *scalefac = (TH1D*)scalefacdir->Get(Form("PythiaRatio_R0%i",radius));

        TDirectory *dRadOrigpp = (TDirectory*)fpp->Get(Form("R0%i",radius));
        TDirectory *dRegOrigpp    = (TDirectory*)dRadOrigpp->Get(Form("reg%i",regnumBayes));
        TH1D *normUnfoldedOrigpp = (TH1D*)dRegOrigpp->Get(Form("normalized_reg%i",regnumBayes));
        normUnfoldedOrigpp->Multiply(normUnfoldedOrigpp,scalefac);

        TDirectory *dRadOrigpPb = (TDirectory*)fpPb->Get(Form("R0%i",radius));
        TDirectory *dRegOrigpPb    = (TDirectory*)dRadOrigpPb->Get(Form("reg%i",regnumBayes));
        TH1D *normUnfoldedOrigpPb = (TH1D*)dRegOrigpPb->Get(Form("normalized_reg%i",regnumBayes));

        TString sysfile = Form("%s/systematics_R0%i.root", sysRootFile.Data(), radius);
        TFile *fSys = TFile::Open(sysfile);
        if(!fSys || fSys->IsZombie()){
            cout << "pp systematics file not found! --> R=0." << radius << endl;
            return;
        }
        TDirectory *dRadSys = (TDirectory*)fSys->Get(Form("R0%i",radius));
        TH1D *systematics = (TH1D*)dRadSys->Get("hTotal");
        systematics->SetName(Form("systematics_R0%i",radius));

        
        TH1D *RpA = (TH1D*)normUnfoldedOrigpPb->Clone("RpA");
        RpA->Divide(normUnfoldedOrigpPb,normUnfoldedOrigpp);
        RpA->Sumw2();
        RpA->Scale(1/nucfac);
        //RpA->Divide(scalefac);
        if(radius == 4) RpA->GetXaxis()->SetRangeUser(20,120);
        else RpA->GetXaxis()->SetRangeUser(20,240);
        TH1D *RpASys = (TH1D*)RpA->Clone(Form("RpASys_R0%i", radius));

        for(int bin = 1; bin <= RpASys->GetNbinsX(); bin++){
            double systrror          = systematics->GetBinContent(bin)/100;
            Double_t scaledError     = systrror*RpASys->GetBinContent(bin);
            RpASys->SetBinError(bin, scaledError);
        }

        legendRatio->AddEntry(RpA, "ALICE Full Jet, #sqrt{#it{s}_{NN}} = 8.16 TeV, |#eta|<0.7", "p");
        legendRatio->AddEntry(vecALICEchjSTAT.at(radius-minradius), "ALICE Charged Jet, #sqrt{#it{s}_{NN}} = 5.02 TeV, |#eta|<0.9, (1.5#times#it{p}_{T}^{jet})", "p");
        if(radius == 4) legendRatio->AddEntry(ATLAS_stat, "ATLAS Full Jet, #sqrt{#it{s}_{NN}} = 5.02 TeV, |#it{y}|<0.3", "p");
        if(radius == 3) legendRatio->AddEntry(CMS_stat, "CMS Full Jet, #sqrt{#it{s}_{NN}} = 5.02 TeV, |#eta|<0.5", "p");
        if(radius == 3) legendRatio->AddEntry(PHNX_stat, "PHENIX Full Jet, #sqrt{#it{s}_{NN}} = 200 GeV, |#eta|<0.3, #it{R}_{dAu}", "p");

        RpA->SetMarkerColor(colorALICEfull);
        RpA->SetLineColor(colorALICEfull);

        RpA->SetMarkerStyle(stylesfilled[radius-minradius]);
        RpA->SetMarkerSize(sizesfilled[radius-minradius]);
        RpA->SetLineWidth(3);
        RpA->SetLineStyle(1);

        RpASys->SetFillColorAlpha(colorALICEfull,alphasval);
        RpASys->SetLineColor(colorALICEfull);
        RpASys->SetFillStyle(1);

        dummy->Draw("axis");
        DrawGammaLines(10.,2000.,1.,1.,8.,16,9);
        RpA->Draw("p,e1,same");
        RpASys->Draw("e2,same");

        if(radius == 4){ 
            ATLAS_stat->Draw("p,e1,same");
            ATLAS_syst->Draw("e2,same");
        }

        if(radius == 3){ 
            CMS_stat->Draw("p,e1,same");
            CMS_syst->Draw("e2,same");

            PHNX_stat->Draw("p,e1,same");
            PHNX_syst->Draw("e2,same");
        }

        vecALICEchjSTAT.at(radius-minradius)->Draw("p,e1,same");
        vecALICEchjSYST.at(radius-minradius)->Draw("e2,same");

        legendRatio->Draw("same");
        legendErrorKeyRatio->Draw("same");

        //drawLatexAdd("ALICE Preliminary",0.14,0.93, textSize,kFALSE, kFALSE, false);
        drawLatexAdd(Form("Full Jets, Anti-#it{k}_{T}, #it{R} = 0.%i", radius),0.14,0.9, textSize,kFALSE, kFALSE, false);

        gSystem->Exec("mkdir -p " + output + "/RpPb/experimentComp");

        cRatio->SaveAs(Form("%s/RpPb/experimentComp/RpPbCombined_R0%i.%s",output.Data(),radius,fileType.Data()));

        legendRatio->Clear();
    }

}
