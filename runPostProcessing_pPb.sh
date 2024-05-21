mc#!/bin/bash

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT


outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures"
#outputdir="/home/austin/alice/SpectrumPostProcessing/POWHEGtest/pPb"

filetype="pdf"

# Raw input data file
fdata="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged/default/AnalysisResults.root"

# MC input file
fmc="/media/austin/mightymouse/data/pPb8TeV_MC/ptscheme_trains/merged/default/AnalysisResults.root"

# Unfolded input files
fUnfolded_svd="unfolding/results_pPb/output_default/svd_pPb_default.root"
fUnfolded_bayes="unfolding/results_pPb/output_default/bayes_pPb_default.root"
fUnfolded_bayes_pp="unfolding/results_pp/output_default/bayes_pPbBinning_2698.root"


# MC Gen input file
fPYTHIA="/media/austin/mightymouse/data/MCGen/PYTHIA/8_16TeV/PYTHIA_pp_816TeV/ptlimited/AnalysisResults.root"
fPOWHEG="/media/austin/mightymouse/data/MCGen/POWHEG/Scaling_816TeV_cent/816TeV/CT14withnegweights_boost/Pythia8JetSpectra_merged.root"

uType="Bayes"

dSysRootFiles="/home/austin/alice/SystematicsRootFiles/pPb"
dSysRootFilesBase="/home/austin/alice/SystematicsRootFiles"
dSysRootFilespp="/home/austin/alice/SystematicsRootFiles/ppNewBin"
dEnergyScaleRootFiles="/home/austin/alice/EnergyScaleRootFiles/pPb"
ResultsRootFilepPb8TeV="/home/austin/alice/FinalResultsRootFiles/FinalResults_pPb8TeV.root"
ResultsRootFilepp8TeV="/home/austin/alice/FinalResultsRootFiles/FinalResults_pp8TeV.root"
inputtxtfiles="/home/austin/alice/SpectrumPostProcessing/input_txt_files/pPb"
inputtxtfilespp="/home/austin/alice/SpectrumPostProcessing/input_txt_files/pp"
SpectrumRootFile13TeV="/home/austin/alice/FinalResultsRootFiles/jetspectrum13TeV.root"
RatioRootFile13TeV="/home/austin/alice/FinalResultsRootFiles/jetspectrumratios13TeV.root"
HEPData2p76TeV="/home/austin/alice/FinalResultsRootFiles/HEPData_2p76TeV"
HEPData5TeV="/home/austin/alice/FinalResultsRootFiles/HEPData_5TeV"
HEPDataATLAS="/home/austin/alice/FinalResultsRootFiles/HEPData_ATLAS_8TeV"


mkdir -p $outputdir
mkdir -p $dEnergyScaleRootFiles

# Do unfolding comparisons
#python /home/austin/alice/SubstructureAnalysis/unfolding/comparisons/comp1D/8TeV/makeallplots_8TeV.py $fUnfolded_svd $fUnfolded_bayes $outputdir "pPb" $filetype
#root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_svd'","SVD","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_bayes'","Bayes","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_svd'","SVD","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_bayes'","Bayes","'$outputdir'","'$filetype'")'

# Run systematics and plot rejection factor
for (( r=2; r<=4; r++ ))
do
#    root -x -q -l -b 'plotSystematicsCorrelated_pPb.cxx("'$fUnfolded_bayes'","'$uType'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#    root -x -q -l -b 'plotSystematicsCorrelatedRpPb.cxx("'$fUnfolded_bayes_pp'", "'$fUnfolded_bayes'", "'$uType'", '$r', "'$filetype'", "'$outputdir'", "'$dSysRootFilespp'","'$dSysRootFilesBase'", "'$inputtxtfilespp'", "'$inputtxtfiles'")'
#        if [ $r -ne 2 ]
#    then
#        root -x -q -l -b 'plotRatioSystematicsCorrelated_pPb.cxx("'$fUnfolded_bayes'","'$uType'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#    fi
    root -x -q -l -b 'plotTriggerSwap.cxx("'$fUnfolded_bayes'","'$outputdir'",'$r',"pPb","'$filetype'")'
done

# Make plots for analysis note
#root -x -q -l -b 'plotKinEff.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","pPb")'
#root -x -q -l -b 'plotDVector.cxx("'$fUnfolded_svd'","'$outputdir'","'$filetype'","pPb")'
#root -x -q -l -b 'plotTriggerBias.cxx("'$fdata'","'$fdata'","'$fdata'","'$fmc'","'$outputdir'","'$filetype'","'pPb'",2,5)'
#root -x -q -l -b 'plotTriggerBiasAll.cxx("'$fdata'","'$fdata'","'$fdata'","'$fmc'","'$outputdir'","'$filetype'","'pPb'",2,4)'
#root -x -q -l -b 'plotTriggerClusters.cxx("'$fdata'","'$fdata'","'$fdata'","'$outputdir'","'$filetype'","'pPb'",2,2)'
#root -x -q -l -b 'plotTriggerClusters.cxx("'$fdata'","'$fdata'","'$fdata'","'$outputdir'","'$filetype'","'pPb'",2,4)'
#root -x -q -l -b 'plotCorrRawSpec.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","pPb",2,4)'
#root -x -q -l -b 'extractEnergyScaleSlices.cxx("'$fmc'","'$outputdir'","'$filetype'",2,5)'
#root -x -q -l -b 'plotTriggerEfficiencies.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'","'pPb'",2,4,6)'
#root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$fmc'","FullJet","default")'
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
#root -x -q -l -b 'plotFinalSpectrum_pPb.cxx("'$fUnfolded_bayes'","'$fPYTHIA'","'$fPOWHEG'","'$ResultsRootFilepp8TeV'","'$dSysRootFiles'","'$uType'","'$outputdir'","'$filetype'", 2, 4)'
#root -x -q -l -b 'plotEnergyComparison_pPb.cxx("'$ResultsRootFilepp8TeV'","'$ResultsRootFilepPb8TeV'","'$outputdir'","'$filetype'", 2, 4)'
#root -x -q -l -b 'plotExperimentComparison.cxx("'$ResultsRootFile8TeV'","'$HEPDataATLAS'","'$outputdir'","'$filetype'")'
