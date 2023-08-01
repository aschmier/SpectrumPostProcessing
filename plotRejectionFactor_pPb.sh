#!/bin/bash

int7file="/media/austin/mightymouse/data/pPb8TeV_data/GA1060_full_default/INT7.root"
ej2file="/media/austin/mightymouse/data/pPb8TeV_data/GA1077_trigger_EGTest/EJ2.root"
ej1file="/media/austin/mightymouse/data/pPb8TeV_data/GA1077_trigger_EGTest/EJ1.root"
mcppbfile="/media/austin/mightymouse/data/pPb8TeV_MC/merged_15383940_default/AnalysisResults.root"
outpPb="/home/austin/alice/pPbTestPost/png"
filetypepPb="png"

[ -d $outpPb ] || mkdir $outpPb

for (( rpPb=2; rpPb<=2; rpPb++ ))
do
  root -x -q -l -b 'plotRejectionFactor_pPb.cxx("'$int7file'","'$ej2file'","'$ej1file'","'$mcppbfile'","'$outpPb'",'$rpPb',"'$filetypepPb'")'
done
