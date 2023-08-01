#!/bin/bash

file="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_default.root"
simfile="/media/austin/mightymouse/data/pp8TeV_MC/PureMC_8TeV/AnalysisResults.root"
dSysRootFiles="/home/austin/alice/SystematicsRootFiles/pPb"
#out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
out="/home/austin/alice/pPbTestPost"

type="Bayes"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotFinalSpectrum_pPb.cxx("'$file'","'$simfile'","'$dSysRootFiles'","'$type'","'$out'","'$filetype'")'
