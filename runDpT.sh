#!/bin/bash

#input="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/default/AnalysisResults.root"
input="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/ChargedRho/AnalysisResults.root"
output="/home/austin/alice/SpectrumPostProcessing/DpT/ChargedRhoAnalysis/charged"
datafile="/home/austin/alice/SpectrumPostProcessing/DpT/megan_rho_data.csv"

[ -d $output ] ||

mkdir -p $output

suffix="default"
minradius=2
maxradius=4
fileType="png"

root -x -q -l -b 'plotDpT.cxx("'$input'", "'$output'", "'$fileType'", "'$suffix'", '$minradius', '$maxradius',"'$datafile'")'