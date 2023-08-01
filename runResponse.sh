#!/bin/bash

mcfile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_default.root"
output="/home/austin/alice/pPbTestPost/Response"
filetype="pdf"
system="pPb"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotResponse.cxx("'$mcfile'","'$output'","'$filetype'","'$system'")'
