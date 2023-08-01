#!/bin/bash

#INT7="/media/austin/mightymouse/data/pp8TeV_data/TrackQA/INT7.root"
#EMC7="/media/austin/mightymouse/data/pp8TeV_data/TrackQA/EMC7.root"
#EJE="/media/austin/mightymouse/data/pp8TeV_data/TrackQA/EJE.root"

INT7="/media/austin/mightymouse/data/pPb8TeV_data/GA1097_full_MIP_TrackingQA/INT7.root"
EMC7="/media/austin/mightymouse/data/pPb8TeV_data/GA1097_full_MIP_TrackingQA/EJ2.root"
EJE="/media/austin/mightymouse/data/pPb8TeV_data/GA1097_full_MIP_TrackingQA/EJ1.root"

out="/home/austin/alice/SpectrumPostProcessing/HPlusMinus_New/pPb"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotTrackQA.cxx("'$INT7'","'$EMC7'","'$EJE'","'$out'","pPb")'
