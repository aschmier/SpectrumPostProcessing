#!/bin/bash

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT


outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures"
#outputdir="/home/austin/alice/SpectrumPostProcessing/SystematicsTest"
#outputdir_pPbBin="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures/coursebinnedpp"
outputdir_pPbBin="/home/austin/alice/SpectrumPostProcessing/SystematicsTest/coursebinnedpp"

filetype="pdf"

# Raw input files
fdata="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/merged/default/AnalysisResults.root"
fmc="/media/austin/mightymouse/data/pp8TeV_MC/ptscheme_trains/merged/default/AnalysisResults.root"
#dmc_runwise="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/runwise"

# Unfolded input files, pp binning
fUnfolded_bayes="unfolding/results_pp/output_default/bayes_default_2702.root"
fUnfolded_svd="unfolding/results_pp/output_default/svd_default_2702.root"
fUnfolded_charged="unfolding/results_pp/output_default/bayes_DefaultScaleVal_BGSub_2785.root"

# Unfolded input files, pPb binning
fUnfolded_bayes_pPbBinning="unfolding/results_pp_pPbBinning/output_default/bayes_default_2702.root"
fUnfolded_charged_pPbBinning="unfolding/results_pp_pPbBinning/output_default/bayes_DefaultScaleVal_BGSub_2785.root"

# Generator input files
fPYTHIA="/media/austin/mightymouse/data/MCGen/PYTHIA/8TeV/ptlimited/pythia_pp8TeV.root"
fPOWHEG="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures/POWHEG/POWHEGfullsys.root"
fPOWHEG_pPbBinning="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/figures/POWHEG/POWHEGfullsysCoarse.root"

# LHC16c2 runlist
#runsMC="/home/austin/alice/pp8TeV_runlists/runsLHC16c2.txt"

uType="Bayes"

dSysRootFiles="/home/austin/alice/SystematicsRootFiles"
dSysRootFilespPbBinning="/home/austin/alice/SystematicsRootFiles/pp_pPbBinning"
dEnergyScaleRootFiles="/home/austin/alice/EnergyScaleRootFiles"
ResultsRootFile8TeV="/home/austin/alice/FinalResultsRootFiles/FinalResults_pp8TeV.root"
SpectrumRootFile13TeV="/home/austin/alice/FinalResultsRootFiles/jetspectrum13TeV.root"
RatioRootFile13TeV="/home/austin/alice/FinalResultsRootFiles/jetspectrumratios13TeV.root"
HEPData2p76TeV="/home/austin/alice/FinalResultsRootFiles/HEPData_2p76TeV"
HEPData5TeV="/home/austin/alice/FinalResultsRootFiles/HEPData_5TeV"
HEPDataATLAS="/home/austin/alice/FinalResultsRootFiles/HEPData_ATLAS_8TeV"
inputtxtfiles="/home/austin/alice/SpectrumPostProcessing/input_txt_files/pp"
inputtxtfilespPbBinning="/home/austin/alice/SpectrumPostProcessing/input_txt_files/pp_pPbBinning"

inputtxtfilesNewBin="/home/austin/alice/SpectrumPostProcessing/input_txt_files/ppNewBin"
dSysRootFilesNewBin="/home/austin/alice/SystematicsRootFiles/ppNewBin"
dEnergyScaleRootFilesNewBin="/home/austin/alice/EnergyScaleRootFiles/ppNewBin"
ResultsRootFile8TeVNewBin="/home/austin/alice/FinalResultsRootFiles/FinalResults_NewBinning_pp8TeV.root"

lowrf=4.0
highrf=12.25

mkdir -p $outputdir
mkdir -p $dEnergyScaleRootFiles

# Do unfolding comparisons
#python /home/austin/alice/SubstructureAnalysis/unfolding/comparisons/comp1D/8TeV/makeallplots_8TeV.py $fUnfolded_svd $fUnfolded_bayes $outputdir "pp" $filetype
#root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_svd'","SVD","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_bayes'","Bayes","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_svd'","SVD","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_bayes'","Bayes","'$outputdir'","'$filetype'")'

# Run systematics and plot rejection factor
# for (( r=2; r<=5; r++ ))
# do
#     root -x -q -l -b 'plotSystematicsCorrelated.cxx("'$fUnfolded_bayes'","'$fUnfolded_charged'","'$uType'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#     if [ $r -ne 5 ]
#     then
#         root -x -q -l -b 'plotSystematicsCorrelated.cxx("'$fUnfolded_bayes_pPbBinning'","'$fUnfolded_charged_pPbBinning'","'$uType'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFilespPbBinning'","'$inputtxtfilespPbBinning'","course")'
#         if [ $r -ne 2 ]
#         then
#             root -x -q -l -b 'plotRatioSystematicsCorrelated.cxx("'$fUnfolded_bayes_pPbBinning'","'$fUnfolded_charged_pPbBinning'","'$uType'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFilespPbBinning'","'$inputtxtfilespPbBinning'","course")'
#         fi
#     fi
#     if [ $r -ne 2 ]
#     then
#         root -x -q -l -b 'plotRatioSystematicsCorrelated.cxx("'$fUnfolded_bayes'","'$fUnfolded_charged'","'$uType'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#     fi
# #    root -x -q -l -b 'plotRejectionFactor.cxx("'$fdata'","'$fmc'","'$outputdir'",'$r','$lowrf','$highrf',"Default","'$filetype'","'pp'")'
# #    root -x -q -l -b 'plotTriggerSwap.cxx("'$fUnfolded_bayes'","'$outputdir'",'$r',"pp","'$filetype'")'
# done

#root -x -q -l -b 'plotRejectionFactor.cxx("'$fdata'","'$fdata'","'$fdata'","'$fmc'","'$outputdir'",2,"'$filetype'","'pp'")'


# Make plots for analysis note
#root -x -q -l -b 'plotKinEff.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","pp")'
#root -x -q -l -b 'plotDVector.cxx("'$fUnfolded_svd'","'$outputdir'","'$filetype'","pp")'
#root -x -q -l -b 'plotTriggerBias.cxx("'$fdata'","'$fdata'","'$fdata'","'$fmc'","'$outputdir'","'$filetype'","'pp'",2,5)'
#root -x -q -l -b 'plotTriggerBiasAll.cxx("'$fdata'","'$fdata'","'$fdata'","'$fmc'","'$outputdir'","'$filetype'","'pp'",2,5)'
#root -x -q -l -b 'plotTriggerClusters.cxx("'$fdata'","'$fdata'","'$fdata'","'$outputdir'","'$filetype'","'pp'",2,5)'
#root -x -q -l -b 'plotCorrRawSpec.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","pp",2,5)'
#root -x -q -l -b 'extractEnergyScaleSlices.cxx("'$fmc'","'$outputdir'","'$filetype'",2,5)'
#root -x -q -l -b 'plotTriggerEfficiencies.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","'pp'",2,5,6)'
#root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$fmc'","FullJet","default")'
#mv EnergyScaleResults_default.root $dEnergyScaleRootFiles/EnergyScaleResults.root
#root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults.root","'$outputdir'","","'$filetype'",2,5)'
#root -x -q -l -b 'plotJESComp.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults.root","'$dEnergyScaleRootFiles'/EnergyScaleResults_tc200.root","'$outputdir'","'$filetype'")'
#while read run || [[ -n $run ]];
#do
#    rfilerunwise="$rundir/$run/AnalysisResults.root"
#    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$dMC_runwise'/'$run'/AnalysisResults.root","FullJet","default")'
#    mv EnergyScaleResults_default.root $dEnergyScaleRootFiles/EnergyScaleResults_$run.root
#    root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults_'$run'.root","'$outputdir'","'$run'","'$filetype'")'
#done < $runsMC

# Plot final results and MC comparison
#root -x -q -l -b 'plotFinalSpectrum.cxx("'$fUnfolded_charged'","'$fPYTHIA'","'$fPOWHEG'","'$dSysRootFiles'","'$uType'","'$outputdir'","'$filetype'",2,5,"fine")'
#root -x -q -l -b 'plotFinalSpectrum.cxx("'$fUnfolded_charged_pPbBinning'","'$fPYTHIA'","'$fPOWHEG_pPbBinning'","'$dSysRootFilespPbBinning'","'$uType'","'$outputdir_pPbBin'","'$filetype'",2,4,"coarse")'
#root -x -q -l -b 'plotEnergyComparison.cxx("'$ResultsRootFile8TeV'","'$HEPData2p76TeV'","'$HEPData5TeV'","'$SpectrumRootFile13TeV'","'$RatioRootFile13TeV'","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotExperimentComparison.cxx("'$ResultsRootFile8TeV'","'$HEPDataATLAS'","'$outputdir'","'$filetype'")'
