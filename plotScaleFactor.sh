#!/bin/bash

filepPb="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/GA1189_TrackQA_RhoScaleTTCorr/AnalysisResults.root"
filepp="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2785_ptscheme_fullscaledrhotest/AnalysisResults.root"
outputpPb="/home/austin/alice/SpectrumPostProcessing/RhoScaling/pPb_1189"
outputpp="/home/austin/alice/SpectrumPostProcessing/RhoScaling/pp_2775"
filetype="png"
centmin=0
centmax=100

root -x -l -b -q 'plotScaleFactor.cxx("'$filepPb'", "'$outputpPb'", "'$filetype'", "pPb", '$centmin', '$centmax')'
#root -x -l -b -q 'plotScaleFactor.cxx("'$filepp'", "'$outputpp'", "'$filetype'", "pp", '$centmin', '$centmax')'