#!/bin/bash

#mbfile="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/INT7.root"
#emc7file="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EMC7.root"
#ejefile="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EJE.root"
#mcfile="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
#out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
#filetype="pdf"

#[ -d $out ] || mkdir $out

#for (( r=2; r<=2; r++ ))
#do
#  root -x -q -l -b 'plotRejectionFactor.cxx("'$mbfile'","'$emc7file'","'$ejefile'","'$mcfile'","'$out'",'$r',"'$filetype'")'
#done

int7file="/media/austin/mightymouse/data/pPb8TeV_data/GA1060_full_default/INT7.root"
#ej2file="/media/austin/mightymouse/data/pPb8TeV_data/GA1060_full_default/EJ2.root"
#ej1file="/media/austin/mightymouse/data/pPb8TeV_data/GA1060_full_default/EJ1.root"
ej2file="/media/austin/mightymouse/data/pPb8TeV_data/GA1077_trigger_EGTest/EJ2.root"
ej1file="/media/austin/mightymouse/data/pPb8TeV_data/GA1077_trigger_EGTest/EJ2.root"
mcppbfile="/media/austin/mightymouse/data/pPb8TeV_MC/merged_15383940_default/AnalysisResults.root"
outpPb="/home/austin/alice/pPbTestPost/png"
filetypepPb="png"

[ -d $outpPb ] || mkdir $outpPb

for (( rpPb=2; rpPb<=2; rpPb++ ))
do
  root -x -q -l -b 'plotRejectionFactor.cxx("'$int7file'","'$ej2file'","'$ej1file'","'$mcppbfile'","'$outpPb'",'$rpPb',"'$filetypepPb'","pPb")'
done
