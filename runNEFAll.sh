#!/bin/bash

fMB="/home/austin/alice/data/pp8TeV_data/GA2294_default_final/INT7.root"
fEMC7="/home/austin/alice/data/pp8TeV_data/GA2294_default_final/EMC7.root"
fEJE="/home/austin/alice/data/pp8TeV_data/GA2294_default_final/EJE.root"
out="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotNEFAll.cxx("'$fMB'","'$fEMC7'","'$fEJE'","'$out'","'$filetype'")'
