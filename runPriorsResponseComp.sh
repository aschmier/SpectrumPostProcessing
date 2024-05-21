#!/bin/bash

fdefault=/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_2284.root
fpriors=/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_priors/bayes_priors_2284.root
output="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures/PriorsResponseComparison"
filetype="pdf"

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotPriorsResponseComp.cxx("'$fdefault'","'$fpriors'","Bayes","'$output'","'$filetype'")'
