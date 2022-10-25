#!/bin/bash

mcfile="/home/austin/alice/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

[ -d $output ] || mkdir $output

root -x -q -l -b 'extractEnergyScaleSlices.cxx("'$mcfile'","'$output'","'$filetype'")'
