#!/bin/bash

mcfile="/home/austin/alice/SpectrumPostProcessing/bayes_default_2423_MinBiasEdit.root"

root -x -q -l -b 'getBinContentsResponse.cxx("'$mcfile'")'
