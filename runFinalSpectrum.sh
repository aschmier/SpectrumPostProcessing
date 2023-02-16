#!/bin/bash

file="/home/austin/alice/unfoldingResults/output_default/bayes_default_final_2423.root"
simfile="/home/austin/alice/data/pp8TeV_MC/PureMC_8TeV/AnalysisResults.root"
dSysRootFiles="/home/austin/alice/SystematicsRootFiles"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
type="SVD"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotFinalSpectrum.cxx("'$file'","'$simfile'","'$dSysRootFiles'","'$type'","'$out'","'$filetype'")'
