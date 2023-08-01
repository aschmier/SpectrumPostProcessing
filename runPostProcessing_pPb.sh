#!/bin/bash

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT


outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures"
filetype="pdf"

# Raw input default files from skim dataset
fMB_sys="/media/austin/mightymouse/data/pPb8TeV_data/merged/defaultskim/INT7.root"
fEMC7_sys="/media/austin/mightymouse/data/pPb8TeV_data/merged/defaultskim/EJ2.root"
fEJE_sys="/media/austin/mightymouse/data/pPb8TeV_data/merged/defaultskim/EJ1.root"

# Raw input default files from full dataset
fMB_final="/media/austin/mightymouse/data/pPb8TeV_data/merged/default/INT7.root"
fEMC7_final="/media/austin/mightymouse/data/pPb8TeV_data/merged/default/EJ2.root"
fEJE_final="/media/austin/mightymouse/data/pPb8TeV_data/merged/default/EJ1.root"

# MC input file
fMC_final="/media/austin/mightymouse/data/pPb8TeV_MC/merged/default/AnalysisResults.root"

# Unfolded input file from full dataset
fUnfolded_svd_final="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/SVD_pPb_default.root"
fUnfolded_bayes_final="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_default.root"

# Unfolded input file with systematics config
fUnfolded_svd_sys="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/SVD_pPb_defaultskim.root"
fUnfolded_bayes_sys="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_defaultskim.root"

# MC Gen input file
fMCGen="/media/austin/mightymouse/data/pp8TeV_MC/PureMC_8TeV/ptlimited/AnalysisResults.root"

uTypeAna="Bayes"
uTypeSys="Bayes"

dSysRootFiles="/home/austin/alice/SystematicsRootFiles/pPb"
dEnergyScaleRootFiles="/home/austin/alice/EnergyScaleRootFiles/pPb"
ResultsRootFilepPb8TeV="/home/austin/alice/FinalResultsRootFiles/FinalResults_pPb8TeV.root"
ResultsRootFilepp8TeV="/home/austin/alice/FinalResultsRootFiles/FinalResults_pPb8TeV.root"
inputtxtfiles="/home/austin/alice/SpectrumPostProcessing/input_txt_files/pPbFiles"

mkdir -p $outputdir
mkdir -p $dEnergyScaleRootFiles

# Do unfolding comparisons
#python /home/austin/alice/SubstructureAnalysis/unfolding/comparisons/comp1D/8TeV/makeallplots_8TeV.py $fUnfolded_svd_final $fUnfolded_bayes_final $outputdir "pPb" $filetype
#root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_svd_final'","SVD","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_bayes_final'","Bayes","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_svd_final'","SVD","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_bayes_final'","Bayes","'$outputdir'","'$filetype'")'

# Run systematics and plot rejection factor
#for (( r=2; r<=5; r++ ))
#do
  #root -x -q -l -b 'plotSystematicsCorrelated_pPb.cxx("'$fUnfolded_bayes_sys'","'$uTypeSys'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#  if [ $r -ne 2 ]
#  then
#    root -x -q -l -b 'plotRatioSystematicsCorrelated_pPb.cxx("'$fUnfolded_bayes_sys'","'$uTypeSys'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#  fi
#  root -x -q -l -b 'plotTriggerSwap.cxx("'$fUnfolded_bayes_final'","'$outputdir'",'$r',"pPb","'$filetype'")'
#done

# Make plots for analysis note
#root -x -q -l -b 'plotKinEff.cxx("'$fUnfolded_bayes_final'","'$outputdir'","'$filetype'","pPb")'
#root -x -q -l -b 'plotDVector.cxx("'$fUnfolded_svd_final'","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotTriggerBias.cxx("'$fMB_final'","'$fEMC7_final'","'$fEJE_final'","'$fMC_final'","'$outputdir'","'$filetype'","'pPb'",2,5)'
#root -x -q -l -b 'plotTriggerBiasAll.cxx("'$fMB_final'","'$fEMC7_final'","'$fEJE_final'","'$fMC_final'","'$outputdir'","'$filetype'","'pPb'",2,5)'
#root -x -q -l -b 'plotTriggerClusters.cxx("'$fMB_sys'","'$fEMC7_sys'","'$fEJE_sys'","'$outputdir'","'$filetype'","'pPb'",2,2)'
#root -x -q -l -b 'plotTriggerClusters.cxx("'$fMB_final'","'$fEMC7_final'","'$fEJE_final'","'$outputdir'","'$filetype'","'pPb'",2,2)'
#root -x -q -l -b 'plotCorrRawSpec.cxx("'$fUnfolded_bayes_final'","'$outputdir'","'$filetype'","pPb",2,5)'
#root -x -q -l -b 'extractEnergyScaleSlices.cxx("'$fMC_final'","'$outputdir'","'$filetype'",2,5)'
#root -x -q -l -b 'plotTriggerEfficiencies.cxx("'$fUnfolded_bayes_final'","'$outputdir'","'$filetype'","'pPb'",2,5,6)'
#root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$fMC_final'","FullJet","default")'
#mv EnergyScaleResults_default.root $dEnergyScaleRootFiles/EnergyScaleResults.root
#root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults.root","'$outputdir'","","'$filetype'",2,5)'
#root -x -q -l -b 'plotJESComp.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults.root","'$dEnergyScaleRootFiles'/EnergyScaleResults_tc200.root","'$outputdir'","'$filetype'")'
#while read run || [[ -n $run ]];
#do
#    rfilerunwise="$rundir/$run/AnalysisResults.root"
#    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$dMC_runwise'/'$run'/AnalysisResults.root","FullJet","nodownscalecorr")'
#    mv EnergyScaleResults_nodownscalecorr.root $dEnergyScaleRootFiles/EnergyScaleResults_$run.root
#    root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults_'$run'.root","'$outputdir'","'$run'","'$filetype'")'
#done < $runsMC

# Plot final results and MC comparison
root -x -q -l -b 'plotFinalSpectrum_pPb.cxx("'$fUnfolded_bayes_final'","'$fMCGen'","'$dSysRootFiles'","'$uTypeAna'","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotEnergyComparison.cxx("'$ResultsRootFile8TeV'","'$HEPData2p76TeV'","'$HEPData5TeV'","'$SpectrumRootFile13TeV'","'$RatioRootFile13TeV'","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotExperimentComparison.cxx("'$ResultsRootFile8TeV'","'$HEPDataATLAS'","'$outputdir'","'$filetype'")'
