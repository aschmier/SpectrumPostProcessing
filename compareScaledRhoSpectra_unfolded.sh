#!/bin/bash

defaultfull="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_default.root"
defaultcharged="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_defaultchargedrho.root"
defaulthigh="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_highchargedrho.root"
defaultlow="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pPb/output_default/bayes_pPb_lowchargedrho.root"

defaultnobgsub_pp="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_default_2702.root"
defaultcharged_pp="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_DefaultScaleVal_BGSub_2785.root"
defaulthigh_pp="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_HighScaleVal_BGSub_2785.root"
defaultlow_pp="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_LowScaleVal_BGSub_2785.root"
defaultfull_pp="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_FullRho_BGSub_2798.root"

output="/home/austin/alice/SpectrumPostProcessing/ScaledRhoSpectraComparisons/unfolded_pPb"
filetype="png"

ppNoBGSub="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_default_2702.root"
ppFullRho="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_FullRho_BGSub_2798.root"
ppChargedRho="/home/austin/alice/SpectrumPostProcessing/unfolding/results_pp/output_default/bayes_DefaultScaleVal_BGSub_2785.root"


[ -d $output ] || mkdir $output

#root -l -x -b -q 'compareScaledRhoSpectra_unfolded.cxx("'$defaultfull'","'$defaultcharged'","'$defaulthigh'","'$defaultlow'","'$output'","'$filetype'","pPb")'

output_pp="/home/austin/alice/SpectrumPostProcessing/ScaledRhoSpectraComparisons/unfolded_pp"
[ -d $output_pp ] || mkdir $output_pp

#root -l -x -b -q 'compareScaledRhoSpectra_unfolded.cxx("'$defaultfull_pp'","'$defaultcharged_pp'","'$defaulthigh_pp'","'$defaultlow_pp'","'$output_pp'","'$filetype'","pp")'

root -l -x -b -q 'compareScaledRhoSpectra_unfolded.cxx("'$ppFullRho'","'$ppChargedRho'","'$ppNoBGSub'","'$ppNoBGSub'","'$output_pp'","'$filetype'","pp")'