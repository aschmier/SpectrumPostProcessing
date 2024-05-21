#!/bin/bash

fSysConfig="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_2284.root"
fFinConfig="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_newbinning_2284.root"
type="Bayes"
filetype="pdf"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"

[ -d $out ] || mkdir $out

for (( r=2; r<=6; r++ ))
do
  root -x -q -l -b 'plotSystematics.cxx("'$fSysConfig'","'$fFinConfig'","'$type'",'$r',"'$filetype'","'$out'")'
done
