#!/bin/bash

fMB="/home/austin/alice/data/pp8TeV_data/GA2284_default/INT7.root"
fEMC7="/home/austin/alice/data/pp8TeV_data/GA2284_default/EMC7.root"
fEJE="/home/austin/alice/data/pp8TeV_data/GA2284_default/EJE.root"
fMC="/home/austin/alice/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotTriggerBias.cxx("'$fMB'","'$fEMC7'","'$fEJE'","'$fMC'","'$out'","'$filetype'")'
