#!/bin/bash

ppRootFile_NoBG="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp_pPbBinning/output_default/bayes_default_2702.root"
ppRootFile_Full="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp_pPbBinning/output_default/bayes_FullRho_BGSub_2798.root"
ppRootFile_Charged="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp_pPbBinning/output_default/bayes_DefaultScaleVal_BGSub_2785.root"

pPbRootFile_Full="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_default.root"
pPbRootFile_Charged="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_defaultchargedrho.root"

#output="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures"
output="/home/austin/alice/PaperPlots/RpPb"


fileType="png"
minradius=2
maxradius=4
scaleFactorRootFile="/home/austin/alice/SpectrumPostProcessing/PythiaGenScaleFactorRoot/scaleFactorPythia.root"
dSysRootFiles="/home/austin/alice/SystematicsRootFiles/RpPb"
atlasRpA="/home/austin/alice/RpPb_ExperimentData/RpPb_ATLAS.tsv"
cmsRpA="/home/austin/alice/RpPb_ExperimentData/RpPb_CMS.tsv"
phnxRpA="/home/austin/alice/RpPb_ExperimentData/RpPb_PHENIX.tsv"
aliceRpA02="/home/austin/alice/RpPb_ExperimentData/RpPb_ALICEchj_R02.tsv"
aliceRpA03="/home/austin/alice/RpPb_ExperimentData/RpPb_ALICEchj_R03.tsv"
aliceRpA04="/home/austin/alice/RpPb_ExperimentData/RpPb_ALICEchj_R04.tsv"


[ -d $output ] || mkdir $output

#root -x -q -l -b 'constructRpPbCombined.cxx("'$ppRootFile'", "'$pPbRootFile'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$atlasRpA'", "'$cmsRpA'", "'$phnxRpA'", "'$aliceRpA02'", "'$aliceRpA03'", "'$aliceRpA04'", "'$output'", "'$fileType'", '$minradius', '$maxradius')'

root -x -q -l -b 'constructRpPbCombined_3Panel.cxx("'$ppRootFile_NoBG'", "'$pPbRootFile_Full'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$atlasRpA'", "'$cmsRpA'", "'$phnxRpA'", "'$aliceRpA02'", "'$aliceRpA03'", "'$aliceRpA04'", "'$output/ppNoBG_pPbFull'", "'$fileType'", '$minradius', '$maxradius')'

root -x -q -l -b 'constructRpPbCombined_3Panel.cxx("'$ppRootFile_NoBG'", "'$pPbRootFile_Charged'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$atlasRpA'", "'$cmsRpA'", "'$phnxRpA'", "'$aliceRpA02'", "'$aliceRpA03'", "'$aliceRpA04'", "'$output/ppNoBG_pPbCharged'", "'$fileType'", '$minradius', '$maxradius')'

root -x -q -l -b 'constructRpPbCombined_3Panel.cxx("'$ppRootFile_Full'", "'$pPbRootFile_Full'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$atlasRpA'", "'$cmsRpA'", "'$phnxRpA'", "'$aliceRpA02'", "'$aliceRpA03'", "'$aliceRpA04'", "'$output/ppFull_pPbFull'", "'$fileType'", '$minradius', '$maxradius')'

root -x -q -l -b 'constructRpPbCombined_3Panel.cxx("'$ppRootFile_Charged'", "'$pPbRootFile_Charged'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$atlasRpA'", "'$cmsRpA'", "'$phnxRpA'", "'$aliceRpA02'", "'$aliceRpA03'", "'$aliceRpA04'", "'$output/ppCharged_pPbCharged'", "'$fileType'", '$minradius', '$maxradius')'