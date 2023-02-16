#!/bin/bash

mbfile="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/INT7.root"
emc7file="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EMC7.root"
ejefile="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EJE.root"
mcfile="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

[ -d $out ] || mkdir $out

for (( r=2; r<=2; r++ ))
do
  root -x -q -l -b 'plotRejectionFactor.cxx("'$mbfile'","'$emc7file'","'$ejefile'","'$mcfile'","'$out'",'$r',"'$filetype'")'
done

#pPbfile="/media/austin/mightymouse/data/pPb8TeV_data/GA951_default/AnalysisResults.root"
#outpPb="/home/austin/alice/SpectrumPostProcessing/RejectionFactors_pPb"
#filetypepPb="pdf"

#[ -d $outpPb ] || mkdir $outpPb

#for (( rpPb=2; rpPb<=6; rpPb++ ))
#do
#  root -x -q -l -b 'plotRejectionFactor.cxx("'$pPbfile'","'$pPbfile'","'$pPbfile'","'$outpPb'",'$rpPb',"'$filetypepPb'")'
#done
