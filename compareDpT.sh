#!/bin/bash

input1="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_runlistEJ1.root"
input2="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_runlistEJ1_DpTEJ1.root"
output="/home/austin/alice/SpectrumPostProcessing/DpT"

suffix="default"
minradius=2
maxradius=4
fileType="png"

root -x -q -l -b 'compareDpT.cxx("'$input1'", "'$input2'", "'$output'", "'$fileType'", "'$suffix'", '$minradius', '$maxradius')'