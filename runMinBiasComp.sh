#!/bin/bash

#fdata_pp="/home/austin/alice/data/pp8TeV_data/GA2294_default_final/INT7.root"
fmc_pp="/home/austin/alice/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"

#fdata_pA="/home/austin/alice/data/pPb8TeV_data/GA951/pPb_GA951.root"
fmc_pA="/home/austin/alice/data/pPb8TeV_MC/merged_EPOSCorrected/AnalysisResults.root"

out="/home/austin/alice/unfoldingResults/RpA_MC"

trigger="INT7"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotMinBiasComp.C("'$fmc_pp'","'$fmc_pA'","'$out'","'$trigger'","pp")'

#root -x -q -l -b 'plotMinBiasComp.C("'$fdata_pp'","'$fmc_pp'","'$out'","'$trigger'","pp")'
#root -x -q -l -b 'plotMinBiasComp.C("'$fdata_pA'","'$fmc_pA'","'$out'","'$trigger'","pPb")'
