#!/bin/bash

svdinput=/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/svd_default_final_2294.root
bayesinput=/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2294.root
output="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotUnfVsRaw.cxx("'$svdinput'","SVD","'$output'","'$filetype'")'
root -x -q -l -b 'plotUnfVsRaw.cxx("'$bayesinput'","Bayes","'$output'","'$filetype'")'
