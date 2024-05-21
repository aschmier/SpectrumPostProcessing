#!/bin/bash

in="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final"
out="/home/austin/alice/SpectrumPostProcessing/FilteredResponse"

[ -d $out ] || mkdir $out

root -x -q -l -b 'filterResponse.cxx("'$in'","'$out'")'
