#!/bin/bash

inputstatpp="/media/austin/mightymouse/data/MCGen/POWHEG/8TeV/CT14withnegweight/Pythia8JetSpectra_merged.root"
inputsystpp="/media/austin/mightymouse/data/MCGen/POWHEG/8TeV/POWHEGPYTHIA_8TeV_allsys.root"
outputpp="/home/austin/alice/JetsComparison/POWHEGPYTHIA_8TeV_fulljets_withsys.root"


root -x -q -l 'constructPOWHEGfullsys_rootfile.cxx("'$inputstatpp'","'$inputsystpp'","'$outputpp'", 2, 5, "fine")'