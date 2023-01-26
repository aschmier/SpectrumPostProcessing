#!/bin/bash

# Train runs
booldefaultbayes=false
booldefaultsvd=false
booldefaultbayesfinal=true
booldefaultsvdfinal=false
boolbinvar1=false
boolbinvar2=false
boolbinvar3=false
boolbinvar4=false
boolpriors=false
boollowerloose=false
boolupperloose=false
boollowerstrong=false
boolupperstrong=false
boolclus3=false
boolclus5=false
boolMaxTrackPt125=false
boolMaxTrackPt150=false
boolMaxTrackPt175=false
boolMaxTrackPt200=false
boolMaxTrackPt225=false
boolMaxClusterE125=false
boolMaxClusterE150=false
boolMaxClusterE175=false
boolMaxClusterE200=false
boolMaxClusterE225=false
boolF07=false
boolMIP=false
boolseed275=false
boolseed350=false
boolTracking=false
boolLowTriggerSwapLow=false
boolLowTriggerSwapHigh=false
boolHighTriggerSwapLow=false
boolHighTriggerSwapHigh=false
boolLowRFBinVarOption1=false
boolLowRFBinVarOption2=false
boolLowRFBinVarOption3=false
boolLowRFBinVarOption4=false
boolHighRFBinVarOption1=false
boolHighRFBinVarOption2=false
boolHighRFBinVarOption3=false
boolHighRFBinVarOption4=false
boolLowRFFitLow=false
boolLowRFFitHigh=false
boolHighRFFitLow=false
boolHighRFFitHigh=false
boolGetRFDefaultBayes=false
boolGetRFDefaultSVD=false
boolClosureBayes=false
boolClosureSVD=false

boolpPb=true

default=2422
defaultfinal=2423
defaultnewbin=2296
clus3=2246
clus5=2245
MaxTrackPt=2386
MaxClusterE=2385
F07=2256
MIP=2257
seed275=2230
seed350=2231

pPb=951

ppRFLow=65.78
ppRFLowError=0.90
ppRFHigh=94.37
ppRFHighError=2.00
ppSwapLow=30
ppSwapHigh=60

# Default
if $booldefaultsvd
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"default", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

if $booldefaultbayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default_sys/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default_sys/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default_sys/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_sys/AnalysisResults.root",'$default',"default", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh',"default")'
fi

# Default final (correct emcal corrections)
if $booldefaultbayesfinal
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultfinal'_default_final/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultfinal'_default_final/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultfinal'_default_final/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root",'$defaultfinal',"default", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh',"default")'
fi

if $booldefaultsvdfinal
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultfinal'_default_final/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultfinal'_default_final/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultfinal'_default_final/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root",'$defaultfinal',"default", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

# Bin Variation
if $boolbinvar1
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"option1",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

if $boolbinvar2
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"option2",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

if $boolbinvar3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"option3",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

if $boolbinvar4
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysBinVar_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"option4",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

# Priors
if $boolpriors
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysPriors_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_2284.root",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

# Truncation
if $boollowerloose
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"lowerloose",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

if $boollowerstrong
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"lowerstrong",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

if $boolupperloose
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"upperloose",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

if $boolupperstrong
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTruncation_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$default',"upperstrong",'$ppRFLow','$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

# Clusterizer
if $boolclus3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$clus3'_3x3/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$clus3'_3x3/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$clus3'_3x3/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_3x3/AnalysisResults.root",'$clus3',"3x3", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "nodownscalecorr")'
fi

if $boolclus5
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$clus5'_5x5/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$clus5'_5x5/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$clus5'_5x5/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_5x5/AnalysisResults.root",'$clus5',"5x5", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "nodownscalecorr")'
fi

# MaxTrackPt
if $boolMaxTrackPt125
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt125", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxTrackPt125")'
fi

if $boolMaxTrackPt150
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt150", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxTrackPt150")'
fi

if $boolMaxTrackPt175
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt175", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxTrackPt175")'
fi

if $boolMaxTrackPt200
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt200", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxTrackPt200")'
fi

if $boolMaxTrackPt225
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxTrackPt'_MaxTrackPt/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxTrackPt/AnalysisResults.root",'$MaxTrackPt',"MaxTrackPt225", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxTrackPt225")'
fi

# MaxClusterE
if $boolMaxClusterE125
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE125", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxClusterPt125")'
fi

if $boolMaxClusterE150
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE150", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxClusterPt150")'
fi

if $boolMaxClusterE175
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE175", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxClusterPt175")'
fi

if $boolMaxClusterE200
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE200", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxClusterPt200")'
fi

if $boolMaxClusterE225
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MaxClusterE'_MaxClusterE/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MaxClusterE/AnalysisResults.root",'$MaxClusterE',"MaxClusterE225", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "MaxClusterPt225")'
fi

# Hadronic Correction
if $boolF07
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$F07'_F07/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$F07'_F07/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$F07'_F07/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_F07/AnalysisResults.root",'$F07',"F07", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "nodownscalecorr")'
fi

if $boolMIP
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$MIP'_MIP/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MIP'_MIP/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$MIP'_MIP/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_MIP/AnalysisResults.root",'$MIP',"MIP", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "nodownscalecorr")'
fi

# Seed/Cell Energy
if $boolseed275
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$seed275'_Seed275/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$seed275'_Seed275/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$seed275'_Seed275/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_Seed275/AnalysisResults.root",'$seed275',"Seed275", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "nodownscalecorr")'
fi

if $boolseed350
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$seed350'_Seed350/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$seed350'_Seed350/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$seed350'_Seed350/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_Seed350/AnalysisResults.root",'$seed350',"Seed350", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "nodownscalecorr")'
fi

# Tracking
if $boolTracking
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$default'_default/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_Tracking/AnalysisResults.root",'$default',"Tracking", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh', "nodownscalecorr")'
fi

#########################
## Trigger Systematics ##
#########################

# Trigger Swap
if $boolLowTriggerSwapLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"LowTriggerSwapLow","nodownscalecorr",25.,60.)'
fi

if $boolLowTriggerSwapHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"LowTriggerSwapHigh","nodownscalecorr",35.,60.)'
fi

if $boolHighTriggerSwapLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"HighTriggerSwapLow","nodownscalecorr",30.,55.)'
fi

if $boolHighTriggerSwapHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"HighTriggerSwapHigh","nodownscalecorr",30.,65.)'
fi

# RFVar
if $boolLowRFBinVarOption1
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"LowRFBinVarOption1","nodownscalecorr",30.,60.,1.5,6.,"option1","default")'
fi

if $boolLowRFBinVarOption2
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"LowRFBinVarOption2","nodownscalecorr",30.,60.,1.5,6.,"option2","default")'
fi

if $boolLowRFBinVarOption3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"LowRFBinVarOption3","nodownscalecorr",30.,60.,1.5,6.,"option3","default")'
fi

if $boolLowRFBinVarOption4
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"LowRFBinVarOption4","nodownscalecorr",30.,60.,1.5,6.,"option4","default")'
fi

if $boolHighRFBinVarOption1
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"HighRFBinVarOption1","nodownscalecorr",30.,60.,1.5,6.,"default","option1")'
fi

if $boolHighRFBinVarOption2
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"HighRFBinVarOption2","nodownscalecorr",30.,60.,1.5,6.,"default","option2")'
fi

if $boolHighRFBinVarOption3
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"HighRFBinVarOption3","nodownscalecorr",30.,60.,1.5,6.,"default","option3")'
fi

if $boolHighRFBinVarOption4
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"HighRFBinVarOption4","nodownscalecorr",30.,60.,1.5,6.,"default","option4")'
fi

if $boolLowRFFitLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"LowRFFitLow","nodownscalecorr",30.,60.,1.,6.)'
fi

if $boolLowRFFitHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"LowRFFitHigh","nodownscalecorr",30.,60.,2.,6.)'
fi

if $boolHighRFFitLow
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"HighRFFitLow","nodownscalecorr",30.,60.,1.5,5.5)'
fi

if $boolHighRFFitHigh
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"HighRFFitHigh","nodownscalecorr",30.,60.,1.5,6.5)'
fi

# Get rejection factors (run Trigger Sys macro with all default settings)
if $boolGetRFDefaultSVD
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"TriggerRFDefault","nodownscalecorr")'
fi

if $boolGetRFDefaultBayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_SysTrigger_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/INT7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EMC7.root","/media/austin/mightymouse/data/pp8TeV_data/GA'$defaultnewbin'_default_newbinning/EJE.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default/AnalysisResults.root",'$defaultnewbin',"TriggerRFDefault","nodownscalecorr")'
fi

# pPb
if $boolpPb
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb.cpp("/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_MC/merged_1449/AnalysisResults.root","EPOSCorrected","nodownscalecorr",2)'
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb.cpp("/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_MC/merged_1461/AnalysisResults.root","NoRejection","nodownscalecorr",2)'
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb.cpp("/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_MC/merged_1471/AnalysisResults.root","HeaderFilter","nodownscalecorr",2)'
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_pPb.cpp("/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_data/GA'$pPb'_default/AnalysisResults.root","/media/austin/mightymouse/data/pPb8TeV_MC/merged_1476/AnalysisResults.root","HeaderFilter2","nodownscalecorr",2)'
fi

# Closure
if $boolClosureBayes
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/Bayes/runCorrectionChain1DBayes_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root",'$defaultfinal',"closure", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi

if $boolClosureSVD
then
    root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/unfolding/1D/SVD/runCorrectionChain1DSVD_SpectrumTaskSimplePoor_CorrectEffPure_8TeV.cpp("/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root","/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root",'$defaultfinal',"closure", '$ppRFLow', '$ppRFHigh','$ppSwapLow','$ppSwapHigh',"nodownscalecorr")'
fi
