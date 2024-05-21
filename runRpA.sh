#!/bin/bash

ppfile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"
pAfile="/home/austin/alice/SpectrumPostProcessing/bayes_pPb_RhoSparse_INT7.root"
out="/home/austin/alice/SpectrumPostProcessing/RpA/New"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotRpA.C("'$ppfile'","'$pAfile'","'$out'","Bayes",2)'
