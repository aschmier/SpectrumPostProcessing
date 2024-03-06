#!/bin/bash

# Train runs
booldefaultnew=true #
booldefaultbayes=false #
boolptscheme=false #
booldefaultbayessys=false #
booldefaultsvd=false #
booldefaultsvdsys=false #
boolbinvar1=false #
boolbinvar2=false #
boolbinvar3=false #
boolbinvar4=false #
boolpriors=false #
boollowerloose=false #
boolupperloose=false #
boollowerstrong=false #
boolupperstrong=false #
boolclus3=false
boolclus5=false # incorrect response name
boolMaxTrackPt125=false #
boolMaxTrackPt150=false #
boolMaxTrackPt175=false #
boolMaxTrackPt225=false #
boolMaxClusterE125=false #
boolMaxClusterE150=false #
boolMaxClusterE175=false #
boolMaxClusterE225=false #
boolF07=false # incorrect response name
boolMIP=false # incorrect response name
boolS275C75=false # incorrect response name
boolS350C100=false # incorrect response name
boolTracking=false #
boolLowTriggerSwapLow=false #
boolLowTriggerSwapHigh=false #
boolHighTriggerSwapLow=false #
boolHighTriggerSwapHigh=false #
boolClosureBayes=false #
boolClosureSVD=false #
boolQPtShift=false
boolEmbedding=false #
boolEJ2LumiUnc=false
boolEJ1LumiUnc=false
boorequirel0=false

datatrains="/media/austin/mightymouse/data/pPb8TeV_data/merged"
ptschemetrains="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/merged"
mctrains="/media/austin/mightymouse/data/pPb8TeV_MC/merged"
mcptschemetrains="/media/austin/mightymouse/data/pPb8TeV_MC/ptscheme_trains/merged"
priorsfile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_defaultsys.root"
lumilow=67523.2
lumilowptscheme=65079.2
lumihigh=1379980
lumihighptscheme=1335920
lumilowrho02=68477.8
lumihighrho02=1402380

lumilowUnc=0.927477
lumihighUnc=0.985066

lumilowskim=77451.9
lumihighskim=1403760

#multiply lumilowskim by lumilowUnc
lumilowvar=$(echo "$lumilowskim*$lumilowUnc" | bc -l)
lumihighvar=$(echo "$lumihighskim*$lumihighUnc" | bc -l)

swaplow=30
swaphigh=50

inputResponseFile="/home/austin/alice/SpectrumPostProcessing/FilteredResponse/filteredResponse.root"

# New default
if $booldefaultnew
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$ptschemetrains'/default/EJ1.root","'$ptschemetrains'/default/EJ1.root","'$ptschemetrains'/default/EJ1.root","'$mcptschemetrains'/default/AnalysisResults.root","default",'$lumilowrho02','$lumihighrho02','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# default
if $booldefaultbayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default_lumi/EJ1.root","'$datatrains'/default_lumi/EJ1.root","'$datatrains'/default_lumi/EJ1.root","'$mctrains'/ESchemeFix/AnalysisResults.root","runlistEJ1",'$lumilow','$lumihigh','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# ptscheme
if $boolptscheme
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/PtScheme/INT7.root","'$datatrains'/PtScheme/EJ2.root","'$datatrains'/PtScheme/EJ1.root","'$mctrains'/PtScheme/AnalysisResults.root","ptscheme",'$lumilowptscheme','$lumihighptscheme','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# RequireL0
#if $boolrequirel0
#then
#    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/INT7.root","'$datatrains'/default/EJ1.root","'$datatrains'/default/EJ1.root","'$mctrains'/RequireL0/AnalysisResults.root","RequireL0",'$lumilow','$lumihigh','$swaplow','$swaphigh',"RequireL0",-1,false,true)'
#fi

# defaultsys (defaultskim)
if $booldefaultbayessys
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","defaultsys",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $booldefaultsvd
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/default/INT7.root","'$datatrains'/default/EJ2.root","'$datatrains'/default/EJ1.root","'$mctrains'/default/AnalysisResults.root","default", '$lumilow','$lumihigh','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $booldefaultsvdsys
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","defaultsys", '$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# luminosity
if $boolEJ2LumiUnc
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","lumilow",'$lumilowvar','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $boolEJ1LumiUnc
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","lumihigh",'$lumilowskim','$lumihighvar','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# embedding
if $boolEmbedding
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_Embedding_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","embed",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# Bin Variation
if $boolbinvar1
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","option1",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $boolbinvar2
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","option2",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $boolbinvar3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","option3",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $boolbinvar4
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","option4",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# Priors
if $boolpriors
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysPriors_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","'$priorsfile'",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# Truncation
if $boollowerloose
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","lowerloose",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $boollowerstrong
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","lowerstrong",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $boolupperloose
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","upperloose",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $boolupperstrong
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","upperstrong",'$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

# Clusterizer
if $boolclus3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/3x3/INT7.root","'$datatrains'/3x3/EJ2.root","'$datatrains'/3x3/EJ1.root","'$mctrains'/3x3/AnalysisResults.root","3x3", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "default",-1,false,true)'
fi

if $boolclus5
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/5x5/INT7.root","'$datatrains'/5x5/EJ2.root","'$datatrains'/5x5/EJ1.root","'$mctrains'/5x5/AnalysisResults.root","5x5", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "default",-1,false,true)'
fi

# MaxTrackPt
if $boolMaxTrackPt125
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxTrackPt/INT7.root","'$datatrains'/MaxTrackPt/EJ2.root","'$datatrains'/MaxTrackPt/EJ1.root","'$mctrains'/MaxTrackPt/AnalysisResults.root","MaxTrackPt125", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "MaxTrackPt125",-1,false,true)'
fi

if $boolMaxTrackPt150
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxTrackPt/INT7.root","'$datatrains'/MaxTrackPt/EJ2.root","'$datatrains'/MaxTrackPt/EJ1.root","'$mctrains'/MaxTrackPt/AnalysisResults.root","MaxTrackPt150", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "MaxTrackPt150",-1,false,true)'
fi

if $boolMaxTrackPt175
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxTrackPt/INT7.root","'$datatrains'/MaxTrackPt/EJ2.root","'$datatrains'/MaxTrackPt/EJ1.root","'$mctrains'/MaxTrackPt/AnalysisResults.root","MaxTrackPt175", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "MaxTrackPt175",-1,false,true)'
fi

if $boolMaxTrackPt225
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxTrackPt/INT7.root","'$datatrains'/MaxTrackPt/EJ2.root","'$datatrains'/MaxTrackPt/EJ1.root","'$mctrains'/MaxTrackPt/AnalysisResults.root","MaxTrackPt225", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "MaxTrackPt225",-1,false,true)'
fi

# MaxClusterE
if $boolMaxClusterE125
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxClusterE/INT7.root","'$datatrains'/MaxClusterE/EJ2.root","'$datatrains'/MaxClusterE/EJ1.root","'$mctrains'/MaxClusterE/AnalysisResults.root","MaxClusterE125", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "MaxClusterPt125",-1,false,true)'
fi

if $boolMaxClusterE150
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxClusterE/INT7.root","'$datatrains'/MaxClusterE/EJ2.root","'$datatrains'/MaxClusterE/EJ1.root","'$mctrains'/MaxClusterE/AnalysisResults.root","MaxClusterE150", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "MaxClusterPt150",-1,false,true)'
fi

if $boolMaxClusterE175
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxClusterE/INT7.root","'$datatrains'/MaxClusterE/EJ2.root","'$datatrains'/MaxClusterE/EJ1.root","'$mctrains'/MaxClusterE/AnalysisResults.root","MaxClusterE175", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "MaxClusterPt175",-1,false,true)'
fi

if $boolMaxClusterE225
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MaxClusterE/INT7.root","'$datatrains'/MaxClusterE/EJ2.root","'$datatrains'/MaxClusterE/EJ1.root","'$mctrains'/MaxClusterE/AnalysisResults.root","MaxClusterE225", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "MaxClusterPt225",-1,false,true)'
fi

# Hadronic Correction
if $boolF07
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/F07/INT7.root","'$datatrains'/F07/EJ2.root","'$datatrains'/F07/EJ1.root","'$mctrains'/F07/AnalysisResults.root","F07", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "default",-1,false,true)'
fi

if $boolMIP
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/MIP/INT7.root","'$datatrains'/MIP/EJ2.root","'$datatrains'/MIP/EJ1.root","'$mctrains'/MIP/AnalysisResults.root","MIP", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "default",-1,false,true)'
fi

# Seed/Cell Energy
if $boolS275C75
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/S275C75/INT7.root","'$datatrains'/S275C75/EJ2.root","'$datatrains'/S275C75/EJ1.root","'$mctrains'/S275C75/AnalysisResults.root","S275C75", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "default",-1,false,true)'
fi

if $boolS350C100
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/S350C100/INT7.root","'$datatrains'/S350C100/EJ2.root","'$datatrains'/S350C100/EJ1.root","'$mctrains'/S350C100/AnalysisResults.root","S350C100", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh', "default",-1,false,true)'
fi

# Tracking
if $boolTracking
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/tracking/AnalysisResults.root","Tracking", '$lumilowskim','$lumihighskim','$swaplow','$swaphigh', "default",-1,false,true)'
fi

# Q/pT Shift
if $boolQPtShift
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/QoverPtShift/INT7.root","'$datatrains'/QoverPtShift/EJ2.root","'$datatrains'/QoverPtShift/EJ1.root","'$mctrains'/default/AnalysisResults.root","QoverPtShift", '$lumilowskim', '$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

#########################
## Trigger Systematics ##
#########################

# Trigger Swap
if $boolLowTriggerSwapLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","LowTriggerSwapLow", '$lumilowskim','$lumihighskim',25,'$swaphigh',"default",-1,false,true)'
fi

if $boolLowTriggerSwapHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","LowTriggerSwapHigh", '$lumilowskim','$lumihighskim',40,'$swaphigh',"default",-1,false,true)'
fi

if $boolHighTriggerSwapLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","HighTriggerSwapLow", '$lumilowskim','$lumihighskim','$swaplow',40,"default",-1,false,true)'
fi

if $boolHighTriggerSwapHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$datatrains'/defaultskim/INT7.root","'$datatrains'/defaultskim/EJ2.root","'$datatrains'/defaultskim/EJ1.root","'$mctrains'/default/AnalysisResults.root","HighTriggerSwapHigh", '$lumilowskim','$lumihighskim','$swaplow',60,"default",-1,false,true)'
fi

# Closure
if $boolClosureBayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","closure", '$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi

if $boolClosureSVD
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_pPb_Lumi.cpp("'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","closure", '$lumilowskim','$lumihighskim','$swaplow','$swaphigh',"default",-1,false,true)'
fi
