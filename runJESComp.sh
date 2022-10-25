#!/bin/bash
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
jesfile0="/home/austin/alice/EnergyScaleRootFiles/EnergyScaleResults.root"
jesfile1="/home/austin/alice/EnergyScaleRootFiles/EnergyScaleResults_tc200.root"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotJESComp.cxx("'$jesfile0'","'$jesfile1'","'$out'","'$filetype'")'
