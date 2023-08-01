#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include <cmath>
#include "TLatex.h"
#include "stdio.h"
#include "TPaveText.h"
#include "/home/austin/alice/RandomPrograms/paperPlotsHeader.h"
#include "fstream"
#include "/home/austin/alice/SubstructureAnalysis/unfolding/binnings/binningPt1D.C"
#include "/home/austin/alice/AliPhysics/PWG/EMCAL/EMCALtrigger/AliEmcalTriggerLuminosity.h"

void getTriggerLuminosity(TString trigger)
{
    TString filename = "/media/austin/mightymouse/data/pPb8TeV_data/merged/default/EJ1.root";
    TString dirname = "EmcalTriggerNormNormalizationpPb";
    cout << "Getting luminosity for " << trigger.Data() << endl;

    PWG::EMCAL::AliEmcalTriggerLuminosity *lumi = new PWG::EMCAL::AliEmcalTriggerLuminosity();
    lumi->InitFromFile(filename, dirname);
    lumi->Evaluate();
    cout << "//////////////////////////////////////" << endl;
    cout << lumi->GetLuminosityUncertaintyForTrigger("EJ1") << endl;
    //AliEmcalTriggerLuminosity::AliEmcalTriggerLuminosity(filename.Data(), dirname.Data());
    //cout << "Luminosity for " << trigger.Data() << " is " << AliEmcalTriggerLuminosity::GetLuminosity(trigger.Data()) << endl;
}
