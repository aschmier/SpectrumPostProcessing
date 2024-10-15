 #!/bin/bash

ppFile="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2779_ptscheme_TrackQA/AnalysisResults.root"
ppFileZero="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2825_TrackQAandChRhoDef/AnalysisResults.root"
ppFileBaseZero="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2825_TrackQAandChRhoDef"
ppFileBase="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2779_ptscheme_TrackQA"
ppPositivePolarity="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2779_ptscheme_TrackQA/positive_polarity.root"
ppNegativePolarity="/media/austin/mightymouse/data/pp8TeV_data/ptscheme_trains/GA2779_ptscheme_TrackQA/negative_polarity.root"
pPbFile="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/GA1186_TrackQA/AnalysisResults.root"
pPbFileZero="/media/austin/mightymouse/data/pPb8TeV_data/ptscheme_trains/GA1185_TrackQA/AnalysisResults.root"


outpp="/home/austin/alice/SpectrumPostProcessing/HPlusMinus/AustinMacro/pp/2825"
outpPb="/home/austin/alice/SpectrumPostProcessing/HPlusMinus/AustinMacro/pPb"

mkdir -p $outpp
mkdir -p $outpPb

root -x -q -l -b 'plotTrackQA.cxx("'$ppFile'","'$outpp'","pp","minus1")'
root -x -q -l -b 'plotTrackQA.cxx("'$ppFile'","'$outpp'","pp","minus5")'
root -x -q -l -b 'plotTrackQA.cxx("'$ppFile'","'$outpp'","pp","minus10")'

root -x -q -l -b 'plotTrackQA.cxx("'$ppFile'","'$outpp'","pp","minus1",0,true)'
root -x -q -l -b 'plotTrackQA.cxx("'$ppFile'","'$outpp'","pp","minus5",0,true)'
root -x -q -l -b 'plotTrackQA.cxx("'$ppFile'","'$outpp'","pp","minus10",0,true)'

# root -x -q -l -b 'plotTrackQA.cxx("'$ppPositivePolarity'","'$outpppos'","pp","plus10")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppPositivePolarity'","'$outpppos'","pp","plus5")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppPositivePolarity'","'$outpppos'","pp","plus1")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppPositivePolarity'","'$outpppos'","pp","minus1")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppPositivePolarity'","'$outpppos'","pp","minus5")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppPositivePolarity'","'$outpppos'","pp","minus10")'

# root -x -q -l -b 'plotTrackQA.cxx("'$ppNegativePolarity'","'$outppneg'","pp","plus10")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppNegativePolarity'","'$outppneg'","pp","plus5")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppNegativePolarity'","'$outppneg'","pp","plus1")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppNegativePolarity'","'$outppneg'","pp","minus1")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppNegativePolarity'","'$outppneg'","pp","minus5")'
# root -x -q -l -b 'plotTrackQA.cxx("'$ppNegativePolarity'","'$outppneg'","pp","minus10")'

# periods=("LHC12a" "LHC12b" "LHC12c" "LHC12d" "LHC12f" "LHC12h" "LHC12i")
# triggers=("INT7" "EMC7" "EJE")
# wagons=("plus10" "plus5" "plus1" "minus1" "minus5" "minus10")



# for i in {1..7}
# do
#     for j in {6..8}
#     do
#         mkdir -p $outpp/${periods[$i-1]}/${triggers[$j-6]}
#         for k in "${wagons[@]}"
#         do
#             root -x -q -l -b 'plotTrackQA.cxx("'$ppFileBase/child_$i/runlist_$j/AnalysisResults.root'","'$outpp/${periods[$i-1]}/${triggers[$j-6]}'","pp","plus10")'
#         done
#     done
# done

# for i in {3..7}
# do
#     mkdir -p $outpp/${periods[$i-1]}
#     root -x -q -l -b 'plotTrackQA.cxx("'$ppFileBaseZero/child_$i/runlist_8/AnalysisResults.root'","'$outpp/${periods[$i-1]}'","pp","default",0,false)'
#     root -x -q -l -b 'plotTrackQA.cxx("'$ppFileBaseZero/child_$i/runlist_8/AnalysisResults.root'","'$outpp/${periods[$i-1]}'","pp","default",0,true)'
# done


# for i in {2..9}
# do
#     root -x -q -l -b 'plotTrackQA.cxx("'$pPbFile'","'$outpPb'","pPb","'$i'",0,true)'
#     root -x -q -l -b 'plotTrackQA.cxx("'$pPbFile'","'$outpPb'","pPb","'$i'",0,false)'
# done

# for i in {-1 0 1}
# do
#     root -x -q -l -b 'plotTrackQA.cxx("'$pPbFileZero'","'$outpPb'","pPb","0",'$i',true)'
#     root -x -q -l -b 'plotTrackQA.cxx("'$pPbFileZero'","'$outpPb'","pPb","0",'$i',false)'
# done

# root -x -q -l -b 'plotTrackQA.cxx("'$pPbFileZero'","'$outpPb'","pPb","0",0,true)'
# root -x -q -l -b 'plotTrackQA.cxx("'$pPbFileZero'","'$outpPb'","pPb","0",1,true)'
# root -x -q -l -b 'plotTrackQA.cxx("'$pPbFileZero'","'$outpPb'","pPb","0",-1,true)'


# root -x -q -l -b 'plotTrackQA.cxx("'$pPbFileZero'","'$outpPb'","pPb","0",0,false)'
# root -x -q -l -b 'plotTrackQA.cxx("'$pPbFileZero'","'$outpPb'","pPb","0",1,false)'
# root -x -q -l -b 'plotTrackQA.cxx("'$pPbFileZero'","'$outpPb'","pPb","0",-1,false)'
