#!/bin/bash

fSysConfig="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_sys_2422.root"
type="Bayes"
filetype="pdf"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"

[ -d $out ] || mkdir $out

for (( r=3; r<=6; r++ ))
do
  root -x -q -l -b 'plotRatioSystematicsCorrelated.cxx("'$fSysConfig'","'$type'",'$r',"'$filetype'","'$out'")'
done
