struct Point {
  double fX, fY, fEX, fEY;
};

void ScaleJetSpectrum(TString infile, TString outfile, double energyin, double energyout, int minradius, int maxradius, double nvar){
    cout << energyin << endl;

    energyin = energyin*1000;
    energyout = energyout*1000;

    TFile *reader = TFile::Open(infile, "READ");
    TFile *writer = TFile::Open(outfile, "RECREATE");
    int regnumBayes = 6;

    for(int radius = minradius; radius <= maxradius; radius++){
        writer->cd();
        writer->mkdir(Form("R0%i", radius));
        writer->cd(Form("R0%i", radius));

        TDirectory *dRad = (TDirectory*)reader->Get(Form("R0%i", radius));
        TDirectory *dReg    = (TDirectory*)dRad->Get(Form("reg%i",regnumBayes));
        TH1D *spectrumIn = (TH1D*)dReg->Get(Form("normalized_reg%i",regnumBayes));

        std::array<Point, 11> points;
        int ip(0);
        for(auto b : ROOT::TSeqI(1, spectrumIn->GetNbinsX()+1)){
            double pt = spectrumIn->GetXaxis()->GetBinCenter(b);
            if(pt < 22.5 || pt > 200) continue;
            double ex = (spectrumIn->GetBinWidth(b))/2;
            double y = spectrumIn->GetBinContent(b);
            double ey = spectrumIn->GetBinError(b);
            points[ip] = {pt, y, ex, ey};
            ip++;
        }

        TGraphErrors *spectrumInXt = new TGraphErrors;
        int ip2(0);
        for(auto p : points){
            spectrumInXt->SetPoint(ip2, 2*p.fX/energyin, p.fY* TMath::Power(energyin, nvar));
            spectrumInXt->SetPointError(ip2, 2*p.fEX/energyin, p.fEY*TMath::Power(energyin, nvar));
            ip2++;
        }

        // Fit with power law
        TF1 *powerlaw = new TF1("powerlaw", "[0] * TMath::Power(x,[1])+[2]", 0.,  1.);
        powerlaw->SetParLimits(0, 1, 1200000000);
        powerlaw->SetParLimits(1, -5.5, -1);
        powerlaw->FixParameter(2, 0);
        spectrumInXt->Fit(powerlaw, "", "", 0.005, 0.05);

        TGraphErrors *spectrumOut = new TGraphErrors;
        TH1D *hSpectrumOut = (TH1D*)spectrumIn->Clone("hSpectrumOut");
        int ip3(0);
        for(auto p : points){
            spectrumOut->SetPoint(ip3, p.fX, powerlaw->Eval(2*p.fX / energyout)/TMath::Power(energyout, nvar));
            spectrumOut->SetPointError(ip3, p.fEX, powerlaw->GetParError(0)*TMath::Power(2*p.fX / energyout, powerlaw->GetParameter(1))/TMath::Power(energyout, nvar));
            if(p.fX < 22.5 || p.fX > 200) hSpectrumOut->SetBinContent(hSpectrumOut->GetBin(p.fX), 0);
            hSpectrumOut->SetBinContent(hSpectrumOut->GetBin(p.fX), powerlaw->Eval(2*p.fX / energyout)/TMath::Power(energyout, nvar));
            hSpectrumOut->SetBinError(hSpectrumOut->GetBin(p.fX), powerlaw->GetParError(0)*TMath::Power(2*p.fX / energyout, powerlaw->GetParameter(1))/TMath::Power(energyout, nvar));
            ip3++;
        }
        spectrumOut->Write("scaledSpectrumOut");
        spectrumInXt->Write("scaledSpectrumInXt");
        hSpectrumOut->Write("scaledSpectrumOutHist");
    }
    writer->Close();
    reader->Close();
}
