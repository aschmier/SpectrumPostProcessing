#!/bin/bash

# Train comparison plots
root -x -q -l -b 'plotTrainComparison.C("output_clusterizer/bayes_3x3_2246.root","input_txt_files/all_bayes.txt","3x3_46","3x3_all_46","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_clusterizer/bayes_5x5_2245.root","input_txt_files/all_bayes.txt","5x5_45","5x5_all_45","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_clusterizer/bayes_3x3_2291.root","input_txt_files/all_bayes.txt","3x3_91","3x3_all_91","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_clusterizer/bayes_5x5_2292.root","input_txt_files/all_bayes.txt","5x5_92","5x5_all_92","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_cutoff/bayes_MaxTrackPt200_R02_2260.root","input_txt_files/all_bayes.txt","MaxTrackPt200","MaxTrackPt200_all","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_default/bayes_default_2273.root","input_txt_files/all_bayes.txt","default_73","default_all_73","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_default/bayes_default_2284.root","input_txt_files/all_bayes.txt","default_84","default_all_84","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_default/bayes_default_2285.root","input_txt_files/all_bayes.txt","default_85","default_all_85","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_hadcorr/bayes_F07_2256.root","input_txt_files/all_bayes.txt","F07","F07_all","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_hadcorr/bayes_MIP_2257.root","input_txt_files/all_bayes.txt","MIP","MIP_all","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_seed/bayes_SeedE275_2230.root","input_txt_files/all_bayes.txt","SeedE275","SeedE275_all","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_seed/bayes_SeedE350_2231.root","input_txt_files/all_bayes.txt","SeedE350","SeedE350_all","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_tracking/bayes_Tracking_2273.root","input_txt_files/all_bayes.txt","tracking_73","tracking_all_73","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_tracking/bayes_Tracking_2284.root","input_txt_files/all_bayes.txt","tracking_84","tracking_all_84","Bayes",2)'
