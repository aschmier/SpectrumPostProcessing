#!/bin/bash

fMB="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/INT7.root"
fEMC7="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EMC7.root"
fEJE="/media/austin/mightymouse/data/pp8TeV_data/GA2423_default_final/EJE.root"

root -x -q -l -b 'getNEntriesForRange.cxx("'$fMB'","'$fEMC7'","'$fEJE'")' > NEntriesForRange.txt
