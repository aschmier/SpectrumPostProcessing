#!/bin/bash

# This script is used to run getPPbStats.cxx
folderPath1="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/CentralBarrelCaloDPG"
folderPath2="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet"
folderPath3="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16s/pass2_merged/CentralBarrelCaloDPG"
folderPath4="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16s/pass2_merged/TPCTestSet"

root -x -l -b -q 'getPPbStats.cxx("'$folderPath1'")'
root -x -l -b -q 'getPPbStats.cxx("'$folderPath2'")'
root -x -l -b -q 'getPPbStats.cxx("'$folderPath3'")'
root -x -l -b -q 'getPPbStats.cxx("'$folderPath4'")'