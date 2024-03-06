#!/bin/bash

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT


#outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/ppNewBinningFigures"
outputdir="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures"
#outputdir="/home/austin/alice/SpectrumPostProcessing/POWHEGtest"
filetype="pdf"

# Raw input files with systematics config
fMB_sys="/media/austin/mightymouse/data/pp8TeV_data/GA2422_default_sys/INT7.root"
fEMC7_sys="/media/austin/mightymouse/data/pp8TeV_data/GA2422_default_sys/EMC7.root"
fEJE_sys="/media/austin/mightymouse/data/pp8TeV_data/GA2422_default_sys/EJE.root"

# Raw input files with final config
fMB_final="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/INT7.root"
fEMC7_final="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EMC7.root"
fEJE_final="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EJE.root"

# MC input file with systematics config
fMC_sys="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_sys/AnalysisResults.root"
dMC_runwise_sys="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_sys/runwise"

# MC input file with final config
fMC_final="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
dMC_runwise_final="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/runwise"

# Unfolded input file with final config
#fUnfolded_bayes_final="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"
#fUnfolded_svd_final="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/svd_default_final_2423.root"

# Unfolded input file with systematics config
#fUnfolded_bayes_sys="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_sys_2422.root"
#fUnfolded_svd_sys="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/svd_default_sys_2422.root"

# Unfolded input file with final config
fUnfolded_bayes_final="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"
fUnfolded_svd_final="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/svd_default_final_2423.root"

# Unfolded input file with final config
#fUnfolded_bayes_final_newbin="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/ppFiles_NewBinning/output_default/bayes_default_final_2423.root"
#fUnfolded_svd_final_newbin="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/ppFiles_NewBinning/output_default/svd_default_final_2423.root"


# Unfolded input file with systematics config
fUnfolded_bayes_sys="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_sys_2422.root"
fUnfolded_svd_sys="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/svd_default_sys_2422.root"

fUnfolded_bayes_sys_newbin="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/ppFiles_NewBinning/output_default/bayes_default_sys_2422.root"
fUnfolded_svd_sys_newbin="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/ppFiles_NewBinning/output_default/svd_default_sys_2422.root"


# MC Gen input file
fPYTHIA="/media/austin/mightymouse/data/MCGen/PYTHIA/8TeV/PYTHIA_pp_8TeV/ptlimited/AnalysisResults.root"
fPOWHEG="/media/austin/mightymouse/data/MCGen/POWHEG/Scaling_816TeV_cent/8TeV/CT14withnegweight/Pythia8JetSpectra_merged.root"

# LHC16c2 runlist
runsMC="/home/austin/alice/pp8TeV_runlists/runsLHC16c2.txt"

uTypeAna="Bayes"
uTypeSys="Bayes"

dSysRootFiles="/home/austin/alice/SystematicsRootFiles"
dEnergyScaleRootFiles="/home/austin/alice/EnergyScaleRootFiles"
dEnergyScaleRootFiles="/home/austin/alice/EnergyScaleRootFiles"
ResultsRootFile8TeV="/home/austin/alice/FinalResultsRootFiles/FinalResults_pp8TeV.root"
SpectrumRootFile13TeV="/home/austin/alice/FinalResultsRootFiles/jetspectrum13TeV.root"
RatioRootFile13TeV="/home/austin/alice/FinalResultsRootFiles/jetspectrumratios13TeV.root"
HEPData2p76TeV="/home/austin/alice/FinalResultsRootFiles/HEPData_2p76TeV"
HEPData5TeV="/home/austin/alice/FinalResultsRootFiles/HEPData_5TeV"
HEPDataATLAS="/home/austin/alice/FinalResultsRootFiles/HEPData_ATLAS_8TeV"
inputtxtfiles="/home/austin/alice/SpectrumPostProcessing/input_txt_files"

inputtxtfilesNewBin="/home/austin/alice/SpectrumPostProcessing/input_txt_files/ppNewBin"
dSysRootFilesNewBin="/home/austin/alice/SystematicsRootFiles/ppNewBin"
dEnergyScaleRootFilesNewBin="/home/austin/alice/EnergyScaleRootFiles/ppNewBin"
dEnergyScaleRootFilesNewBin="/home/austin/alice/EnergyScaleRootFiles/ppNewBin"
ResultsRootFile8TeVNewBin="/home/austin/alice/FinalResultsRootFiles/FinalResults_NewBinning_pp8TeV.root"

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
#    root -x -q -l -b 'plotSystematicsCorrelated.cxx("'$fUnfolded_bayes_sys'","'$uTypeSys'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#  if [ $r -ne 2 ]
#  then
#    root -x -q -l -b 'plotRatioSystematicsCorrelated.cxx("'$fUnfolded_bayes_sys'","'$uTypeSys'",'$r',"'$filetype'","'$outputdir'","'$dSysRootFiles'","'$inputtxtfiles'")'
#  fi
#  root -x -q -l -b 'plotRejectionFactor.cxx("'$fMB_final'","'$fEMC7_final'","'$fEJE_final'","'$fMC_final'","'$outputdir'",'$r',"'$filetype'","'pp'")'
#  root -x -q -l -b 'plotTriggerSwap.cxx("'$fUnfolded_bayes_final'","'$outputdir'",'$r',"pp","'$filetype'")'
#done

#root -x -q -l -b 'plotRejectionFactor.cxx("'$fMB_final'","'$fEMC7_final'","'$fEJE_final'","'$fMC_final'","'$outputdir'",2,"'$filetype'","'pp'")'


# Make plots for analysis note
#root -x -q -l -b 'plotKinEff.cxx("'$fUnfolded_bayes_final'","'$outputdir'","'$filetype'","pPb")'
#root -x -q -l -b 'plotDVector.cxx("'$fUnfolded_svd_final'","'$outputdir'","'$filetype'","pp")'
#root -x -q -l -b 'plotTriggerBias.cxx("'$fMB_final'","'$fEMC7_final'","'$fEJE_final'","'$fMC_final'","'$outputdir'","'$filetype'","'pPb'",2,2)'
#root -x -q -l -b 'plotTriggerBiasAll.cxx("'$fEJE_final'","'$fEJE_final'","'$fEJE_final'","'$fMC_final'","'$outputdir'","'$filetype'","'pp'",2,5)'
#root -x -q -l -b 'plotTriggerClusters.cxx("'$fMB_sys'","'$fEMC7_sys'","'$fEJE_sys'","'$outputdir'","'$filetype'","'pPb'",2,2)'
#root -x -q -l -b 'plotTriggerClusters.cxx("'$fMB_final'","'$fEMC7_final'","'$fEJE_final'","'$outputdir'","'$filetype'","'pp'",2,5)'
#root -x -q -l -b 'plotCorrRawSpec.cxx("'$fUnfolded_bayes_final'","'$outputdir'","'$filetype'","pp",2,5)'
#root -x -q -l -b 'extractEnergyScaleSlices.cxx("'$fMC_final'","'$outputdir'","'$filetype'",2,2)'
#root -x -q -l -b 'plotTriggerEfficiencies.cxx("'$fUnfolded_bayes_final'","'$outputdir'","'$filetype'","'pp'",2,5,6)'
#root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$fMC_final'","FullJet","default")'
#mv EnergyScaleResults_default.root $dEnergyScaleRootFiles/EnergyScaleResults.root
#root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults.root","'$outputdir'","","'$filetype'",2,6)'
#root -x -q -l -b 'plotJESComp.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults.root","'$dEnergyScaleRootFiles'/EnergyScaleResults_tc200.root","'$outputdir'","'$filetype'")'
#while read run || [[ -n $run ]];
#do
#    rfilerunwise="$rundir/$run/AnalysisResults.root"
#    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$dMC_runwise'/'$run'/AnalysisResults.root","FullJet","nodownscalecorr")'
#    mv EnergyScaleResults_nodownscalecorr.root $dEnergyScaleRootFiles/EnergyScaleResults_$run.root
#    root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles'/EnergyScaleResults_'$run'.root","'$outputdir'","'$run'","'$filetype'")'
#done < $runsMC

# Plot final results and MC comparison
#root -x -q -l -b 'plotFinalSpectrum.cxx("'$fUnfolded_bayes_final'","'$fPYTHIA'","'$fPOWHEG'","'$dSysRootFiles'","'$uTypeAna'","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotEnergyComparison.cxx("'$ResultsRootFile8TeV'","'$HEPData2p76TeV'","'$HEPData5TeV'","'$SpectrumRootFile13TeV'","'$RatioRootFile13TeV'","'$outputdir'","'$filetype'")'
#root -x -q -l -b 'plotExperimentComparison.cxx("'$ResultsRootFile8TeV'","'$HEPDataATLAS'","'$outputdir'","'$filetype'")'
