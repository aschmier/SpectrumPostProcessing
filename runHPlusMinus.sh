#!/bin/bash

INT7="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/INT7.root"
EMC7="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EMC7.root"
EJE="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EJE.root"

out="/home/austin/alice/SpectrumPostProcessing/HPlusMinus"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotHPlusMinus.cxx("'$INT7'","'$EMC7'","'$EJE'","'$out'")'
