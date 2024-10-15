#include </home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C>
#include <iostream>

void rebinPythia(TString input, TString output, double collenergy, int minradius = 2, int maxradius = 6, TString outlier = "nooutlier"){
    auto reader = std::unique_ptr<TFile>(TFile::Open(input.Data(), "READ"));
    if(!reader){ cout << "Input file not found!" << endl; return; }
    auto writer = std::unique_ptr<TFile>(TFile::Open(output.Data(), "RECREATE"));

    std::vector<double> binning;
    if(collenergy == 8.)        binning = getJetPtBinningNonLinTrue8TeV_Prelim();
    else if(collenergy == 8.16) binning = getJetPtBinningNonLinTrue8TeVCourse();
    else if(collenergy == 13.)  binning = getJetPtBinningNonLinTruePoor();
    else cout << "Collision energy not supported" << endl;

    for(int radius = minradius; radius <= maxradius; radius++){
        writer->mkdir(Form("R0%i", radius));
        writer->cd(Form("R0%i", radius));
        std::stringstream dirname;
        dirname << "PartLevelJetResultsFullJetR0";
        dirname << radius << "_";
        dirname << outlier.Data();
        std::cout << "Reading " << dirname.str() << std::endl;
        auto histlist = dynamic_cast<TList *>(reader->Get(dirname.str().data()));
        if(!histlist) std::cout << dirname.str().data() << " not found" << std::endl;
        auto hJetPt = dynamic_cast<TH1D*>(histlist->FindObject("hJetPt"));
        hJetPt->Sumw2();
        auto rebinned = hJetPt->Rebin(binning.size()-1., Form("%s_R0%i_rebinned", hJetPt->GetName(), radius), binning.data());
        rebinned->SetDirectory(nullptr);
        rebinned->Scale(1., "width");
        double drad = (0.1)*((double)radius);
        rebinned->Scale(1./(1.4 - 2*drad));
        rebinned->SetName("spectrumStat");
        rebinned->Write();
    }
    writer->Close();
    reader->Close();
    return; 
}