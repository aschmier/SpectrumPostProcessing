#!/bin/bash

file="/home/austin/alice/unfoldingResults/output_default/svd_default_final_2294.root"
outputdir="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"
rmin=2
rmax=6
regnum=6

mkdir -p $outputdir

root -x -q -l -b 'plotTriggerEfficiencies.cxx("'$file'","'$outputdir'","'$filetype'",'$rmin','$rmax','$regnum')'
