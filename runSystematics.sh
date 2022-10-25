#!/bin/bash

file="/home/austin/alice/unfoldingResults/output_default/bayes_default_2284.root"
#out="/home/austin/alice/unfoldingResults/"
type="Bayes"
radius=$1
filetype="pdf"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"

[ -d $out ] || mkdir $out

for (( r=2; r<=6; r++ ))
do
  root -x -q -l -b 'plotSystematics.cxx("'$file'","'$type'",'$r',"'$filetype'","'$out'")'
done
