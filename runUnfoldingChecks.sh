#!/bin/bash

file="/home/austin/alice/unfoldingResults/output_default/bayes_default_final_2294.root"
outputdir="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"
uType="SVD"
defaultreg=4

mkdir -p $outputdir

for (( r=2; r<=6; r++ ))
do
  root -x -q -l -b 'plotUnfoldingChecks.cxx("'$file'","'$outputdir'","'$filetype'",'$r','$defaultreg')'
done
