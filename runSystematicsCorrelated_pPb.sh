#!/bin/bash

fdefault="Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_defaultsys.root"
type="Bayes"
filetype="pdf"
out="/home/austin/alice/pPbTestPost"

[ -d $out ] || mkdir $out

for (( r=2; r<=5; r++ ))
do
  root -x -q -l -b 'plotSystematicsCorrelated_pPb.cxx("'$fdefault'","'$type'",'$r',"'$filetype'","'$out'")'
done
