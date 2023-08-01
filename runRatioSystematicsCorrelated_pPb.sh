#!/bin/bash

fSysConfig="Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_defaultsys.root"
type="Bayes"
filetype="pdf"
out="/home/austin/alice/pPbTestPost"
dSysRootFiles="/home/austin/alice/SystematicsRootFiles/pPb"

[ -d $out ] || mkdir $out

for (( r=3; r<=5; r++ ))
do
  root -x -q -l -b 'plotRatioSystematicsCorrelated_pPb.cxx("'$fSysConfig'","'$type'",'$r',"'$filetype'","'$out'","'$dSysRootFiles'")'
done