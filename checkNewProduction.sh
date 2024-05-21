#!/bin/bash

ppfile="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
pPbFile="/media/austin/mightymouse/data/pPb8TeV_MC/merged_default/AnalysisResults.root"
outputdir="/home/austin/alice/SpectrumPostProcessing/newProductionQA"
fileType="png"
radius=2

root -x -q -l -b 'checkNewProduction.cxx("'$ppfile'","'$pPbFile'","'$outputdir'","'$fileType'",'$radius')'