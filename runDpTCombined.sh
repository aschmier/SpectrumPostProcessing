#!/bin/bash

#input="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/default/AnalysisResults.root"
inputpPb="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/GA1177_ChargedRho_fulltest/AnalysisResults.root"
inputpp="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2770_ptscheme_rhosub/AnalysisResults.root"
outputpPbcharged="/home/austin/alice/SpectrumPostProcessing/DpT/ChargedRho/pPb"
outputppcharged="/home/austin/alice/SpectrumPostProcessing/DpT/ChargedRho/pp"
outputpPbfull="/home/austin/alice/SpectrumPostProcessing/DpT/FullRho/pPb"
outputppfull="/home/austin/alice/SpectrumPostProcessing/DpT/FullRho/pp"
outputcombined="/home/austin/alice/SpectrumPostProcessing/DpT/Combined"
datafile="/home/austin/alice/SpectrumPostProcessing/DpT/megan_rho_data.csv"

inputppcharged="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/merged/default_chargedrho/AnalysisResults.root"
inputppfull="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/merged/default_fullrho/AnalysisResults.root"

inputpPbfull="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/default_fullrho/AnalysisResults.root"
inputpPbcharged="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/default_chargedrho/AnalysisResults.root"


mkdir -p $outputpPbcharged
mkdir -p $outputppcharged
mkdir -p $outputpPbfull
mkdir -p $outputppfull
mkdir -p $outputcombined

suffixFull="default"
suffixCharged="DefaultScaleVal"
minradius=2
maxradiuspPb=4
maxradiuspp=5
fileType="png"

#root -x -q -l -b 'plotDpT.cxx("'$inputpPb'", "'$outputpPb'", "'$fileType'", "'$suffix'", '$minradius', '$maxradius',"'$datafile'","pPb")'
#root -x -q -l -b 'plotDpT.cxx("'$inputpp'", "'$outputpp'", "'$fileType'", "'$suffix'", '$minradius', '$maxradius',"'$datafile'","pp")'

#root -x -q -l -b 'plotDpT.cxx("'$inputppcharged'", "'$outputppcharged'", "'$fileType'", "'$suffixCharged'", '$minradius', '$maxradiuspp',"'$datafile'","pp")'
#root -x -q -l -b 'plotDpT.cxx("'$inputppfull'", "'$outputppfull'", "'$fileType'", "'$suffixFull'", '$minradius', '$maxradiuspp',"'$datafile'","pp")' 
#root -x -q -l -b 'plotDpT.cxx("'$inputpPbfull'", "'$outputpPbfull'", "'$fileType'", "'$suffixFull'", '$minradius', '$maxradius',"'$datafile'","pPb")'
#root -x -q -l -b 'plotDpT.cxx("'$inputpPbcharged'", "'$outputpPbcharged'", "'$fileType'", "'$suffixCharged'", '$minradius', '$maxradius',"'$datafile'","pPb")'

root -x -q -l -b 'plotDpTCombined.cxx("'$inputppcharged'", "'$inputppfull'", "'$inputpPbcharged'", "'$inputpPbfull'", "'$outputcombined'", "'$fileType'", "'$suffixCharged'", "'$suffixFull'", '$minradius', '$maxradiuspp', '$maxradiuspPb')'