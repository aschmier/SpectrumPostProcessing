#!/bin/bash

svdinput=/home/austin/alice/unfoldingResults/output_default/svd_default_2284.root
bayesinput=/home/austin/alice/unfoldingResults/output_default/bayes_default_2284.root
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotCompareIter.cxx("'$svdinput'","SVD","'$output'","'$filetype'")'
root -x -q -l -b 'plotCompareIter.cxx("'$bayesinput'","Bayes","'$output'","'$filetype'")'
