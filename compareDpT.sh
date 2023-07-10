#!/bin/bash

input1="bayes_pPb_PerpCone_INT7_R02.root"
input2="bayes_pPb_PerpCone_EJ1_R02.root"
output="/home/austin/alice/SpectrumPostProcessing/DpT"

suffix="default"
minradius=2
maxradius=2
fileType="png"

root -x -q -l -b 'compareDpT.cxx("'$input1'", "'$input2'", "'$output'", "'$fileType'", "'$suffix'", '$minradius', '$maxradius')'