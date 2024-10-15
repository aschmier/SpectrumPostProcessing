#!/bin/bash

ppRootFile_NoBG="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp_pPbBinning/output_default/bayes_default_2702.root"
ppRootFile_Full="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp_pPbBinning/output_default/bayes_FullRho_BGSub_2798.root"
ppRootFile_Charged="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp_pPbBinning/output_default/bayes_DefaultScaleVal_BGSub_2785.root"

pPbRootFile_Full="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_default.root"
pPbRootFile_Charged="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_defaultchargedrho.root"

output="/home/austin/alice/PaperPlots/RpPb"

fileType="png"
minradius=2
maxradius=4
scaleFactorRootFile="/home/austin/alice/SpectrumPostProcessing/PythiaGenScaleFactorRoot/scaleFactorPythia.root"
dSysRootFiles="/home/austin/alice/SystematicsRootFiles/RpPb"

[ -d $output ] || mkdir $output

#root -x -q -l -b 'constructRpPb.cxx("'$ppRootFile'", "'$pPbRootFile'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$output'", "'$fileType'", '$minradius', '$maxradius')'

#root -x -q -l -b 'constructRpPb_3Panel.cxx("'$ppRootFile'", "'$pPbRootFile'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$output'", "'$fileType'", '$minradius', '$maxradius')'

root -x -q -l -b 'constructRpPb_3Panel.cxx("'$ppRootFile_NoBG'", "'$pPbRootFile_Full'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$output/ppNoBG_pPbFull'", "'$fileType'", '$minradius', '$maxradius')'

root -x -q -l -b 'constructRpPb_3Panel.cxx("'$ppRootFile_NoBG'", "'$pPbRootFile_Charged'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$output/ppNoBG_pPbCharged'", "'$fileType'", '$minradius', '$maxradius')'

root -x -q -l -b 'constructRpPb_3Panel.cxx("'$ppRootFile_Full'", "'$pPbRootFile_Full'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$output/ppFull_pPbFull'", "'$fileType'", '$minradius', '$maxradius')'

root -x -q -l -b 'constructRpPb_3Panel.cxx("'$ppRootFile_Charged'", "'$pPbRootFile_Charged'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$output/ppCharged_pPbCharged'", "'$fileType'", '$minradius', '$maxradius')'