#!/bin/bash

#INT7="/media/austin/mightymouse/data/pp8TeV_data/TrackQA/INT7.root"
#EMC7="/media/austin/mightymouse/data/pp8TeV_data/TrackQA/EMC7.root"
datafile="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2636_ptscheme_default/EJE.root"

#INT7="/media/austin/mightymouse/data/pPb8TeV_data/GA1097_full_MIP_TrackingQA/INT7.root"
#EMC7="/media/austin/mightymouse/data/pPb8TeV_data/GA1097_full_MIP_TrackingQA/EJ2.root"
#EJE="/media/austin/mightymouse/data/pPb8TeV_data/GA1097_full_MIP_TrackingQA/EJ1.root"

out="/home/austin/alice/SpectrumPostProcessing/HPlusMinus_Newer"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotTrackQA.cxx("'$datafile'","'$datafile'","'$datafile'","'$out'","pp")'
