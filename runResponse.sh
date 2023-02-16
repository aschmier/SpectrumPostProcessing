#!/bin/bash

mcfile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"
output="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures/Response"
filetype="pdf"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotResponse.cxx("'$mcfile'","'$output'","'$filetype'")'
