#!/bin/bash

input8TeV="/media/austin/mightymouse/data/MCGen/POWHEG/8TeV/CT14withnegweight/Pythia8JetSpectra_merged.root"
output8TeV="/media/austin/mightymouse/data/MCGen/POWHEG/8TeV/CT14withnegweight/POWHEG_pp_8TeV.root"

root -x -q -l -b '/home/austin/alice/SpectrumPostProcessing/rebinPowheg.cxx("'$input8TeV'","'$output8TeV'",8.)'
