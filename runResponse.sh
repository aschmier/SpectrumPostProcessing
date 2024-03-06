#!/bin/bash

mcfile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"
outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/Response"
filetype="pdf"
system="pp"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotResponse.cxx("'$mcfile'","'$outputdir'","'$filetype'","'$system'")'
