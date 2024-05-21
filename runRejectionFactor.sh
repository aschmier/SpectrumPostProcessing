#!/bin/bash

datafile="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/merged/default/AnalysisResults.root"
mcfile="/media/austin/mightymouse/data/pp8TeV_MC/ptscheme_trains/merged/default/AnalysisResults.root"
out="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures"
filetype="pdf"
lowvallow=3.5
lowvaldefault=4.0
lowvalhigh=4.5
highvallow=12.0
highvaldefault=15.0
highvalhigh=20.0

[ -d $out ] || mkdir $out

for (( r=2; r<=2; r++ ))
do
  root -x -q -l -b 'plotRejectionFactor.cxx("'$datafile'","'$mcfile'","'$out'",'$r','$lowvallow','$highvaldefault',"LowRFLow","'$filetype'","pp")'
  root -x -q -l -b 'plotRejectionFactor.cxx("'$datafile'","'$mcfile'","'$out'",'$r','$lowvaldefault','$highvaldefault',"Default","'$filetype'","pp")'
  root -x -q -l -b 'plotRejectionFactor.cxx("'$datafile'","'$mcfile'","'$out'",'$r','$lowvalhigh','$highvaldefault',"LowRFHigh","'$filetype'","pp")'
  root -x -q -l -b 'plotRejectionFactor.cxx("'$datafile'","'$mcfile'","'$out'",'$r','$lowvaldefault','$highvallow',"HighRFLow","'$filetype'","pp")'
  root -x -q -l -b 'plotRejectionFactor.cxx("'$datafile'","'$mcfile'","'$out'",'$r','$lowvaldefault','$highvalhigh',"HighRFHigh","'$filetype'","pp")'
done