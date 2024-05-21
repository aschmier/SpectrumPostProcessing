#!/bin/bash

file="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/GA1131_rhoscaling/EJ1.root"
#file="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/GA1134_RhoScaleINT/EJ1.root"
output="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures/RhoScaling/all"
#output="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures/RhoScaling/int"
filetype="png"
centmin=0
centmax=100

root -x -l -b -q 'plotScaleFactor.cxx("'$file'", "'$output'", "'$filetype'", '$centmin', '$centmax')'