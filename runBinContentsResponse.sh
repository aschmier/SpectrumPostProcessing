#!/bin/bash

unfoldedfile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"
rawfile="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"

root -x -q -l -b 'getBinContentsResponse.cxx("'$unfoldedfile'","'$rawfile'")'
