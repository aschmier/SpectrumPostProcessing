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
#include "/home/austin/alice/SubstructureAnalysis/helpers/math.C"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"

class Trials {
public:
    Trials(TH1 *hist) { fHistogram = hist; };
    ~Trials() { delete fHistogram; }

    double getMaxTrials() const {
        auto bins = getBins();
        return *std::max_element(bins.begin(), bins.end());
    }

    double getAverageTrials() const {
        auto bins = getBins();
        return TMath::Mean(bins.begin(), bins.end());
    }

    double getTrialsFit() const {
        TF1 model("meanntrials", "pol0", 0., 100.);
        fHistogram->Fit(&model, "N", "", fHistogram->GetXaxis()->GetBinLowEdge(2), fHistogram->GetXaxis()->GetBinUpEdge(fHistogram->GetXaxis()->GetNbins()+1));
        return model.GetParameter(0);
    }

private:
    std::vector<double> getBins() const {
        std::vector<double> result;
        for(int ib = 0; ib < fHistogram->GetXaxis()->GetNbins(); ib++) {
            auto entries  = fHistogram->GetBinContent(ib+1);
            if(TMath::Abs(entries) > DBL_EPSILON) {
                // filter 0
                result.emplace_back(entries);
            }
        }
        return result;
    }

    TH1 *fHistogram;
};

void getBinContentsResponse(TString unfoldedfile, TString rawfile)
{
    int minradius = 2;
    int maxradius = 2;

    auto binningpart = getJetPtBinningNonLinTrue8TeV(),
         binningdet  = getJetPtBinningNonLinSmear8TeV();

    TFile *f = TFile::Open(unfoldedfile.Data());
    if(!f || f->IsZombie()){
        cout << "File not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
        TDirectory *dRadius   = (TDirectory*)f->Get(Form("R0%i",radius));
        TDirectory *dResponse = (TDirectory*)dRadius->Get("response");
        TH2D *response  = (TH2D*)dResponse->Get(Form("Rawresponse_R0%i_fine_rebinned_standard",radius));

        for(int x = 1; x < response->GetNbinsX(); x++){
            for(int y = 1; y < response->GetNbinsY(); y++){
                int lowX = response->GetXaxis()->GetBinLowEdge(x);
                int lowY = response->GetYaxis()->GetBinLowEdge(y);
                int upX  = response->GetXaxis()->GetBinUpEdge(x);
                int upY  = response->GetYaxis()->GetBinUpEdge(y);
                double content = response->GetBinContent(x,y);
                double error   = response->GetBinError(x,y);
                if(content < 11){
                    cout << "Radius: 0." << radius << endl;
                    cout << "Det level bin: " << lowX << " - " << upX << endl;
                    cout << "Part level bin: " << lowY << " - " << upY << endl;
                    cout << "Bin contents: " << content << endl << endl;
                }
            }
        }

    }
/*
    TFile *fraw = TFile::Open(rawfile.Data());
    if(!fraw || fraw->IsZombie()){
        cout << "Raw file not found!" << endl;
        return;
    }

    for(int radius = minradius; radius <= maxradius; radius++){
      TString dirname = Form("EnergyScaleResults_FullJet_R0%i_INT7_default", radius);
      TString listname = Form("EnergyScaleHists_FullJet_R0%i_INT7_default", radius);

      TDirectory *dir = (TDirectory*)fraw->Get(dirname.Data());
      TList *list = (TList*)dir->Get(listname.Data());
      TH2D *response = (TH2D*)list->FindObject("hJetResponseFine");
      TH1D *histtrials = (TH1D*)list->FindObject("fHistTrials");


      auto trials = Trials(histtrials);
      auto mcscale = trials.getMaxTrials();
      response->SetName(Form("%s_fine", response->GetName()));
      response->Scale(mcscale);   // undo scaling with the number of trials (used to be 15e6)
      auto rebinnedresponse  = makeRebinned2D(response, binningdet, binningpart);
      rebinnedresponse->SetName(Form("%s_standard", rebinnedresponse->GetName()));

        for(int x = 1; x < rebinnedresponse->GetNbinsX(); x++){
            for(int y = 1; y < rebinnedresponse->GetNbinsY(); y++){
                int lowX = rebinnedresponse->GetXaxis()->GetBinLowEdge(x);
                int lowY = rebinnedresponse->GetYaxis()->GetBinLowEdge(y);
                int upX  = rebinnedresponse->GetXaxis()->GetBinUpEdge(x);
                int upY  = rebinnedresponse->GetYaxis()->GetBinUpEdge(y);
                double content = rebinnedresponse->GetBinContent(x,y);
                double error   = rebinnedresponse->GetBinError(x,y);
                //if(content < 11){
                    cout << "Radius: 0." << radius << endl;
                    cout << "Det level bin: " << lowX << " - " << upX << endl;
                    cout << "Part level bin: " << lowY << " - " << upY << endl;
                    cout << "Bin contents: " << content << endl << endl;
                //}
            }
        }

    }
    */
}
