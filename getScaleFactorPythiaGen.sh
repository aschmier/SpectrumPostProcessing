#!/bin/bash

ppFile="/media/austin/mightymouse/data/MCGen/pp_8TeV_minbias/AnalysisResults.root"
pPbFile="/media/austin/mightymouse/data/MCGen/pp_816TeV_minbias/AnalysisResults.root"
filetype="png"
rootfiledir="/home/austin/alice/SpectrumPostProcessing/PythiaGenScaleFactorRoot"
outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures/PythiaGenScaleFactor"
minradius=2
maxradius=4

[ -d $outputdir ] || mkdir $outputdir

root -x -q -l -b 'getScaleFactorPythiaGen.cxx("'$ppFile'","'$pPbFile'","'$rootfiledir'","'$outputdir'","'$filetype'", '$minradius', '$maxradius')'