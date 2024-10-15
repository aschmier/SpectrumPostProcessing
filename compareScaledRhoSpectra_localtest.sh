#!/bin/bash

testfile="/home/austin/alice/localRunMacro/pPb_8TeV_ScaleTest/LHC16r/AnalysisResults_1.root"

output="/home/austin/alice/SpectrumPostProcessing/ScaledRhoSpectraComparisons/localtest"
filetype="png"

[ -d $output ] || mkdir $output

root -l -x -b -q 'compareScaledRhoSpectra_localtest.cxx("'$testfile'","'$output'","'$filetype'","pPb")'