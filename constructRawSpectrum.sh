#!/bin/bash

datafile="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2682_ptscheme_chjets/EJE.root"

mcfile="/media/austin/mightymouse/data/pp8TeV_MC/ptscheme_trains/merged/default/AnalysisResults.root"
output="/home/austin/alice/SpectrumPostProcessing/ChargedJetsEMC"
filetype="png"
radius=2

[ -d $output ] || mkdir $output

root -l -x -b -q 'constructRawSpectrum.cxx("'$datafile'","'$mcfile'","'$output'","'$filetype'",'$radius')'