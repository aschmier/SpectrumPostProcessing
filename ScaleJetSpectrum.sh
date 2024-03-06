#!/bin/bash

infile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/ppFiles_NewBinning/output_default/bayes_default_final_2423.root"
outfile="/home/austin/alice/SpectrumPostProcessing/Scaledpp8TeV.root"
energyin=8
energyout=8.16
minradius=2
maxradius=5
nvar=4.85

root -x -q -l -b 'ScaleJetSpectrum.C("'$infile'","'$outfile'",'$energyin','$energyout','$minradius','$maxradius','$nvar')'