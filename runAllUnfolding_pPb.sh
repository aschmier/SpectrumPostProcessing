#!/bin/bash

# Train runs
booldefaultbayes=false #
booldefaultsvd=false #
boolbinvar1=false #
boolbinvar2=false #
boolbinvar3=false #
boolbinvar4=false #
boolpriors=false #
boollowerloose=false #
boolupperloose=false #
boollowerstrong=false #
boolupperstrong=false #
bool3x3=false #
bool5x5=false # 
boolMaxTrackPt125=false #
boolMaxTrackPt150=false #
boolMaxTrackPt175=false #
boolMaxTrackPt225=false #
boolMaxClusterE125=true #
boolMaxClusterE150=true #
boolMaxClusterE175=true #
boolMaxClusterE225=true #
boolF07=false # 
boolMIP=false # 
boolS275C75=false # 
boolS350C100=false # 
boolTrackEff=false #
boolLowTriggerSwapLow=false #
boolLowTriggerSwapHigh=false #
boolHighTriggerSwapLow=false #
boolHighTriggerSwapHigh=false #
boolClosureBayes=false #
boolClosureSVD=false #
boolQPtShift=false #
boolEmbedding=false #
boolEJ2LumiUnc=false #
boolEJ1LumiUnc=false #

datatrains="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged"
mctrains="/media/austin/mightymouse/data/pPb8TeV_MC/ptscheme_trains/merged"
priorsfile="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_default.root"

lumilow=67818.6
lumihigh=1391060.0
lumilowUnc=0.979304
lumihighUnc=0.985112

#multiply lumilow by lumilowUnc
lumilowvar=$(echo "$lumilow*$lumilowUnc" | bc -l)
lumihighvar=$(echo "$lumihigh*$lumihighUnc" | bc -l)

swaplow=30
swaphigh=50

inputResponseFile="/home/austin/alice/SpectrumPostProcessing/FilteredResponse/filteredResponse.root"

# default
if $booldefaultbayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","default",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $booldefaultsvd
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","default", '$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

# luminosity
if $boolEJ2LumiUnc
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","lumilow",'$lumilowvar','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolEJ1LumiUnc
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","lumihigh",'$lumilow','$lumihighvar','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

# embedding
if $boolEmbedding
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_Embedding_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","embed",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

# Bin Variation
if $boolbinvar1
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","option1",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolbinvar2
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","option2",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolbinvar3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","option3",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolbinvar4
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","option4",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

# Priors
if $boolpriors
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysPriors_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$priorsfile'",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

# Truncation
if $boollowerloose
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","lowerloose",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boollowerstrong
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","lowerstrong",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolupperloose
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","upperloose",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolupperstrong
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","upperstrong",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

# Clusterizer
if $bool3x3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/3x3/AnalysisResults.root","'$datatrains'/3x3/AnalysisResults.root","'$datatrains'/3x3/AnalysisResults.root","'$mctrains'/3x3/AnalysisResults.root","3x3", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "RequireL0",-1,4,false,true)'
fi

if $bool5x5
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/5x5/AnalysisResults.root","'$datatrains'/5x5/AnalysisResults.root","'$datatrains'/5x5/AnalysisResults.root","'$mctrains'/5x5/AnalysisResults.root","5x5", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "RequireL0",-1,4,false,true)'
fi

# MaxTrackPt
if $boolMaxTrackPt125
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$mctrains'/MaxTrackPt/AnalysisResults.root","MaxTrackPt125", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "MaxTrackPt125",-1,4,false,true)'
fi

if $boolMaxTrackPt150
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$mctrains'/MaxTrackPt/AnalysisResults.root","MaxTrackPt150", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "MaxTrackPt150",-1,4,false,true)'
fi

if $boolMaxTrackPt175
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$mctrains'/MaxTrackPt/AnalysisResults.root","MaxTrackPt175", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "MaxTrackPt175",-1,4,false,true)'
fi

if $boolMaxTrackPt225
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$mctrains'/MaxTrackPt/AnalysisResults.root","MaxTrackPt225", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "MaxTrackPt225",-1,4,false,true)'
fi

# MaxClusterE
if $boolMaxClusterE125
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$mctrains'/MaxClusterE/AnalysisResults.root","MaxClusterE125", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "MaxClusterPt125",-1,4,false,true)'
fi

if $boolMaxClusterE150
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$mctrains'/MaxClusterE/AnalysisResults.root","MaxClusterE150", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "MaxClusterPt150",-1,4,false,true)'
fi

if $boolMaxClusterE175
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$mctrains'/MaxClusterE/AnalysisResults.root","MaxClusterE175", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "MaxClusterPt175",-1,4,false,true)'
fi

if $boolMaxClusterE225
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$mctrains'/MaxClusterE/AnalysisResults.root","MaxClusterE225", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "MaxClusterPt225",-1,4,false,true)'
fi

# Hadronic Correction
if $boolF07
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/F07/AnalysisResults.root","'$datatrains'/F07/AnalysisResults.root","'$datatrains'/F07/AnalysisResults.root","'$mctrains'/F07/AnalysisResults.root","F07", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "RequireL0",-1,4,false,true)'
fi

if $boolMIP
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MIP/AnalysisResults.root","'$datatrains'/MIP/AnalysisResults.root","'$datatrains'/MIP/AnalysisResults.root","'$mctrains'/MIP/AnalysisResults.root","MIP", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "RequireL0",-1,4,false,true)'
fi

# Seed/Cell Energy
if $boolS275C75
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/S275C75/AnalysisResults.root","'$datatrains'/S275C75/AnalysisResults.root","'$datatrains'/S275C75/AnalysisResults.root","'$mctrains'/S275C75/AnalysisResults.root","S275C75", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "RequireL0",-1,4,false,true)'
fi

if $boolS350C100
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/S350C100/AnalysisResults.root","'$datatrains'/S350C100/AnalysisResults.root","'$datatrains'/S350C100/AnalysisResults.root","'$mctrains'/S350C100/AnalysisResults.root","S350C100", '$lumilow', '$lumihigh','$swaplow','$swaphigh', "RequireL0",-1,4,false,true)'
fi

# Tracking
if $boolTrackEff
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/TrackEff/AnalysisResults.root","TrackEff", '$lumilow','$lumihigh','$swaplow','$swaphigh', "RequireL0",-1,4,false,true)'
fi

# Q/pT Shift
if $boolQPtShift
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/QpT/AnalysisResults.root","'$datatrains'/QpT/AnalysisResults.root","'$datatrains'/QpT/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","QpT", '$lumilow', '$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

#########################
## Trigger Systematics ##
#########################

# Trigger Swap
if $boolLowTriggerSwapLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","LowTriggerSwapLow", '$lumilow','$lumihigh',25,'$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolLowTriggerSwapHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","LowTriggerSwapHigh", '$lumilow','$lumihigh',40,'$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolHighTriggerSwapLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","HighTriggerSwapLow", '$lumilow','$lumihigh','$swaplow',40,"RequireL0",-1,4,false,true)'
fi

if $boolHighTriggerSwapHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","HighTriggerSwapHigh", '$lumilow','$lumihigh','$swaplow',60,"RequireL0",-1,4,false,true)'
fi

# Closure
if $boolClosureBayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","closure", '$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi

if $boolClosureSVD
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","closure", '$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,4,false,true)'
fi
