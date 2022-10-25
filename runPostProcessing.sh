#!/bin/bash

# exit when any command fails
set -e

# keep track of the last executed command
trap 'last_command=$current_command; current_command=$BASH_COMMAND' DEBUG
# echo an error message before exiting
trap 'echo "\"${last_command}\" command filed with exit code $?."' EXIT


outputdir="/home/austin/alice/AnalysisNote_pp_8TeV/618164481744be1fd5ef7dfe/figures"
filetype="pdf"

# Fine binned cluster raw input files with old config.
fMB_finebin_old="/home/austin/alice/data/pp8TeV_data/GA2296_default_newbinning/INT7.root"
fEMC7_finebin_old="/home/austin/alice/data/pp8TeV_data/GA2296_default_newbinning/EMC7.root"
fEJE_finebin_old="/home/austin/alice/data/pp8TeV_data/GA2296_default_newbinning/EJE.root"

# Course binned cluster raw input files with old config
fMB_coursebin_old="/home/austin/alice/data/pp8TeV_data/GA2284_default/INT7.root"
fEMC7_coursebin_old="/home/austin/alice/data/pp8TeV_data/GA2284_default/EMC7.root"
fEJE_coursebin_old="/home/austin/alice/data/pp8TeV_data/GA2284_default/EJE.root"

# Course binned cluster raw input files with correct config
fMB_coursebin="/home/austin/alice/data/pp8TeV_data/GA2294_default_final/INT7.root"
fEMC7_coursebin="/home/austin/alice/data/pp8TeV_data/GA2294_default_final/EMC7.root"
fEJE_coursebin="/home/austin/alice/data/pp8TeV_data/GA2294_default_final/EJE.root"

# MC input file with old config
fMC_old="/home/austin/alice/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root"
dMC_runwise_old="/home/austin/alice/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/runwise"

# MC input file with correct config
fMC="/home/austin/alice/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
dMC_runwise="/home/austin/alice/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/runwise"

# Unfolded input file with correct config
fUnfolded_bayes="/home/austin/alice/unfoldingResults/output_default/bayes_default_final_2294.root"
fUnfolded_svd="/home/austin/alice/unfoldingResults/output_default/svd_default_final_2294.root"

# Unfolded input file with old config
fUnfolded_bayes_old="/home/austin/alice/unfoldingResults/output_default/bayes_default_2284.root"
fUnfolded_svd_old="/home/austin/alice/unfoldingResults/output_default/svd_default_2284.root"

# MC Gen input file
fMCGen="/home/austin/alice/data/pp8TeV_MC/PureMC_8TeV/AnalysisResults.root"

# LHC16c2 runlist
runsMC="/home/austin/alice/data/pp8TeV_runlists/runsLHC16c2.txt"

uTypeAna="SVD"
uTypeSys="Bayes"

dSysRootFiles="/home/austin/alice/SystematicsRootFiles"
dEnergyScaleRootFiles="/home/austin/alice/EnergyScaleRootFiles"

mkdir -p $outputdir

# Do unfolding comparisons
python /home/austin/alice/SubstructureAnalysis/unfolding/comparisons/comp1D/8TeV/makeallplots_8TeV.py $fUnfolded_svd $fUnfolded_bayes $outputdir $filetype
root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_svd'","SVD","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotUnfVsRaw.cxx("'$fUnfolded_bayes'","Bayes","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_svd'","SVD","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotCompareIter.cxx("'$fUnfolded_bayes'","Bayes","'$outputdir'","'$filetype'")'

# Run systematics and plot rejection factor
for (( r=2; r<=6; r++ ))
do
  root -x -q -l -b 'plotSystematics.cxx("'$fUnfolded_bayes_old'","'$uTypeSys'",'$r',"'$filetype'","'$outputdir'")'
  root -x -q -l -b 'plotRejectionFactor.cxx("'$fMB_finebin_old'","'$fEMC7_finebin_old'","'$fEJE_finebin_old'","'$outputdir'",'$r',"'$filetype'")'
done

# Make plots for analysis note
root -x -q -l -b 'plotKinEff.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotDVector.cxx("'$fUnfolded_svd'","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotNEFComp.cxx("'$fMB_coursebin'","'$fEMC7_coursebin'","'$fEJE_coursebin'","'$fMC'","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotTriggerClusters.cxx("'$fMB_finebin_old'","'$fEMC7_finebin_old'","'$fEJE_finebin_old'","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotCorrRawSpec.cxx("'$fUnfolded_bayes'","'$outputdir'","'$filetype'")'
root -x -q -l -b 'extractEnergyScaleSlices.cxx("'$fMC'","'$outputdir'","'$filetype'")'
root -x -q -l -b 'plotTriggerEfficiencies.cxx("'$fMC'","'$outputdir'","'$filetype'")'
root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$fMC'","FullJet","nodownscalecorr")'
mv EnergyScaleResults_nodownscalecorr.root $dEnergyScaleRootFiles/EnergyScaleResults.root
root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles/EnergyScaleResults.root'","'$outputdir'","","'$filetype'")'
root -x -q -l -b 'plotJESComp.cxx("'$dEnergyScaleRootFiles/EnergyScaleResults.root'","'$dEnergyScaleRootFiles/EnergyScaleResults_tc200.root'","'$outputdir'","","'$filetype'")'
while read run || [[ -n $run ]];
do
    rfilerunwise="$rundir/$run/AnalysisResults.root"
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/EnergyScale/EnergyScaleTask/extractJetEnergyScaleSimple.cpp("'$dMC_runwise/$run/AnalysisResults.root'","FullJet","nodownscalecorr")'
    mv EnergyScaleResults_nodownscalecorr.root $dEnergyScaleRootFiles/EnergyScaleResults_$run.root
    root -x -q -l -b 'plotJES.cxx("'$dEnergyScaleRootFiles/EnergyScaleResults_$run.root'","'$outputdir'","'$run'","'$filetype'")'
done < $runsMC

# Plot final results and MC comparison
root -x -q -l -b 'plotFinalSpectrum.cxx("'$fUnfolded_svd'","'$fMCGen'","'$dSysRootFiles'","'$uTypeAna'","'$outputdir'","'$filetype'")'
