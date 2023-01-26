#!/bin/bash

#mbfile="/home/austin/alice/data/pp8TeV_data/GA2296_default_newbinning/INT7.root"
#emc7file="/home/austin/alice/data/pp8TeV_data/GA2296_default_newbinning/EMC7.root"
#ejefile="/home/austin/alice/data/pp8TeV_data/GA2296_default_newbinning/EJE.root"
#out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
#filetype="pdf"

#[ -d $out ] || mkdir $out

#for (( r=2; r<=6; r++ ))
#do
#  root -x -q -l -b 'plotRejectionFactor.cxx("'$mbfile'","'$emc7file'","'$ejefile'","'$out'",'$r',"'$filetype'")'
#done

pPbfile="/media/austin/mightymouse/data/pPb8TeV_data/GA951_default/AnalysisResults.root"
outpPb="/home/austin/alice/SpectrumPostProcessing/RejectionFactors_pPb"
filetypepPb="pdf"

[ -d $outpPb ] || mkdir $outpPb

for (( rpPb=2; rpPb<=6; rpPb++ ))
do
  root -x -q -l -b 'plotRejectionFactor.cxx("'$pPbfile'","'$pPbfile'","'$pPbfile'","'$outpPb'",'$rpPb',"'$filetypepPb'")'
done
