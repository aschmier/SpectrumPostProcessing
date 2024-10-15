#!/bin/bash

inputstatpp="/media/austin/mightymouse/data/MCGen/POWHEG/8TeV/CT14withnegweight/Pythia8JetSpectra_merged.root"
inputsystpp="/media/austin/mightymouse/data/MCGen/POWHEG/8TeV/POWHEGPYTHIA_8TeV_allsys.root"
#outputpp="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures/POWHEG"
outputpp="/home/austin/alice/JetsComparison/POWHEGPYTHIA_8TeV_fulljets_withsys"

#inputstatppb="/media/austin/mightymouse/data/MCGen/POWHEG/816TeV/CT14withnegweights_boost/Pythia8JetSpectra_merged.root"
#inputsystppb="/media/austin/mightymouse/data/MCGen/POWHEG/816TeV/POWHEGPYTHIA_816TeV_withboost_allsys.root"
#outputppb="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures/pPbFigures/POWHEG"

root -x -q -l 'constructPOWHEGfullsys.cxx("'$inputstatpp'","'$inputsystpp'","'$outputpp'", 2, 5, "fine")'
#root -x -q -l 'constructPOWHEGfullsys.cxx("'$inputstatpp'","'$inputsystpp'","'$outputpp'", 2, 5, "coarse")'
#root -x -q -l 'constructPOWHEGfullsys.cxx("'$inputstatppb'","'$inputsystppb'","'$outputppb'", 2, 5, "coarse")'