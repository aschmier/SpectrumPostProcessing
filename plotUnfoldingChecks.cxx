// Macro for comparing rejection factors in pp 2012 jet data

#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TH2D.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"

void plot(TString output, int r, TString type, int reg, TCanvas *c, TH1D* hist, TString xlabel = "", TString ylabel = "", TString savetitle = "out", bool logy = false, bool logz = false, bool logx = false){
  c->SetLogy(logy);
  c->SetLogz(logz);
  c->SetLogx(logx);

  SetStyleHistoTH1ForGraphs(hist,"",xlabel.Data(),ylabel.Data(),0.03,0.04,0.03,0.04,1,1.2);
  hist->GetXaxis()->SetRangeUser(20,320);
  hist->SetMarkerColor(1);
  hist->SetMarkerStyle(8);
  hist->SetMarkerSize(2);
  hist->Draw("p");

  drawLatexAdd(Form("%s Unfolding, Reg %i",type.Data(),reg),0.95,0.87, 0.03,false, false, true);
  drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.83, 0.03,false, false, true);
  drawLatexAdd(Form("Full Jets, R = 0.%i",r),0.95,0.79, 0.03,false, false, true);
  c->SaveAs(Form("%s/%s_%s_R0%i_reg%i.png",output.Data(),savetitle.Data(),type.Data(),r,reg));
}

void plotRF(TString output, int r, TString type, int reg, TH1D* hist, double rf, double rferror, TString trigger, TString fileType, TString xlabel = "", TString ylabel = "", TString savetitle = "out", bool logy = false, bool logz = false, bool logx = false){
    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    double leftMargin         = 0.075;
    double rightMargin        = 0.02;
    double topMargin          = 0.04;
    double bottomMargin       = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);

  SetStyleHistoTH1ForGraphs(hist,"",xlabel.Data(),ylabel.Data(),0.03,0.04,0.03,0.04,1,0.7);
  hist->GetXaxis()->SetRangeUser(20,320);
  hist->SetMarkerColor(1);
  hist->SetMarkerStyle(8);
  hist->SetMarkerSize(2);
  hist->SetLineColor(1);
  //hist->GetXaxis()->SetMoreLogLabels();
  canvas->SetLogx();
  if(trigger=="EMC7/INT7"){
      hist->GetXaxis()->SetRangeUser(1,40);
      hist->GetYaxis()->SetRangeUser(35,80);
  }else{
      hist->GetXaxis()->SetRangeUser(3,200);
      hist->GetYaxis()->SetRangeUser(5,110);
  }
  hist->Draw("p");

  drawLatexAdd(Form("pp #it{#sqrt{s_{NN}}} = 8 TeV; Full Jets, R = 0.%i",r),0.4,0.38, 0.04,false, false, false);
  drawLatexAdd(Form("%s Unfolding, Reg %i",type.Data(),reg),0.4,0.34, 0.04,false, false, false);
  drawLatexAdd(Form("%s RF = %1.2f #pm %1.2f",trigger.Data(),rf,rferror),0.4,0.3, 0.04,false, false, false);
  canvas->SaveAs(Form("%s/%s_%s_R0%i_reg%i.%s",output.Data(),savetitle.Data(),type.Data(),r,reg,fileType.Data()));
}

void plotTriggerEfficiency(TString output, int radius, TString jetType, TString fileType, TH1D *tempSpectrumEMC7, TH1D *tempSpectrumEJE){
    TCanvas* canvas             = new TCanvas("canvas","",10,10,750,500);  // gives the page size
    double leftMargin         = 0.075;
    double rightMargin        = 0.02;
    double topMargin          = 0.04;
    double bottomMargin       = 0.09;
    DrawPaperCanvasSettings(canvas,leftMargin,rightMargin,topMargin,bottomMargin);
    gStyle->SetOptStat(0);
    canvas->SetLogx();
    tempSpectrumEMC7->GetXaxis()->SetMoreLogLabels();
    tempSpectrumEMC7->GetXaxis()->SetRangeUser(25,900);
    tempSpectrumEJE->GetXaxis()->SetMoreLogLabels();
    SetStyleHistoTH1ForGraphs(tempSpectrumEMC7,"","p_{T}^{jet} [GeV/c]","EMC7 Trigger Efficiency",0.03,0.04,0.03,0.04,1,0.9);
    SetStyleHistoTH1ForGraphs(tempSpectrumEJE,"","p_{T}^{jet} [GeV/c]","EJE Trigger Efficiency",0.03,0.04,0.03,0.04,1,0.9);
    tempSpectrumEMC7->Draw("p,e");
    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.12,0.88, 0.03, false, false, false);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.12,0.84, 0.03, false, false, false);
    canvas->SaveAs(Form("%s/%s.%s",output.Data(),tempSpectrumEMC7->GetName(),fileType.Data()));
    tempSpectrumEJE->Draw("p,e");
    drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.12,0.88, 0.03, false, false, false);
    drawLatexAdd(Form("%s Jets, R=0.%i", jetType.Data(), radius),0.12,0.84, 0.03, false, false, false);
    canvas->SaveAs(Form("%s/TriggerEfficiency/%s.%s",output.Data(),tempSpectrumEJE->GetName(),fileType.Data()));
}

void drawResponse(int r, TString type, TCanvas *c, TH2D* hist, TString xlabel = "", TString ylabel = "", TString savetitle = "out", bool logy = false, bool logz = false, bool logx = false){
  c->SetLogy(logy);
  c->SetLogz(logz);
  c->SetLogx(logx);

  SetStyleHistoTH2ForGraphs(hist,xlabel.Data(),ylabel.Data(),0.03,0.04,0.03,0.04,1,1.2);
  hist->GetXaxis()->SetRangeUser(0,180);
  hist->GetYaxis()->SetRangeUser(0,300);
  hist->Draw("colz");

  drawLatexAdd("pp #it{#sqrt{s_{NN}}} = 8 TeV",0.95,0.87, 0.03,false, false, true);
  drawLatexAdd(Form("Full Jets, R = 0.%i",r),0.95,0.83, 0.03,false, false, true);
  c->SaveAs(Form("/home/austin/alice/unfoldingResults/unfoldingChecks/%s_%s.png",savetitle.Data(),type.Data()));
}

void plotUnfoldingChecks(TString resultsFile, TString output, TString fileType, TString unfoldingtype, int radius, int regnum)
{
    // Define variables
    double textSize     = 0.03;
    TString jetType       = "Full";

    // Open unfolding results file
    TFile *f = TFile::Open(resultsFile.Data());
    if(!f || f->IsZombie()) return;

    // Get the histograms...
    TDirectory *rDir     = (TDirectory*)f->Get(Form("R0%i",radius));

    // From the rawlevel directory
    TDirectory *rawlevel = (TDirectory*)rDir->Get("rawlevel");
    TH1D *trigEffEMC7    = (TH1D*)rawlevel->Get(Form("TriggerEfficiency_EMC7_R0%i_rebinned",radius));
    TH1D *trigEffEJE     = (TH1D*)rawlevel->Get(Form("TriggerEfficiency_EJE_R0%i_rebinned",radius));
    TH1D *regFacEMC7     = (TH1D*)rawlevel->Get(Form("RejectionFactor_EMC7_INT7_R0%i",radius));
    TH1D *regFacEJE      = (TH1D*)rawlevel->Get(Form("RejectionFactor_EJE_EMC7_R0%i",radius));
    TF1 *erfuncEMC7      = (TF1*)regFacEMC7->GetFunction("erfunc");
    TF1 *erfuncEJE       = (TF1*)regFacEJE->GetFunction("erfunc");
    TH1D *JFPurity       = (TH1D*)rawlevel->Get("puretag");
    TH1D *hraw           = (TH1D*)rawlevel->Get(Form("hraw_R0%i",radius));
    TH1D *hrawFine       = (TH1D*)rawlevel->Get(Form("hraw_fine_R0%i",radius));

    // From the response directory
    TDirectory *response = (TDirectory*)rDir->Get("response");
    TH2D *Rawresponse    = (TH2D*)response->Get(Form("Rawresponse_R0%i_fine_rebinned_standard",radius));
    TH1D *JFEff          = (TH1D*)response->Get(Form("hJetfindingEfficiency_R0%ihJetfindingPurity_R0%i",radius,radius));

    // From the closuretest directory
    //TDirectory *closure  = (TDirectory*)rDir->Get("closuretest");

    // From the regularization directory
    TDirectory *reg      = (TDirectory*)rDir->Get(Form("reg%i",regnum));
    TH1D *normUnfolded   = (TH1D*)reg->Get(Form("normalized_reg%i",regnum));

    hraw->Scale(1.,"width");

    // Set up canvas for plotting
    TCanvas *c1   = new TCanvas("c1", "", 1200, 800);
    DrawPaperCanvasSettings(c1,0.1,0.025,0.025,0.1);
    gStyle->SetOptStat(0);

    double rfEMC7      = erfuncEMC7->GetParameter(2)+erfuncEMC7->GetParameter(3);
    double rfErrorEMC7 = (erfuncEMC7->GetParameter(2)+erfuncEMC7->GetParameter(3))*TMath::Sqrt(TMath::Power(erfuncEMC7->GetParError(2)/erfuncEMC7->GetParameter(2),2)+TMath::Power(erfuncEMC7->GetParError(3)/erfuncEMC7->GetParameter(3),2));
    double rfEJE       = erfuncEJE->GetParameter(2)+erfuncEJE->GetParameter(3);
    double rfErrorEJE  = (erfuncEJE->GetParameter(2)+erfuncEJE->GetParameter(3))*TMath::Sqrt(TMath::Power(erfuncEJE->GetParError(2)/erfuncEJE->GetParameter(2),2)+TMath::Power(erfuncEJE->GetParError(3)/erfuncEJE->GetParameter(3),2));


    plotTriggerEfficiency(output, radius, jetType, fileType, trigEffEMC7, trigEffEJE);
    plotRF(output, radius, unfoldingtype, regnum, regFacEMC7, rfEMC7, rfErrorEMC7, "EMC7/INT7", fileType, "E^{cluster}", "RF (EMC7/INT7)", "RejectionFactorEMC7");
    plotRF(output, radius, unfoldingtype, regnum, regFacEJE, rfEJE, rfErrorEJE, "EJE/EMC7", fileType, "E^{cluster}", "RF (EJE/EMC7)", "RejectionFactorEJE");
/*    plot(radius, unfoldingtype, regnum, c1, JFPurity, "p_{T}^{jet}", "Jet Finding Purity", "Purity");
    plot(radius, unfoldingtype, regnum, c1, hraw, "p_{T}^{jet}", "1/N^{trig} dN/dp_{T}^{jet}", "RawSpectrumRebinned", true);
    plot(radius, unfoldingtype, regnum, c1, hrawFine, "p_{T}^{jet}", "1/N^{trig} dN/dp_{T}^{jet}", "RawSpectrumFineNoPureCorr", true);
    drawResponse(radius, unfoldingtype, c1, Rawresponse, "p_{T}^{det}", "p_{T}^{part}", "Response", false, true);
    plot(radius, unfoldingtype, regnum, c1, JFEff, "p_{T}^{jet}", "Jet Finding Efficiency", "Efficiency");
    plot(radius, unfoldingtype, regnum, c1, normUnfolded, "p_{T}^{jet}", "1/N^{trig} dN/dp_{T}^{jet}", "UnfSpectrumNorm", true);
*/}
