#!/bin/bash

fMB="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/INT7.root"
fEMC7="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EMC7.root"
fEJE="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EJE.root"
fMC="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
fpPb="/media/austin/mightymouse/data/pPb8TeV_data/GA951_default/AnalysisResults.root"
fpPbMC="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1476/AnalysisResults.root"
out="/home/austin/alice/SpectrumPostProcessing/NEFRpAComp"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotNEFRpAComp.cxx("'$fMB'","'$fEMC7'","'$fEJE'","'$fMC'","'$fpPb'","'$fpPbMC'","'$out'")'
