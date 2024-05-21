#!/bin/bash

in="/home/austin/alice/unfoldingResults/output_default/bayes_TriggerRFDefault_2296.root"
out="/home/austin/alice/unfoldingResults/RFactorsFromUnfoldingMacro"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotRFBasic.cpp("'$in'","'$out'")'
