#!/bin/bash

#file="/home/austin/alice/unfoldingResults/output_default/svd_default_final_2294.root"
file="/home/austin/alice/SpectrumPostProcessing/bayes_pPb_RhoSparse_INT7_lumi.root"
#out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
out="/home/austin/alice/pPbTestPost/png"
radius=3
system="pA"
filetype="png"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotTriggerSwap.cxx("'$file'","'$out'",'$radius',"'$system'","'$filetype'")'
