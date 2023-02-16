#!/bin/bash

inWR="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1476/AnalysisResults.root"
inNR="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1461/AnalysisResults.root"
inPP="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final_old/AnalysisResults.root"

out="/home/austin/alice/SpectrumPostProcessing/ConstituentStudy/All"
#out="/home/austin/alice/SpectrumPostProcessing/ConstituentStudy/Charged"
#out="/home/austin/alice/SpectrumPostProcessing/ConstituentStudy/Neutral"

#inWR="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1476/mergedtrains/18/AnalysisResults.root"
#inNR="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1461/mergedtrains/18/AnalysisResults.root"
#inPP="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/18/AnalysisResults.root"

#out="/home/austin/alice/SpectrumPostProcessing/ConstituentStudy/Bin18"

suffix="nodownscalecorr"
radius=2

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotNConst.cxx("'$inPP'","'$inNR'","'$inWR'","'$out'","'$suffix'", '$radius',"All")'
