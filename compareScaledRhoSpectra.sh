#!/bin/bash

scaledChargedRho="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/GA1187_ChargedRhoFullTest/AnalysisResults.root"
unscaledChargedRho="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/ChargedRho/AnalysisResults.root"
unscaledFullRho="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/default/AnalysisResults.root"

output="/home/austin/alice/SpectrumPostProcessing/ScaledRhoSpectraComparisons"
filetype="png"

[ -d $output ] || mkdir $output

root -l -x -b -q 'compareScaledRhoSpectra.cxx("'$scaledChargedRho'","'$unscaledChargedRho'","'$unscaledFullRho'","'$output'","'$filetype'","pPb")'