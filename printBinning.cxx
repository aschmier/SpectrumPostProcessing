#include </home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C>
#include <iostream>

void printBinning(TString input){
    auto reader = std::unique_ptr<TFile>(TFile::Open(input.Data(), "READ"));
    if(!reader){ cout << "Input file not found!" << endl; return; }

    int minradius = 2;
    int maxradius = 5;

    for(int radius = minradius; radius <= maxradius; radius++){
        auto histlist = dynamic_cast<TDirectory*>(reader->Get(Form("R0%i", radius)));
        if(!histlist) std::cout << "R0" << radius << " not found" << std::endl;
        auto hJetPt = dynamic_cast<TGraphErrors*>(histlist->Get("spectrumStat_8TeV"));
        if(!hJetPt) std::cout << "spectrumStat_8TeV not found" << std::endl;
        std::cout << "R0" << radius << " binning: " << std::endl;
        for(int i = 0; i < hJetPt->GetN(); i++){
            double x, y;
            hJetPt->GetPoint(i, x, y);
            double ex = hJetPt->GetErrorX(i);
            double ey = hJetPt->GetErrorY(i);
            std::cout << "Bin " << i << std::endl;
            std::cout << "x: " << x << " +/- " << ex << std::endl;
            std::cout << "y: " << y << " +/- " << ey << std::endl;
        }
    }
    reader->Close();
    return; 
}