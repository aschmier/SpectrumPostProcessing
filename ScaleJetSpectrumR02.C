struct Point {
  double fX, fY, fEX, fEY;
};

void ScaleJetSpectrumR02(){
  std::array<Point, 10> points = {{
    {22.5, 6.64E-04, 2.5, 1.06E-04},
    {27.5, 2.81E-04, 2.5, 5.08E-05},
    {32.5, 1.47E-04, 2.5, 2.47E-05},
    {37.5, 8.47E-05, 2.5, 1.44E-05},
    {45, 3.27E-05, 5, 5.85E-06},
    {55, 1.20E-05, 5, 2.24E-06},
    {65, 6.15E-06, 5, 1.26E-06},
    {77.5, 2.23E-06, 7.5, 5.25E-07},
    {95 ,1.05E-06, 10., 2.83E-07},
    {115 ,2.77E-07, 10., 1.24E-07}
  }};

  TGraphErrors *spec276TeV = new TGraphErrors;
  int ip(0);
  for(auto p : points){
    spec276TeV->SetPoint(ip, 2*p.fX/2.76e3, p.fY* TMath::Power(2.76e3, 4.9));
    spec276TeV->SetPointError(ip, 2*p.fEX/2.76e3, p.fEY*TMath::Power(2.76e3, 4.9));
    ip++;
  }

  // Fit with power law
  TF1 *powerlaw = new TF1("powerlaw", "[0] * TMath::Power(x,[1])+[2]", 0.,  1.);
  powerlaw->SetParLimits(0, 60000, 120000);
  powerlaw->SetParLimits(1, -5.5, -4.5);
  powerlaw->FixParameter(2, 0);
  spec276TeV->Fit(powerlaw, "N", "", 3e-2, 1e-1);

  TCanvas *powerlawplot = new TCanvas("powerlawplot", "Power law fit");
  powerlawplot->cd();
  powerlawplot->SetLogy();
  powerlawplot->SetLogx();
  TH1 *axis = new TH1D("specaxis", ";x_{t}; #sqrt{s}^{4.9} d#sigma/dx_{t}", 100, 1e-3, 1.);
  axis->SetStats(false);
  axis->GetYaxis()->SetRangeUser(1e8, 1e15);
  axis->Draw("axis");
  spec276TeV->SetMarkerStyle(20);
  spec276TeV->SetMarkerColor(kBlack);
  spec276TeV->SetLineColor(kBlack);
  spec276TeV->Draw("epsame");
  powerlaw->SetLineColor(kRed);
  powerlaw->Draw("lsame");

  TPaveText *pave = new TPaveText(0.15, 0.15, 0.45, 0.22, "NDC");
  pave->SetBorderSize(0);
  pave->SetFillStyle(0);
  pave->SetTextFont(42);
  pave->AddText("pp #sqrt{s} = 2.76 TeV, Jets, R=0.2");
  pave->Draw();

  for(auto p  : points) {
    std::cout << "Scaled cross section 5 TeV: pt = " << p.fX << ", " << powerlaw->Eval(2*p.fX / 5.02e3)/TMath::Power(5.02e3, 4.9) << std::endl;
  }
}
