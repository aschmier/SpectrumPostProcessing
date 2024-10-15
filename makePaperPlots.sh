#!/bin/bash

fppFine="/home/austin/alice/FinalResultsRootFiles/FinalResults_pp8TeV.root"
fppCoarse="/home/austin/alice/FinalResultsRootFiles/FinalResults_pp8TeV.root"
fpPb="/home/austin/alice/FinalResultsRootFiles/FinalResults_pp8TeV.root"
outputdir="/home/austin/alice/PaperPlots/temp"
filetype="png"

mkdir -p $outputdir

root -x -q -l -b 'makePaperPlots.cxx("'$fppFine'","'$fppCoarse'","'$fpPb'","'$outputdir'","'$filetype'")'