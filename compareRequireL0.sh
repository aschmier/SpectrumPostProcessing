#!/bin/bash

#ppRootFile="/home/austin/alice/FinalResultsRootFiles/FinalResults_NewBinning_pp8TeV.root"
#pPbRootFile="/home/austin/alice/FinalResultsRootFiles/FinalResults_pPb8TeV.root"
refRootFile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_default.root"
rL0RootFile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_requirel0/bayes_pPb_RequireL0.root"
output="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures"
fileType="png"
minradius=2
maxradius=5
scaleFactorRootFile="/home/austin/alice/SpectrumPostProcessing/scaleFactorPythia.root"
dSysRootFiles="/home/austin/alice/SystematicsRootFiles/pPb/RpPb"

[ -d $output ] || mkdir $output

root -x -q -l -b 'compareRequireL0.cxx("'$refRootFile'", "'$rL0RootFile'", "'$output'", "'$fileType'", '$minradius', '$maxradius')'