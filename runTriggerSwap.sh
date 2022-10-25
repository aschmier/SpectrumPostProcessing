#!/bin/bash

#file="/home/austin/alice/unfoldingResults/output_default/svd_default_final_2294.root"
file="/home/austin/alice/unfoldingResults/output_pPb/bayes_pPb_EPOSCorrected.root"
#out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
out="/home/austin/alice/unfoldingResults"
radius=2
system="pA"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotTriggerSwap.cxx("'$file'","'$out'",'$radius',"'$system'","'$filetype'")'
