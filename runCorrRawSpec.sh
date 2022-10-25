#!/bin/bash

file="/home/austin/alice/unfoldingResults/output_default/bayes_default_final_2294.root"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotCorrRawSpec.cxx("'$file'","'$out'","'$filetype'")'
