#!/bin/bash

input13TeV="/media/austin/mightymouse/data/MCGen/PYTHIA/13TeV/ptlimited/pythia_pp13TeV.root"
input8TeV="/media/austin/mightymouse/data/MCGen/PYTHIA/8TeV/ptlimited/pythia_pp8TeV.root"
#input8p16TeV="/media/austin/mightymouse/data/MCGen/PYTHIA/8_16TeV/ptlimited/pythia_pp8160GeV.root"

output13TeV="/media/austin/mightymouse/data/MCGen/PYTHIA/13TeV/ptlimited/pythia_pp13TeV_rebinned.root"
output8TeV="/media/austin/mightymouse/data/MCGen/PYTHIA/8TeV/ptlimited/pythia_pp8TeV_rebinned.root"
#output8p16TeV="/media/austin/mightymouse/data/MCGen/PYTHIA/8_16TeV/ptlimited/pythia_pp8160GeV_rebinned.root"

root -x -q -l -b '/home/austin/alice/SpectrumPostProcessing/rebinPythia.cxx("'$input13TeV'","'$output13TeV'",13.)'
root -x -q -l -b '/home/austin/alice/SpectrumPostProcessing/rebinPythia.cxx("'$input8TeV'","'$output8TeV'",8.)'
#root -x -q -l -b '/home/austin/alice/SpectrumPostProcessing/rebinPythia.cxx("'$input8p16TeV'","'$output8p16TeV'",8.16)'
