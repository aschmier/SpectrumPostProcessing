#!/bin/bash

ppINT7="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/INT7.root"
ppEMC7="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EMC7.root"
ppEJE="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EJE.root"

pAINT7="/media/austin/mightymouse/data/pPb8TeV_data/GA951_default/AnalysisResults.root"
pAEJ2="/media/austin/mightymouse/data/pPb8TeV_data/GA951_default/AnalysisResults.root"
pAEJ1="/media/austin/mightymouse/data/pPb8TeV_data/GA951_default/AnalysisResults.root"

ppMC="/media/austin/mightymouse/data/pp8TeV_MC/mergedGA_LHC16c2/LHC16c2_default_final/AnalysisResults.root"
pAMCHF2="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1488/AnalysisResults.root"
pAMCHF="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1471/AnalysisResults.root"
pAMCEC="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1449/AnalysisResults.root"
pAMCNR="/media/austin/mightymouse/data/pPb8TeV_MC/merged_1461/AnalysisResults.root"

arrPAMC=("'$pAMCHF'","'$pAMCEC'","'$pAMCNR'")

outHF2="/home/austin/alice/SpectrumPostProcessing/RpADebug/HeaderFilter2"
outHF="/home/austin/alice/SpectrumPostProcessing/RpADebug/HeaderFilter"
outEC="/home/austin/alice/SpectrumPostProcessing/RpADebug/EPOSCorrected"
outNR="/home/austin/alice/SpectrumPostProcessing/RpADebug/NoRejection"
outRC="/home/austin/alice/SpectrumPostProcessing/RpADebug/RejectionComparison"

[ -d $outHF ] || mkdir $outHF
[ -d $outHF2 ] || mkdir $outHF2
[ -d $outEC ] || mkdir $outEC
[ -d $outNR ] || mkdir $outNR
[ -d $outRC ] || mkdir $outRC

root -x -q -l -b 'plotRpADebug.cxx("'$ppINT7'","'$ppEMC7'","'$ppEJE'","'$ppMC'","'$pAINT7'","'$pAEJ2'","'$pAEJ1'","'$pAMCHF2'","'$outHF2'")'
#root -x -q -l -b 'plotRpADebug.cxx("'$ppINT7'","'$ppEMC7'","'$ppEJE'","'$ppMC'","'$pAINT7'","'$pAEJ2'","'$pAEJ1'","'$pAMCHF'","'$outHF'")'
#root -x -q -l -b 'plotRpADebug.cxx("'$ppINT7'","'$ppEMC7'","'$ppEJE'","'$ppMC'","'$pAINT7'","'$pAEJ2'","'$pAEJ1'","'$pAMCEC'","'$outEC'")'
root -x -q -l -b 'plotRpADebug.cxx("'$ppINT7'","'$ppEMC7'","'$ppEJE'","'$ppMC'","'$pAINT7'","'$pAEJ2'","'$pAEJ1'","'$pAMCNR'","'$outNR'")'

#root -x -q -l -b 'plotRpARejComp.cxx("'$ppMC'","'$pAMCHF2'","'$pAMCHF'","'$pAMCEC'","'$pAMCNR'","'$outRC'")'
