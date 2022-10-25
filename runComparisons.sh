#!/bin/bash

svdinput=/home/austin/alice/unfoldingResults/output_default/svd_default_2284.root
bayesinput=/home/austin/alice/unfoldingResults/output_default/bayes_default_2284.root
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

python /home/austin/alice/SubstructureAnalysis/unfolding/comparisons/comp1D/8TeV/makeallplots_8TeV.py $svdinput $bayesinput $out $filetype
