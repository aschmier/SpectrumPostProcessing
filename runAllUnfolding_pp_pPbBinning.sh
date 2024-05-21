#!/bin/bash

# Train runs
booldefaultbayes=false
booldefaultsvd=false
boolbinvar1=false
boolbinvar2=false
boolbinvar3=false
boolbinvar4=false
boolpriors=false
boollowerloose=false
boolupperloose=false
boollowerstrong=false
boolupperstrong=false
bool3x3=false
bool5x5=false
boolMaxTrackPt125=false
boolMaxTrackPt150=false
boolMaxTrackPt175=false
boolMaxTrackPt225=false
boolMaxClusterE125=false
boolMaxClusterE150=false
boolMaxClusterE175=false
boolMaxClusterE225=false
boolF07=false
boolMIP=false
boolS275C75=false
boolS350C100=false
boolTrackEff=false
boolLowTriggerSwapLow=false
boolLowTriggerSwapHigh=false
boolHighTriggerSwapLow=false
boolHighTriggerSwapHigh=false
boolLowRFFitLow=false
boolLowRFFitHigh=false
boolHighRFFitLow=false
boolHighRFFitHigh=false
boolClosureBayes=true
boolClosureSVD=false
boolQpT=false

default=2698
c3x3=2646
c5x5=2647
MaxTrackPt=2661
MaxClusterE=2660
F07=2652
MIP=2655
S275C75=2658
S350C100=2659
QpT=2681

LowRFDefault=69.78
LowRFDefaultError=0.47
HighRFDefault=94.46
HighRFDefaultError=0.78

LowRFLow=69.72
LowRFHigh=70.24
HighRFLow=94.35
HighRFHigh=94.54

ppSwapLow=30
ppSwapHigh=60

datatrains="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/merged"
mctrains="/media/austin/mightymouse/data/pp8TeV_MC/ptscheme_trains/merged"

inputResponseFile="/home/austin/alice/SpectrumPostProcessing/FilteredResponse/filteredResponse.root"
priorsfile="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_default_$default.root"

# Default Bayes
if $booldefaultbayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"default", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

# Default SVD
if $booldefaultsvd
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"default", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

# Bin Variation
if $boolbinvar1
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"option1",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolbinvar2
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"option2",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolbinvar3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"option3",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolbinvar4
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"option4",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

# Priors
if $boolpriors
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysPriors_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"'$priorsfile'",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

# Truncation
if $boollowerloose
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"lowerloose",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boollowerstrong
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"lowerstrong",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolupperloose
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"upperloose",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolupperstrong
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"upperstrong",'$LowRFDefault','$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

# Clusterizer
if $bool3x3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/3x3/AnalysisResults.root","'$datatrains'/3x3/AnalysisResults.root","'$datatrains'/3x3/AnalysisResults.root","'$mctrains'/3x3/AnalysisResults.root",'$c3x3',"3x3", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "default",-1,4,"coarse")'
fi

if $bool5x5
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/5x5/AnalysisResults.root","'$datatrains'/5x5/AnalysisResults.root","'$datatrains'/5x5/AnalysisResults.root","'$mctrains'/5x5/AnalysisResults.root",'$c5x5',"5x5", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "default",-1,4,"coarse")'
fi

# MaxTrackPt
if $boolMaxTrackPt125
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$mctrains'/MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt125", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "MaxTrackPt125",-1,4,"coarse")'
fi

if $boolMaxTrackPt150
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$mctrains'/MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt150", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "MaxTrackPt150",-1,4,"coarse")'
fi

if $boolMaxTrackPt175
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$mctrains'/MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt175", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "MaxTrackPt175",-1,4,"coarse")'
fi

if $boolMaxTrackPt225
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$datatrains'/MaxTrackPt/AnalysisResults.root","'$mctrains'/MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt225", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "MaxTrackPt225",-1,4,"coarse")'
fi

# MaxClusterE
if $boolMaxClusterE125
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$mctrains'/MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE125", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "MaxClusterPt125",-1,4,"coarse")'
fi

if $boolMaxClusterE150
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$mctrains'/MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE150", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "MaxClusterPt150",-1,4,"coarse")'
fi

if $boolMaxClusterE175
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$mctrains'/MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE175", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "MaxClusterPt175",-1,4,"coarse")'
fi

if $boolMaxClusterE225
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$datatrains'/MaxClusterE/AnalysisResults.root","'$mctrains'/MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE225", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "MaxClusterPt225",-1,4,"coarse")'
fi

# Hadronic Correction
if $boolF07
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/F07/AnalysisResults.root","'$datatrains'/F07/AnalysisResults.root","'$datatrains'/F07/AnalysisResults.root","'$mctrains'/F07/AnalysisResults.root",'$F07',"F07", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "default",-1,4,"coarse")'
fi

if $boolMIP
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/MIP/AnalysisResults.root","'$datatrains'/MIP/AnalysisResults.root","'$datatrains'/MIP/AnalysisResults.root","'$mctrains'/MIP/AnalysisResults.root",'$MIP',"MIP", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "default",-1,4,"coarse")'
fi

# Seed/Cell Energy
if $boolS275C75
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/S275C75/AnalysisResults.root","'$datatrains'/S275C75/AnalysisResults.root","'$datatrains'/S275C75/AnalysisResults.root","'$mctrains'/S275C75/AnalysisResults.root",'$S275C75',"S275C75", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "default",-1,4,"coarse")'
fi

if $boolS350C100
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/S350C100/AnalysisResults.root","'$datatrains'/S350C100/AnalysisResults.root","'$datatrains'/S350C100/AnalysisResults.root","'$mctrains'/S350C100/AnalysisResults.root",'$S350C100',"S350C100", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "default",-1,4,"coarse")'
fi

# Tracking
if $boolTrackEff
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/TrackEff/AnalysisResults.root",'$default',"TrackEff", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh', "default",-1,4,"coarse")'
fi

# Q/pT Shift
if $boolQpT
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/QpT/AnalysisResults.root","'$datatrains'/QpT/AnalysisResults.root","'$datatrains'/QpT/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$QpT',"QpT", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

#########################
## Trigger Systematics ##
#########################

# Trigger Swap
if $boolLowTriggerSwapLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"LowTriggerSwapLow", '$LowRFDefault', '$HighRFDefault',25,'$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolLowTriggerSwapHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"LowTriggerSwapHigh", '$LowRFDefault', '$HighRFDefault',35,'$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolHighTriggerSwapLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"HighTriggerSwapLow", '$LowRFDefault', '$HighRFDefault','$ppSwapLow',50,"default",-1,4,"coarse")'
fi

if $boolHighTriggerSwapHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"HighTriggerSwapHigh", '$LowRFDefault', '$HighRFDefault','$ppSwapLow',70,"default",-1,4,"coarse")'
fi

##

if $boolLowRFFitLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"LowRFFitLow", '$LowRFLow', '$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolLowRFFitHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"LowRFFitHigh", '$LowRFHigh', '$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolHighRFFitLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"HighRFFitLow", '$LowRFDefault', '$HighRFLow','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolHighRFFitHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$datatrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"HighRFFitHigh", '$LowRFDefault', '$HighRFHigh','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

# Closure
if $boolClosureBayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"closure", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi

if $boolClosureSVD
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root","'$mctrains'/default/AnalysisResults.root",'$default',"closure", '$LowRFDefault', '$HighRFDefault','$ppSwapLow','$ppSwapHigh',"default",-1,4,"coarse")'
fi