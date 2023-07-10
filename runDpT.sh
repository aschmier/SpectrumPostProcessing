#!/bin/bash

input="/media/austin/mightymouse/data/pPb8TeV_data/GA995_PerpCone/AnalysisResults.root"
output="/home/austin/alice/SpectrumPostProcessing/DpT"

suffix="default"
minradius=2
maxradius=6
fileType="png"

root -x -q -l -b 'plotDpT.cxx("'$input'", "'$output'", "'$fileType'", "'$suffix'", '$minradius', '$maxradius')'