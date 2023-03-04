#!/bin/bash

default=/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root
filtered=/home/austin/alice/SpectrumPostProcessing/bayes_default_final_2423_ResponseFilter.root
output="/home/austin/alice/SpectrumPostProcessing/FilterComparison"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotFilterComparison.cxx("'$default'","'$filtered'","'$output'")'
