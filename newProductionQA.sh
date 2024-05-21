#!/bin/bash

ppFile1="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/01/AnalysisResults.root"
ppFile2="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/02/AnalysisResults.root"
ppFileAll="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/merged0102.root"
pPbFile1="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1507/mergedtrains/01/AnalysisResults.root"
pPbFile2="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1507/mergedtrains/02/AnalysisResults.root"
pPbFileAll="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1507/AnalysisResults.root"

outputdir="/home/austin/alice/SpectrumPostProcessing/newProductionQA"
mkdir -p $outputdir

root -x -q -l -b 'newProductionQA.cxx("'$ppFile1'", "'$pPbFile1'","'$outputdir'", 2, "png", 1)'
root -x -q -l -b 'newProductionQA.cxx("'$ppFile2'", "'$pPbFile2'","'$outputdir'", 2, "png", 2)'
root -x -q -l -b 'newProductionQA.cxx("'$ppFileAll'", "'$pPbFileAll'","'$outputdir'", 2, "png", 0)'