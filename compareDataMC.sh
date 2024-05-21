#!/bin/bash

unfoldedfilepp="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/ppFiles_NewBinning/output_default/bayes_default_final_2423.root"
unfoldedfilepPb="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/withoutL0/bayes_pPb_default.root"
mcfilepp="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
mcfilepPb="/media/austin/mightymouse/data/pPb8TeV_MC/merged_RequireL0/AnalysisResults.root"
output="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures"
fileType="png"
minradius=2
maxradius=5

[ -d $output ] || mkdir $output

root -x -q -l -b 'compareDataMC.cxx("'$unfoldedfilepp'","'$unfoldedfilepPb'", "'$mcfilepp'", "'$mcfilepPb'", "'$output'", '$minradius', '$maxradius', "'$fileType'")'