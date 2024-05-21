#!/bin/bash

fComp1="/media/austin/mightymouse/data/pPb8TeV_data/GA1063_minbias_5x5/INT7.root"
fComp2="/media/austin/mightymouse/data/pPb8TeV_data/GA1065_minbias_Systematics_R02/INT7.root"
out="/home/austin/alice/pPbTestPost/SysTrainTest"

trigger="INT7"

[ -d $out ] || mkdir $out

root -x -q -l -b 'plotMinBiasComp.C("'$fComp1'","'$fComp2'","'$out'","'$trigger'","pPb")'
