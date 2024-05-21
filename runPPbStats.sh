#!/bin/bash

# This script is used to run getPPbStats.cxx
folderPath1="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/CentralBarrelCaloDPG"
folderPath2="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet"
folderPath3="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16s/pass2_merged/CentralBarrelCaloDPG"
folderPath4="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16s/pass2_merged/TPCTestSet"

folderpath5="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/pass2_EMCGoodJet"
folderpath6="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16s/pass2_merged/pass2_EMCGoodJet"

outputpath="/home/austin/alice/SpectrumPostProcessing/PPbStats"

#root -x -l -b -q 'getPPbStats.cxx("'$folderPath1'", "'$outputpath'", "LHC16r", "CentralBarrelCaloDPG")'
#root -x -l -b -q 'getPPbStats.cxx("'$folderPath2'", "'$outputpath'", "LHC16r", "TPCTestSet")'
#root -x -l -b -q 'getPPbStats.cxx("'$folderPath3'", "'$outputpath'", "LHC16s", "CentralBarrelCaloDPG")'
#root -x -l -b -q 'getPPbStats.cxx("'$folderPath4'", "'$outputpath'", "LHC16s", "TPCTestSet")'
root -x -l -b -q 'getPPbStats.cxx("'$folderpath5'", "'$outputpath'", "LHC16r", "pass2_EMCGoodJet")'
root -x -l -b -q 'getPPbStats.cxx("'$folderpath6'", "'$outputpath'", "LHC16s", "pass2_EMCGoodJet")'
