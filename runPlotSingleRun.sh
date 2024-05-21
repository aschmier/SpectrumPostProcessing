#!/bin/bash

radius=2
output="/home/austin/alice/SpectrumPostProcessing/SingleRunQA"

# Run 266187, period LHC16r, CentralBarrelCaloDPG
input="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/CentralBarrelCaloDPG/266187/AnalysisResults.root"
run=266187
# Run 266997, period LHC16s, CentralBarrelCaloDPG
input2="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16s/pass2_merged/CentralBarrelCaloDPG/266997/AnalysisResults.root"
run2=266997
# Run 265700, period LHC16r, TPCTestSet
input3="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/265700/AnalysisResults.root"
run3=256700
# Run 265705, period LHC16r, TPCTestSet
input4="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/265705/AnalysisResults.root"
run4=265705
# Run 265797, period LHC16r, TPCTestSet
input5="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/265797/AnalysisResults.root"
run5=265797
# Run 266193, period LHC16r, TPCTestSet
input6="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/266193/AnalysisResults.root"
run6=266193
# Run 266304, period LHC16r, TPCTestSet
input7="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/266304/AnalysisResults.root"
run7=266304
# Run 265741, period LHC16r, TPCTestSet
input8="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/265741/AnalysisResults.root"
run8=265741
# Run 266034, period LHC16r, TPCTestSet
input9="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/266034/AnalysisResults.root"
run9=266034
# Run 265756, period LHC16r, TPCTestSet
input10="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_nosub_runwise/LHC16r/pass2_merged/TPCTestSet/maybe/265756/AnalysisResults.root"
run10=265756

#root -x -q -l -b 'plotSingleRun.cxx("'$input'",'$radius','$run',"'$output'")'
#root -x -q -l -b 'plotSingleRun.cxx("'$input2'",'$radius','$run2',"'$output'")'
#root -x -q -l -b 'plotSingleRun.cxx("'$input3'",'$radius','$run3',"'$output'")'
#root -x -q -l -b 'plotSingleRun.cxx("'$input4'",'$radius','$run4',"'$output'")'
#root -x -q -l -b 'plotSingleRun.cxx("'$input5'",'$radius','$run5',"'$output'")'
#root -x -q -l -b 'plotSingleRun.cxx("'$input6'",'$radius','$run6',"'$output'")'
#root -x -q -l -b 'plotSingleRun.cxx("'$input7'",'$radius','$run7',"'$output'")'
#root -x -q -l -b 'plotSingleRun.cxx("'$input8'",'$radius','$run8',"'$output'")'
#root -x -q -l -b 'plotSingleRun.cxx("'$input9'",'$radius','$run9',"'$output'")'
root -x -q -l -b 'plotSingleRun.cxx("'$input10'",'$radius','$run10',"'$output'")'
