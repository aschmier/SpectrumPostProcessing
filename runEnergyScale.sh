#!/bin/bash

mcfile="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
rundir="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/runwise"
runfile="/home/austin/alice/unfoldingResults/runsLHC12c2_default.txt"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
jesfile="/home/austin/alice//EnergyScaleRootFiles/EnergyScaleResults.root"
jettype="Full"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$mcfile'","'$jettype'Jet","default")'
mv EnergyScaleResults_nodownscalecorr.root $out/EnergyScaleResults.root
root -x -q -l -b 'plotJES.cxx("'$jesfile'","'$out'","","'$filetype'")'

#while read run || [[ -n $run ]];
#do
#    rfilerunwise="$rundir/$run/AnalysisResults.root"
#    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$rfilerunwise'","'$jettype'Jet","nodownscalecorr")'
#    mv EnergyScaleResults_nodownscalecorr.root $out/EnergyScaleResults_$run.root
#    jesfilerunwise="/home/austin/alice/unfoldingResults/EnergyScale/EnergyScaleResults_$run.root"
#    root -x -q -l -b 'plotJES.cxx("'$jesfilerunwise'","'$out'","'$run'","'$filetype'")'
#done < $runfile
