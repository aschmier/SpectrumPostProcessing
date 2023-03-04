#!/bin/bash

file="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"
simfile="/media/austin/mightymouse/data/pp8TeV_MC/PureMC_8TeV/AnalysisResults.root"
dSysRootFiles="/home/austin/alice/SystematicsRootFiles"
#out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
out="/home/austin/alice/SpectrumPostProcessing/testFinals"

type="Bayes"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotFinalSpectrum.cxx("'$file'","'$simfile'","'$dSysRootFiles'","'$type'","'$out'","'$filetype'")'
