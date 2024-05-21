#!/bin/bash

#ppRootFile="/home/austin/alice/FinalResultsRootFiles/FinalResults_NewBinning_pp8TeV.root"
#pPbRootFile="/home/austin/alice/FinalResultsRootFiles/FinalResults_pPb8TeV.root"
#ppRootFile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/ppFiles_NewBinning/output_default/bayes_default_final_2423.root"
ppRootFile="/home/austin/alice/SpectrumPostProcessing/bayes_default_2636.root"
#ppRootFile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_runlistEJE_2423.root"
#ppRootFile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_runlistEJ1.root"


pPbRootFile="/home/austin/alice/SpectrumPostProcessing/bayes_pPb_default.root"
#pPbRootFile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_requirel0/bayes_pPb_RequireL0.root"
#pPbRootFile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/pPbFiles/output_default/bayes_pPb_runlistEJ1.root"

output="/home/austin/alice/AnalysisNote_Spectra_RpA_8TeV/6495ba34f5cf02b65cad18e5/figures/pPbFigures"
fileType="pdf"
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

root -x -q -l -b 'constructRpPbCombined.cxx("'$ppRootFile'", "'$pPbRootFile'", "'$scaleFactorRootFile'", "'$dSysRootFiles'", "'$atlasRpA'", "'$cmsRpA'", "'$phnxRpA'", "'$aliceRpA02'", "'$aliceRpA03'", "'$aliceRpA04'", "'$output'", "'$fileType'", '$minradius', '$maxradius')'