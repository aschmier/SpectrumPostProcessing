#!/bin/bash

ppRootFile="/home/austin/alice/FinalResultsRootFiles/FinalResults_NewBinning_pp8TeV.root"
pPbRootFile="/home/austin/alice/FinalResultsRootFiles/FinalResults_pPb8TeV.root"
output="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures"
fileType="png"
minradius=2
maxradius=4

root -x -q -l -b 'constructRpPb.cxx("'$ppRootFile'", "'$pPbRootFile'", "'$output'", "'$fileType'", '$minradius', '$maxradius')'