#!/bin/bash

option=$1

fUnfolded_bayes_final="/home/austin/alice/SpectrumPostProcessing/bayes_default_final_2423_$option.root"
fUnfolded_svd_final="/home/austin/alice/SpectrumPostProcessing/bayes_default_final_2423_$option.root"

outputdir="/home/austin/alice/SpectrumPostProcessing/Comparisons/Option_$option"
filetype="png"


python /home/austin/alice/SubstructureAnalysis/unfolding/comparisons/comp1D/8TeV/makeallplots_8TeV.py $fUnfolded_svd_final $fUnfolded_bayes_final $outputdir $filetype
