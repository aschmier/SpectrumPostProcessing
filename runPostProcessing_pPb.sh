#!/bin/bash

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT


outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures/pPbFigures"
#outputdir="/home/austin/alice/SpectrumPostProcessing/SystematicsTest_pPb"

#outputdir="/home/austin/alice/SpectrumPostProcessing/POWHEGtest/pPb"

filetype="pdf"

# Raw input data file
fdata="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/default/AnalysisResults.root"

# MC input file
fmc="/media/austin/mightymouse/data/pPb8TeV_MC/ptscheme_trains/merged/default/AnalysisResults.root"

# Unfolded input files
fUnfolded_svd="unfolding/results_pPb/output_default/svd_pPb_default.root"
fUnfolded_bayes="unfolding/results_pPb/output_default/bayes_pPb_default.root"
fUnfolded_bayes_pp="unfolding/results_pp_pPbBinning/output_default/bayes_default_2702.root"
fUnfolded_charged="unfolding/results_pPb/output_default/bayes_pPb_defaultchargedrho.root"
fUnfolded_charged_pp="unfolding/results_pp_pPbBinning/output_default/bayes_DefaultScaleVal_BGSub_2785.root"

# MC Gen input file
fPYTHIA="/media/austin/mightymouse/data/MCGen/PYTHIA/8_16TeV/ptlimited/pythia_pp8160GeV.root"
fPOWHEG="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures/pPbFigures/POWHEG/POWHEGfullsysCoarse.root"

uType="Bayes"

dSysRootFiles="/home/austin/alice/SystematicsRootFiles/pPb"
dSysRootFilesRpPb="/home/austin/alice/SystematicsRootFiles/RpPb"
dSysRootFilesBase="/home/austin/alice/SystematicsRootFiles"
dSysRootFilespp="/home/austin/alice/SystematicsRootFiles/pp_pPbBinning"
dEnergyScaleRootFiles="/home/austin/alice/EnergyScaleRootFiles/pPb"
ResultsRootFilepPb8TeV="/home/austin/alice/FinalResultsRootFiles/FinalResults_pPb8TeV.root"
ResultsRootFilepp8TeV="/home/austin/alice/FinalResultsRootFiles/FinalResults_pp8TeV_Course.root"
inputtxtfiles="/home/austin/alice/SpectrumPostProcessing/input_txt_files/pPb"
inputtxtfilespp="/home/austin/alice/SpectrumPostProcessing/input_txt_files/pp_pPbBinning"
SpectrumRootFile13TeV="/home/austin/alice/FinalResultsRootFiles/jetspectrum13TeV.root"
RatioRootFile13TeV="/home/austin/alice/FinalResultsRootFiles/jetspectrumratios13TeV.root"
HEPData2p76TeV="/home/austin/alice/FinalResultsRootFiles/HEPData_2p76TeV"
HEPData5TeV="/home/austin/alice/FinalResultsRootFiles/HEPData_5TeV"
HEPDataATLAS="/home/austin/alice/FinalResultsRootFiles/HEPData_ATLAS_8TeV"
scaleFactorRootFile="/home/austin/alice/SpectrumPostProcessing/PythiaGenScaleFactorRoot/scaleFactorPythia.root"
pythiaGenMinBias8TeV="/media/austin/mightymouse/data/MCGen/PYTHIA/8TeV/PYTHIA_pp_8TeV/scale_factor/AnalysisResults.root"
pythiaGenMinBias8p16TeV="/media/austin/mightymouse/data/MCGen/PYTHIA/8_16TeV/PYTHIA_pp_816TeV/scale_factor/AnalysisResults.root"
pythiaGenScaleFactor="/home/austin/alice/SpectrumPostProcessing/PythiaGenScaleFactorRoot"
atlasRpA="/home/austin/alice/RpPb_ExperimentData/RpPb_ATLAS.tsv"
cmsRpA="/home/austin/alice/RpPb_ExperimentData/RpPb_CMS.tsv"
phnxRpA="/home/austin/alice/RpPb_ExperimentData/RpPb_PHENIX.tsv"
aliceRpA02="/home/austin/alice/RpPb_ExperimentData/RpPb_ALICEchj_R02.tsv"
aliceRpA03="/home/austin/alice/RpPb_ExperimentData/RpPb_ALICEchj_R03.tsv"
aliceRpA04="/home/austin/alice/RpPb_ExperimentData/RpPb_ALICEchj_R04.tsv"


minradius=2
maxradius=4

mkdir -p $outputdir
mkdir -p $dEnergyScaleRootFiles

# Do unfolding comparisons
#python /home/austin/alice/SubstructureAnalysis/unfolding/comparisons/comp1D/8TeV/makeallplots_8TeV.py $fUnfolded_svd $fUnfolded_bayes $outputdir "pPb" $filetype
#root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_svd'","SVD","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_bayes'","Bayes","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_svd'","SVD","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_bayes'","Bayes","'$outputdir'","'$filetype'")'

# Run systematics and plot rejection factor
# for (( r=2; r<=4; r++ ))
# do
#     root -x -q -l -b 'plotSystematicsCorrelated_pPb.cxx("'$fUnfolded_bayes'","'$fUnfolded_charged'","'$uType'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#     root -x -q -l -b 'plotSystematicsCorrelatedRpPb.cxx("'$fUnfolded_bayes_pp'", "'$fUnfolded_bayes'", "'$fUnfolded_charged_pp'", "'$fUnfolded_charged'", "'$uType'", '$r', "'$filetype'", "'$outputdir'", "'$dSysRootFilespp'","'$dSysRootFilesBase'", "'$inputtxtfilespp'", "'$inputtxtfiles'")'
#     if [ $r -ne 2 ]
#     then
#         root -x -q -l -b 'plotRatioSystematicsCorrelated_pPb.cxx("'$fUnfolded_bayes'","'$fUnfolded_charged'","'$uType'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#     fi
# #    root -x -q -l -b 'plotTriggerSwap.cxx("'$fUnfolded_bayes'","'$outputdir'",'$r',"pPb","'$filetype'")'
# done

# Make plots for analysis note
#root -x -q -l -b 'plotKinEff.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","pPb",2,4)'
#root -x -q -l -b 'plotDVector.cxx("'$fUnfolded_svd'","'$outputdir'","'$filetype'","pPb")'
#root -x -q -l -b 'plotTriggerBias.cxx("'$fdata'","'$fdata'","'$fdata'","'$fmc'","'$outputdir'","'$filetype'","'pPb'",2,4)'
#root -x -q -l -b 'plotTriggerBiasAll.cxx("'$fdata'","'$fdata'","'$fdata'","'$fmc'","'$outputdir'","'$filetype'","'pPb'",2,4)'
#root -x -q -l -b 'plotTriggerClusters.cxx("'$fdata'","'$fdata'","'$fdata'","'$outputdir'","'$filetype'","'pPb'",2,2)'
#root -x -q -l -b 'plotCorrRawSpec.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","pPb",2,4)'
#root -x -q -l -b 'extractEnergyScaleSlices.cxx("'$fmc'","'$outputdir'","'$filetype'",2,4)'
#root -x -q -l -b 'plotTriggerEfficiencies.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","'pPb'",2,4,6)'
#root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$fmc'","FullJet","default",2,4)'
#mv EnergyScaleResults_default.root $dEnergyScaleRootFiles/EnergyScaleResults.root
#root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults.root","'$outputdir'","","'$filetype'",2,4)'
#root -x -q -l -b 'plotJESComp.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults.root","'$dEnergyScaleRootFiles'/EnergyScaleResults_tc200.root","'$outputdir'","'$filetype'")'
#while read run || [[ -n $run ]];
#do
#    rfilerunwise="$rundir/$run/AnalysisResults.root"
#    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$dMC_runwise'/'$run'/AnalysisResults.root","FullJet","nodownscalecorr")'
#    mv EnergyScaleResults_nodownscalecorr.root $dEnergyScaleRootFiles/EnergyScaleResults_$run.root
#    root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults_'$run'.root","'$outputdir'","'$run'","'$filetype'")'
#done < $runsMC

# Plot final results and MC comparison
#root -x -q -l -b 'plotFinalSpectrum_pPb.cxx("'$fUnfolded_charged'","'$fPYTHIA'","'$fPOWHEG'","'$ResultsRootFilepp8TeV'","'$dSysRootFiles'","'$uType'","'$outputdir'","'$filetype'", 2, 4)'
#root -x -q -l -b 'getScaleFactorPythiaGen.cxx("'$pythiaGenMinBias8TeV'","'$pythiaGenMinBias8p16TeV'","'$pythiaGenScaleFactor'","'$outputdir'","'$filetype'", '$minradius', '$maxradius')'
#root -x -q -l -b 'constructRpPb.cxx("'$fUnfolded_bayes_pp'", "'$fUnfolded_bayes'", "'$scaleFactorRootFile'", "'$dSysRootFilesRpPb'", "'$outputdir'", "'$filetype'", '$minradius', '$maxradius')'
#root -x -q -l -b 'constructRpPb_3Panel.cxx("'$fUnfolded_charged_pp'", "'$fUnfolded_charged'", "'$scaleFactorRootFile'", "'$dSysRootFilesRpPb'", "'$outputdir'", "'$filetype'", '$minradius', '$maxradius')'
#root -x -q -l -b 'constructRpPbCombined.cxx("'$fUnfolded_bayes_pp'", "'$fUnfolded_bayes'", "'$scaleFactorRootFile'", "'$dSysRootFilesRpPb'", "'$atlasRpA'", "'$cmsRpA'", "'$phnxRpA'", "'$aliceRpA02'", "'$aliceRpA03'", "'$aliceRpA04'", "'$outputdir'", "'$filetype'", '$minradius', '$maxradius')'
root -x -q -l -b 'constructRpPbCombined_3Panel.cxx("'$fUnfolded_charged_pp'", "'$fUnfolded_charged'", "'$scaleFactorRootFile'", "'$dSysRootFilesRpPb'", "'$atlasRpA'", "'$cmsRpA'", "'$phnxRpA'", "'$aliceRpA02'", "'$aliceRpA03'", "'$aliceRpA04'", "'$outputdir'", "'$filetype'", '$minradius', '$maxradius')'
#root -x -q -l -b 'plotEnergyComparison_pPb.cxx("'$ResultsRootFilepp8TeV'","'$ResultsRootFilepPb8TeV'","'$outputdir'","'$filetype'", 2, 4)'
#root -x -q -l -b 'plotExperimentComparison.cxx("'$ResultsRootFile8TeV'","'$HEPDataATLAS'","'$outputdir'","'$filetype'")'
