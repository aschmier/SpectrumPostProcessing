#!/bin/bash

# Train comparison plots
#root -x -q -l -b 'plotTrainComparison.C("output_clusterizer/bayes_3x3_2246.root","input_txt_files/some_bayes.txt","3x3_46","3x3_some_46","Bayes",2)'
#root -x -q -l -b 'plotTrainComparison.C("output_clusterizer/bayes_5x5_2245.root","input_txt_files/some_bayes.txt","5x5_45","5x5_some_45","Bayes",2)'
#root -x -q -l -b 'plotTrainComparison.C("output_clusterizer/bayes_3x3_2291.root","input_txt_files/some_bayes.txt","3x3_91","3x3_some_91","Bayes",2)'
#root -x -q -l -b 'plotTrainComparison.C("output_clusterizer/bayes_5x5_2292.root","input_txt_files/some_bayes.txt","5x5_92","5x5_some_92","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_default/bayes_default_2273.root","input_txt_files/some_bayes.txt","default_73","default_some_73","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_default/bayes_default_2284.root","input_txt_files/some_bayes.txt","default_84","default_some_84","Bayes",2)'
root -x -q -l -b 'plotTrainComparison.C("output_default/bayes_default_2285.root","input_txt_files/some_bayes.txt","default_85","default_some_85","Bayes",2)'
