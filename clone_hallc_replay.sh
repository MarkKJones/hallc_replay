#!/bin/csh

#INITIAL HCANA CONFIGURATION ON LOCAL MACHINE

#Clone the hcana repository from the main site (only done once)
git clone https://github.com/Yero1990/hallc_replay

#change directory to hcana (repository)
cd hallc_replay

#add a path to keep local copy of hallc_replay up to date with main repo   
#master=> branch cloned to local machine, upstream=> main repo branch
git remote add --track master upstream git@github.com:/JeffersonLab/hallc_replay

#For hallc_replay, it is important to branch off the master branch and
#So immediately do

git checkout -b hms_DC_develop



