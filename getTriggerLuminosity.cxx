#include <cmath>
#include "stdio.h"
#include "fstream"

void getTriggerLuminosity()
{
    TString filename = "/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/default/AnalysisResults.root";
    TString dirname = "EmcalTriggerNormNormalizationpPb";
    PWG::EMCAL::AliEmcalTriggerLuminosity *lumi = new PWG::EMCAL::AliEmcalTriggerLuminosity();
    lumi->InitFromFile(filename, dirname);
    lumi->Evaluate();
    cout << "//////////////////////////////////////" << endl;
    cout << "EJ2 Luminosity Uncertainty: " << lumi->GetLuminosityUncertaintyForTrigger("EJ2") << endl;
    cout << "EJ1 Luminosity Uncertainty: " << lumi->GetLuminosityUncertaintyForTrigger("EJ1") << endl;
}
