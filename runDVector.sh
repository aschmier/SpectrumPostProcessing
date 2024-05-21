#!/bin/bash

file="/home/austin/alice/unfoldingResults/output_default/svd_default_2284.root"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotDVector.cxx("'$file'","'$out'","'$filetype'")'
