#!/bin/bash

#NoSub="/media/austin/mightymouse/data/pPb8TeV_data/GA1005_default/AnalysisResults.root"
#PerpCone="/media/austin/mightymouse/data/pPb8TeV_data/GA995_PerpCone/AnalysisResults.root"
#RhoSparse="/media/austin/mightymouse/data/pPb8TeV_data/GA996_RhoSparse/AnalysisResults.root"

NoSub="/media/austin/mightymouse/data/pPb8TeV_data/GA1055_RhoSparse/AnalysisResults.root"
PerpCone="/media/austin/mightymouse/data/pPb8TeV_data/GA1058_skim_RhoSparse_Clusterizer3x3/AnalysisResults.root"
RhoSparse="/media/austin/mightymouse/data/pPb8TeV_data/GA996_RhoSparse/AnalysisResults.root"

output="/home/austin/alice/SpectrumPostProcessing/BGRejMethods/temp"

fileType="png"
radius=2

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotBGRejMethods.cxx("'$NoSub'", "'$PerpCone'", "'$RhoSparse'", "'$output'", '$radius', "'$fileType'")'
