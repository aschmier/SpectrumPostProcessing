#!/bin/bash

default=/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root
filtered=/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423_ResponseFilter.root
output="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures/FilterComparison"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotFilterComparison.cxx("'$default'","'$filtered'","'$output'")'
