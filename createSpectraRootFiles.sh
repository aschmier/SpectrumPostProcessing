#!/bin/bash

#HEPtextfile="/home/austin/alice/SpectrumPostProcessing/spectrahepfiles.txt"
HEPtextfile="/home/austin/alice/SpectrumPostProcessing/spectrahepfiles8tev.txt"
output="/home/austin/alice/SpectrumPostProcessing/HEPoutput"
radius=4

mkdir -p $output

root -x -q -l -b 'createSpectraRootFiles.cxx("'$HEPtextfile'", "'$output'", '$radius')'