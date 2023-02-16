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

void getBinContentsResponse(TString file)
{
    int minradius = 2;
    int maxradius = 6;

    TFile *f = TFile::Open(file);
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
                if(content < 11){
                    cout << "Det level bin: " << lowX << " - " << upX << endl;
                    cout << "Part level bin: " << lowY << " - " << upY << endl;
                    cout << "Bin contents: " << response->GetBinContent(x,y) << endl << endl;
                }
            }
        }

    }
}
