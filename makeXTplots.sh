#!/bin/bash

fileE1="/home/austin/alice/FinalResultsRootFiles/FinalResults_NewBinning_pp8TeV.root"
fileE2="/home/austin/alice/FinalResultsRootFiles/pp_analysis_root_files/jetspectrum13TeV.root"
folderE3="/home/austin/alice/FinalResultsRootFiles/HEPData_5TeV"
output="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures/xTScaling"
filetype="png"

E1=8
E2=13
E3=5

root 'makeXTplots.cxx("'$fileE1'","'$fileE2'", "'$folderE3'", '$E1', '$E2', '$E3', "'$output'", "'$filetype'")'
