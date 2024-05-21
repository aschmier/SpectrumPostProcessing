#!/bin/bash

alicefiles="/home/austin/alice/FinalResultsRootFiles/FinalResults_pp8TeV.root"
atlasfiles="/home/austin/alice/FinalResultsRootFiles/atlas8tev_statonly.root"
output="/home/austin/alice/SpectrumPostProcessing/ExperimentComparison"
fileType="png"
maxradius=4
minradius=4

mkdir -p $output

root -x -q -l -b 'plotExperimentComparison.cxx("'$alicefiles'", "'$atlasfiles'", "'$output'", "'$fileType'", '$minradius', '$maxradius')'