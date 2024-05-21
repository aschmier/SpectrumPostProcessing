#!/bin/bash

input="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"
output="/home/austin/alice/SpectrumPostProcessing/RawFromUnfolded"
filetype="pdf"
suffix="default"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotRawFromUnf.cxx("'$input'","'$output'","'$filetype'","'$suffix'")'
