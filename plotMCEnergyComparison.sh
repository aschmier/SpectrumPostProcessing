#!/bin/bash

f8TeV="FinalResults_pp8TeV.root"
f13TeV="jetspectrum13TeV.root"
f13TeVPOWHEG="POWHEG_pp_13TeV.root"
outputdir=""
filetype="pdf"

mkdir -p $outputdir

root -x -q -l -b 'plotMCEnergyComparison.cxx("'$f8TeV'","'$f13TeV'","'$f13TeVPOWHEG'","'$outputdir'","'$filetype'")'