#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"

struct Point {
  double fX, fY, fEX, fEY;
};

TGraphErrors *makeXtSpectrum(TGraphErrors *g, double energy, double nvar){
    energy *= 1000;
    TGraphErrors *spectrumXt = new TGraphErrors;
    int ip(0);
    for(int p = 0; p < g->GetN(); p++){
        spectrumXt->SetPoint(ip, 2*(g->GetPointX(p))/energy, (g->GetPointY(p))* TMath::Power(energy, nvar));
        spectrumXt->SetPointError(ip, 2*(g->GetErrorX(p))/energy, (g->GetErrorY(p))*TMath::Power(energy, nvar));
        ip++;
    }
    return spectrumXt;
}

void iterXTscaling(TString infile, TString outfile, const int minradius = 2, const int maxradius = 4){
    const int rdiff = maxradius - minradius + 1;

    TFile *reader = TFile::Open(infile, "READ");

    double textSize = 0.04;
    double alphasval = 0.25;
    double nvarmin = 2.7;
    double nvarmax = 5.0;
    double nvar = 3.0;

    bool hasSys = true;

    //TString enames[4] = {"2p76", "5", "8", "13"};
    int nenergies = 2;
    TString enames[2] = {"atlas7TeV", "cms13TeV"};
    //double energies[4] = {2.76, 5.02, 8, 13};
    double energies[2] = {7, 13};
    vector<TGraphErrors*> spectraSys[rdiff];
    vector<TGraphErrors*> spectraStat[rdiff];
    TGraphErrors *nVSchi2ndf = new TGraphErrors;

    int styles[17] = {8,21,33,34,41,43,45,47,48,49,3,2,5,22,23,29,39};
    Color_t colors[14] = {kBlack, kRed+2, kYellow+2, kGreen+2, kCyan+2, kBlue+2, kMagenta+2, kOrange+7, kSpring+8, kTeal+1, kAzure-4, kViolet+5, kPink-4};


    for(int radius = minradius; radius <= maxradius; radius++){
        for(double nvar = 2.7; nvar < 5.1; nvar+=0.1){
            // Make xT spectra
            TGraphErrors *spectrumCombined = new TGraphErrors;
            int cpoint(0);
            for(int eiter = 0; eiter < nenergies; eiter++){
                if(enames[eiter] == "2p76TeV" && radius == 3) continue;
                //TDirectory *dEnergy = (TDirectory*)reader->Get(Form("pp%sTeV", enames[eiter].Data()));
                TDirectory *dEnergy = (TDirectory*)reader->Get(Form("%s", enames[eiter].Data()));
                TDirectory *dRadius = (TDirectory*)dEnergy->Get(Form("R0%i", radius));
                TGraphErrors *spectrumSys = (TGraphErrors*)dRadius->Get("spectrumSys");
                TGraphErrors *spectrumStat = (TGraphErrors*)dRadius->Get("spectrumStat");

                if(!spectrumSys){ 
                    cout << "No systematics provided. Continuing with statistical errors only..." << endl;
                    hasSys = false;
                }

                TGraphErrors *xTSpectrumSys;
                if(hasSys) xTSpectrumSys = makeXtSpectrum(spectrumSys, energies[eiter], nvar);
                TGraphErrors *xTSpectrumStat = makeXtSpectrum(spectrumStat, energies[eiter], nvar);

                if(hasSys) spectraSys[radius-minradius].push_back(xTSpectrumSys);
                spectraStat[radius-minradius].push_back(xTSpectrumStat);

                // Combine xT Spectra
                for(int p = 0; p < xTSpectrumStat->GetN(); p++){
                    double x, y, ex, eyStat, eySys;
                    xTSpectrumStat->GetPoint(p, x, y);
                    ex = xTSpectrumStat->GetErrorX(p);
                    eyStat = xTSpectrumStat->GetErrorY(p);
                    if(hasSys) eySys  = xTSpectrumSys->GetErrorY(p);
                    spectrumCombined->SetPoint(cpoint, x, y);
                    if(hasSys) spectrumCombined->SetPointError(cpoint, ex, sqrt( pow(eyStat,2) + pow(eySys,2) ));
                    else spectrumCombined->SetPointError(cpoint, ex, eyStat);
                    cpoint++;
                }
            }

            // Fit with power law
            TF1 *powerlaw = new TF1("powerlaw", "[0] * TMath::Power(x,[1])+[2]", 2e-2,  5e-1);
            //powerlaw->SetParLimits(0, 1e-10, 1e20);
            if(nenergies == 4) powerlaw->SetParLimits(1, -5.5, -1);
            else powerlaw->SetParLimits(1, -10, -0.5);
            powerlaw->FixParameter(2, 0);
            if(nenergies == 4) spectrumCombined->Fit(powerlaw, "", "", 2e-2, 1e-1);
            else spectrumCombined->Fit(powerlaw, "", "", 5e-2, 5e-1);
            double chi2 = powerlaw->GetChisquare();
            double ndf = powerlaw->GetNDF();
            double chi2ndf = chi2/ndf;
            nVSchi2ndf->SetPoint(nVSchi2ndf->GetN(), nvar, chi2ndf);

            // Plotting
            TCanvas *c   = new TCanvas("c", "", 1200, 800);
            DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
            gStyle->SetOptStat(0);
            c->SetLogy();
            c->SetLogx();

            // Make dummy histo
            TH1D *h = new TH1D("h","",10000,2e-3,1);
            SetStyleHistoTH1ForGraphs(h,"","x_{T} (GeV/c)","#sqrt{#it{s}}^{#it{n}} d#sigma/dp_{T}d#eta (mb)",0.03,0.04,0.03,0.04,1,1.2);
            if(nenergies == 4){
                h->GetXaxis()->SetRangeUser(2e-3,2e-1);
                if(nvar < 3.5) h->GetYaxis()->SetRangeUser(7e2,1e12);
                else if(nvar >= 3.5 && nvar < 4.0) h->GetYaxis()->SetRangeUser(1e5,1e14);
                else if(nvar >= 4.0 && nvar < 4.5) h->GetYaxis()->SetRangeUser(1e7,1e16);
                else h->GetYaxis()->SetRangeUser(1e9,1e19);
            }else{
                h->GetXaxis()->SetRangeUser(2e-3,1);
                h->GetYaxis()->SetRangeUser(1e5,1e30);
            }

            TLegend *legend =  GetAndSetLegend2(0.12,0.14,0.5,0.12+(4)*textSize*1.5,textSize);

            h->Draw("axis");
        
            for(int eiter = 0; eiter < nenergies; eiter++){
                if(enames[eiter] == "2p76TeV" && radius == 3) continue;
                TGraphErrors *spectrumSys;
                if(hasSys) spectrumSys = (TGraphErrors*)spectraSys[radius-minradius][eiter]->Clone(Form("spectrumSys_%f_%i_%i", nvar, radius, eiter));
                TGraphErrors *spectrumStat = (TGraphErrors*)spectraStat[radius-minradius][eiter]->Clone(Form("spectrumStat_%f_%i_%i", nvar, radius, eiter));;

                if(hasSys){
                    spectrumSys->SetMarkerColor(colors[eiter]);
                    spectrumSys->SetLineColor(colors[eiter]);
                    spectrumSys->SetFillStyle(1);
                    spectrumSys->SetFillColorAlpha(colors[eiter],alphasval);
                    spectrumSys->Draw("e2,same");
                }
                spectrumStat->SetMarkerStyle(styles[eiter]);
                spectrumStat->SetMarkerColor(colors[eiter]);
                spectrumStat->SetLineColor(colors[eiter]);
                spectrumStat->SetLineWidth(3);
                spectrumStat->SetLineStyle(1);
                spectrumStat->SetMarkerSize(2);
                spectrumStat->Draw("p,e1,same");
                legend->AddEntry(spectrumStat, Form("%s TeV", enames[eiter].Data()), "p");
            }
            legend->Draw("same");

            drawLatexAdd("ALICE pp FullJets",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
            drawLatexAdd(Form("n = %1.1f", nvar),0.94,0.87, 0.03,kFALSE, kFALSE, kTRUE);

            //c->SaveAs(Form("%s/xT_spectra_n%i_R0%i.png", outfile.Data(), (int)(nvar*10), radius));
            c->Clear();

            // Plot combined
            h->Draw("axis");
            spectrumCombined->SetMarkerStyle(styles[radius-minradius]);
            spectrumCombined->SetMarkerColor(colors[radius-minradius]);
            spectrumCombined->SetLineColor(colors[radius-minradius]);
            spectrumCombined->SetLineWidth(3);
            spectrumCombined->SetLineStyle(1);
            spectrumCombined->SetMarkerSize(2);
            spectrumCombined->Draw("p,e1,same");

            drawLatexAdd("ALICE pp FullJets",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
            drawLatexAdd(Form("n = %1.1f", nvar),0.94,0.87, 0.03,kFALSE, kFALSE, kTRUE);
            drawLatexAdd(Form("#chi^{2}/NDF = %1.2f", chi2ndf),0.94,0.83, 0.03,kFALSE, kFALSE, kTRUE);

            c->SaveAs(Form("%s/xT_spectra_combined_n%i_R0%i.png", outfile.Data(), (int)(nvar*10), radius));

            spectraSys[radius-minradius].clear();
            spectraStat[radius-minradius].clear();

            delete c;
            delete h;
            delete legend;
        }

        // Fit chi2ndf vs n
        //TF1 *chi2fit = new TF1("chi2fit", "[0] * (TMath::Power(x,[1])+[2])+[3]", 2.2,  5.5);
        //chi2fit->FixParameter(0, 1);
        //chi2fit->FixParameter(1, 2);
        //chi2fit->FixParameter(2, 3.1);
        //chi2fit->FixParameter(3, 1.5);
        TF1 *chi2fit = new TF1("chi2fit", "[0]*(pow((x-[1]),2))+[2]", 2.8,  3.7);

        chi2fit->SetParLimits(0, 1, 100);
        chi2fit->SetParLimits(1, 3.0, 3.5);
        chi2fit->SetParLimits(2, 0, 10);
        nVSchi2ndf->Fit(chi2fit, "", "", 2.8, 3.7);

        // Plot chi2ndf vs n
        TCanvas *c = new TCanvas("c", "", 1200, 800);
        DrawPaperCanvasSettings(c,0.1,0.025,0.025,0.1);
        gStyle->SetOptStat(0);

        TH1D *h = new TH1D("h","",100,2.2,5.5);
        SetStyleHistoTH1ForGraphs(h,"","n","#chi^{2}/NDF",0.03,0.04,0.03,0.04,1,1.2);
        h->GetXaxis()->SetRangeUser(2.2,5.5);
        h->GetYaxis()->SetRangeUser(0,60);
        h->Draw("axis");

        nVSchi2ndf->SetMarkerStyle(styles[radius-minradius]);
        nVSchi2ndf->SetMarkerColor(colors[radius-minradius]);
        nVSchi2ndf->SetLineColor(colors[radius-minradius]);
        nVSchi2ndf->SetLineWidth(3);
        nVSchi2ndf->SetLineStyle(1);
        nVSchi2ndf->SetMarkerSize(2);
        nVSchi2ndf->Draw("p,e1,same");

        drawLatexAdd("ALICE pp FullJets",0.95,0.91, 0.03,kFALSE, kFALSE, kTRUE);
        drawLatexAdd(Form("R = 0.%i", radius),0.94,0.87, 0.03,kFALSE, kFALSE, kTRUE);

        c->SaveAs(Form("%s/chi2ndf_vs_n_R0%i.png", outfile.Data(), radius));
        cout << "min chi2ndf = " << chi2fit->GetParameter(1) << endl;

        delete c;
        delete h;

        nVSchi2ndf->Set(0);
    }
}
