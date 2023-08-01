input=/media/austin/mightymouse/data/pPb8TeV_MC/merged_15383940_default/AnalysisResults.root
output=/home/austin/alice/pPbTestPost/png
system="pPb"
fileType="png"
minradius=2
maxradius=2

[ -d $output ] || mkdir $output

root -x -q -l -b 'plotTriggerEfficienciesManual.cxx("'$input'", "'$output'", "'$fileType'", "'$system'", '$minradius', '$maxradius')'