#!/bin/bash

ppfile="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_default/bayes_default_final_2423.root"

pAfile_HeaderFilter="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_pPb/bayes_pPb_HeaderFilter_R02.root"
pAfile_HeaderFilter2="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_pPb/bayes_pPb_HeaderFilter2_R02.root"
pAfile_NoRejection="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_pPb/bayes_pPb_NoRejection_R02.root"
pAfile_EPOSCorrected="/home/austin/alice/SpectrumPostProcessing/Unfolding/UnfoldingResults/output_pPb/bayes_pPb_EPOSCorrected_R02.root"

out_HeaderFilter="/home/austin/alice/SpectrumPostProcessing/RpA/HeaderFilter"
out_HeaderFilter2="/home/austin/alice/SpectrumPostProcessing/RpA/HeaderFilter2_new"
out_NoRejection="/home/austin/alice/SpectrumPostProcessing/RpA/NoRejection"
out_EPOSCorrected="/home/austin/alice/SpectrumPostProcessing/RpA/EPOSCorrected"

[ -d $out_HeaderFilter ] || mkdir $out_HeaderFilter
[ -d $out_HeaderFilter2 ] || mkdir $out_HeaderFilter2
[ -d $out_NoRejection ] || mkdir $out_NoRejection
[ -d $out_EPOSCorrected ] || mkdir $out_EPOSCorrected

#root -x -q -l -b 'plotRpA.C("'$ppfile'","'$pAfile_HeaderFilter'","'$out_HeaderFilter'","Bayes",2)'
root -x -q -l -b 'plotRpA.C("'$ppfile'","'$pAfile_HeaderFilter2'","'$out_HeaderFilter2'","Bayes",2)'
#root -x -q -l -b 'plotRpA.C("'$ppfile'","'$pAfile_NoRejection'","'$out_NoRejection'","Bayes",2)'
#root -x -q -l -b 'plotRpA.C("'$ppfile'","'$pAfile_EPOSCorrected'","'$out_EPOSCorrected'","Bayes",2)'
