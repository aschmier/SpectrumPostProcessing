#!/bin/bash

#infile="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/pp_FullJet_Spectra.root"
infile="/home/austin/alice/FinalResultsRootFiles/cms_atlas_rootfiles.root"
outfile="/home/austin/alice/SpectrumPostProcessing/xTFitting/atlascms"
minradius=4
maxradius=4

mkdir -p $outfile

root -x -q -l -b 'iterXTscaling.cxx("'$infile'","'$outfile'",'$minradius','$maxradius')'