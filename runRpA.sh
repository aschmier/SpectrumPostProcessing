#!/bin/bash

ppfile="/home/austin/alice/unfoldingResults/output_default/bayes_default_final_2294.root"
#ppfile="/home/austin/alice/unfoldingResults/output_pPb/bayes_pPb_orig.root"

#pAfile_orig="/home/austin/alice/unfoldingResults/output_pPb/bayes_pPb_orig.root"
#pAfile_ppSim="/home/austin/alice/unfoldingResults/output_pPb/bayes_pPb_ppSim.root"
pAfile_EPOSCorrected="/home/austin/alice/unfoldingResults/output_pPb/bayes_pPb_EPOSCorrected.root"
#out_orig="/home/austin/alice/unfoldingResults/RpA_results/Orig"
#out_ppSim="/home/austin/alice/unfoldingResults/RpA_results/ppSim"
out_EPOSCorrected="/home/austin/alice/unfoldingResults/RpA_results/EPOSCorrected"

#[ -d $out_orig ] || mkdir $out_orig
#[ -d $out_ppSim ] || mkdir $out_ppSim
[ -d $out_EPOSCorrected ] || mkdir $out_EPOSCorrected

#root -x -q -l -b 'plotRpA.C("'$ppfile'","'$pAfile_orig'","'$out_orig'","Bayes",2)'
#root -x -q -l -b 'plotRpA.C("'$ppfile'","'$pAfile_ppSim'","'$out_ppSim'","Bayes",2)'
root -x -q -l -b 'plotRpA.C("'$ppfile'","'$pAfile_EPOSCorrected'","'$out_EPOSCorrected'","Bayes",2)'
