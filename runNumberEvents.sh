#!/bin/bash

base="/home/austin/alice/data/pp8TeV_data/GA2294_default_final"
mcfile="/home/austin/alice/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
out="/home/austin/alice/unfoldingResults/EventTable.tsv"

if [[ -f $out ]]; then
    rm $out
fi

echo "Event Table" >> $out
echo -e "Period\tINT7\tEMC7\tEJE" >> $out

period=""

for d in $base/*/; do
    if [[ "$d" == *"child_1"* ]]; then
        period="LHC12a"
    elif [[ "$d" == *"child_2"* ]]; then
        period="LHC12b"
    elif [[ "$d" == *"child_3"* ]]; then
        period="LHC12c"
    elif [[ "$d" == *"child_4"* ]]; then
        period="LHC12d"
    elif [[ "$d" == *"child_5"* ]]; then
        period="LHC12f"
    elif [[ "$d" == *"child_6"* ]]; then
        period="LHC12h"
    elif [[ "$d" == *"child_7"* ]]; then
        period="LHC12i"
    else
        period="Period not identified!"
    fi

    fileMerge=$d/runlist_merge/AnalysisResults.root
    fileMB=$d/runlist_6/AnalysisResults.root
    fileEMC7=$d/runlist_7/AnalysisResults.root
    fileEJE=$d/runlist_8/AnalysisResults.root

    if [[ -f "$fileMerge" ]]; then
        root -x -q -l -b 'getNumberEvents.cpp("'$period'","'$fileMerge'","'$fileMerge'","'$fileMerge'","'$out'")'
    else
        root -x -q -l -b 'getNumberEvents.cpp("'$period'","'$fileMB'","'$fileEMC7'","'$fileEJE'","'$out'")'
    fi
done

#root -x -q -l -b 'getNumberEvents.cpp("LHC16c2","'$mcfile'","'$mcfile'","'$mcfile'","'$out'")'
