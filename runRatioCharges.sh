#!/bin/bash

ppFileBase="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2779_ptscheme_TrackQA"
ppFileBaseZero="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2775_ptscheme_rhoscale"
pPbFileBase="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/GA1186_TrackQA"

outpp="/home/austin/alice/SpectrumPostProcessing/HPlusMinus/MarkusMacro/pp"
outpPb="/home/austin/alice/SpectrumPostProcessing/HPlusMinus/MarkusMacro/pPb"

mkdir -p $outpp
mkdir -p $outpPb

periodspp=("LHC12a" "LHC12b" "LHC12c" "LHC12d" "LHC12f" "LHC12h" "LHC12i")
triggerspp=("INT7" "EMC7" "EJE")
wagonspp=("plus10" "plus5" "plus1" "minus1" "minus5" "minus10")

triggerspPb=("INT7" "EJ")
#wagonspPb=("0" "1" "3" "5" "default" "plus1")
wagonspPb=("2" "3" "4" "5" "6" "7" "8" "9")

# for wagon in "${wagonspPb[@]}"
# do
#     # full eta
#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$pPbFileBase/AnalysisResults.root'","'$outpPb'","pPb","'$wagon'")'
#     # -eta
#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$pPbFileBase/AnalysisResults.root'","'$outpPb'","pPb","'$wagon'",-1)'
#     # +eta
#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$pPbFileBase/AnalysisResults.root'","'$outpPb'","pPb","'$wagon'",1)'
# done

# for i in {3..7}
# do 
#     period=${periodspp[$i-1]}
#     mkdir -p $outpp/$period
#     # full eta
#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBaseZero/child_$i/runlist_8/AnalysisResults.root'","'$outpp/$period'","pp","default",true)'
# done

mkdir -p $outpp/PositiveField
mkdir -p $outpp/NegativeField
root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBaseZero/positiveField.root'","'$outpp/PositiveField'","pp","default",true)'
root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBaseZero/negativeField.root'","'$outpp/NegativeField'","pp","default",true)'

rm $outpp/*/*.pdf $outpp/*/*.gif $outpp/*/*.jpg $outpp/*/*.eps

# for child in {1..7}
# do
#     for list in {6..8}
#     do
#         mkdir -p $outpp/Full/${periodspp[$child-1]}/${triggerspp[$list-6]}
#         for wagon in "${wagonspp[@]}"
#         do
#             root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/child_$child/runlist_$list/AnalysisResults.root'","'$outpp/Full/${periodspp[$child-1]}/${triggerspp[$list-6]}'","pp","'$wagon'")'
#             root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/child_$child/runlist_$list/AnalysisResults.root'","'$outpp/Full/${periodspp[$child-1]}/${triggerspp[$list-6]}'","pp","'$wagon'",-1)'
#             root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/child_$child/runlist_$list/AnalysisResults.root'","'$outpp/Full/${periodspp[$child-1]}/${triggerspp[$list-6]}'","pp","'$wagon'",1)'
#         done

#         mkdir -p $outpp/Simple/${periodspp[$child-1]}/${triggerspp[$list-6]}
#         for wagon in "${wagonspp[@]}"
#         do
#             root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/child_$child/runlist_$list/AnalysisResults.root'","'$outpp/Simple/${periodspp[$child-1]}/${triggerspp[$list-6]}'","pp","'$wagon'")'
#             root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/child_$child/runlist_$list/AnalysisResults.root'","'$outpp/Simple/${periodspp[$child-1]}/${triggerspp[$list-6]}'","pp","'$wagon'",-1)'
#             root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/child_$child/runlist_$list/AnalysisResults.root'","'$outpp/Simple/${periodspp[$child-1]}/${triggerspp[$list-6]}'","pp","'$wagon'",1)'
#         done
#     done
# done

# for wagon in "${wagonspp[@]}"
# do
#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/AnalysisResults.root'","'$outpp/Full'","pp","'$wagon'")'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/AnalysisResults.root'","'$outpp/Simple'","pp","'$wagon'")'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/negative_polarity.root'","'$outpp/Full/NegativePolarity'","pp","'$wagon'")'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/negative_polarity.root'","'$outpp/Simple/NegativePolarity'","pp","'$wagon'")'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/positive_polarity.root'","'$outpp/Full/PositivePolarity'","pp","'$wagon'")'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/positive_polarity.root'","'$outpp/Simple/PositivePolarity'","pp","'$wagon'")'

#     # -eta
#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/AnalysisResults.root'","'$outpp/Full'","pp","'$wagon'",-1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/AnalysisResults.root'","'$outpp/Simple'","pp","'$wagon'",-1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/negative_polarity.root'","'$outpp/Full/NegativePolarity'","pp","'$wagon'",-1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/negative_polarity.root'","'$outpp/Simple/NegativePolarity'","pp","'$wagon'",-1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/positive_polarity.root'","'$outpp/Full/PositivePolarity'","pp","'$wagon'",-1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/positive_polarity.root'","'$outpp/Simple/PositivePolarity'","pp","'$wagon'",-1)'

#     # +eta
#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/AnalysisResults.root'","'$outpp/Full'","pp","'$wagon'",1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/AnalysisResults.root'","'$outpp/Simple'","pp","'$wagon'",1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/negative_polarity.root'","'$outpp/Full/NegativePolarity'","pp","'$wagon'",1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/negative_polarity.root'","'$outpp/Simple/NegativePolarity'","pp","'$wagon'",1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioCharges_8TeV.cpp("'$ppFileBase/positive_polarity.root'","'$outpp/Full/PositivePolarity'","pp","'$wagon'",1)'

#     root -x -q -l -b '/home/austin/alice/SubstructureAnalysis/TRDtest/MakeRatioChargesSimple_8TeV.cpp("'$ppFileBase/positive_polarity.root'","'$outpp/Simple/PositivePolarity'","pp","'$wagon'",1)'
# done