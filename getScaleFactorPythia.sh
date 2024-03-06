#!/bin/bash

ppFile="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
pPbFile="/media/austin/mightymouse/data/pPb8TeV_MC/merged/default/AnalysisResults.root"
filetype="pdf"
rootfiledir="/home/austin/alice/SpectrumPostProcessing"
outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/ScaleFactorPythia"
minradius=2
maxradius=5

[ -d $outputdir ] || mkdir $outputdir

root -x -q -l -b 'getScaleFactorPythia.cxx("'$ppFile'","'$pPbFile'","'$rootfiledir'","'$outputdir'","'$filetype'", '$minradius', '$maxradius')'