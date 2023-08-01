#!/bin/bash

int7file="/media/austin/mightymouse/data/pPb8TeV_data/merged/default/INT7.root"
ej2file="/media/austin/mightymouse/data/pPb8TeV_data/merged/default/EJ2.root"
ej1file="/media/austin/mightymouse/data/pPb8TeV_data/merged/default/EJ1.root"
mcfileEJ="/media/austin/mightymouse/data/pPb8TeV_MC/merged/default/AnalysisResults.root"
output="/home/austin/alice/pPbTestPost/png/rawSpectrum"
filetype="png"
radius=2

[ -d $output ] || mkdir $output

root -l -x -b -q 'constructRawSpectrum.cxx("'$int7file'","'$ej2file'","'$ej1file'","'$mcfileEJ'","'$output'","'$filetype'",'$radius')'