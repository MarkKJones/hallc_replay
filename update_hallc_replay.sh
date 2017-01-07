#!/bin/bash

#Update hallc replay on local machine

#While on hall c replay directory, do:
git checkout master
git fetch upstream
git merge upstream/master

#Update forked copy of hcana
git push origin master   #updates the hcana develop branch on my GitHub Account

#If the master branch on the main repository
#(upstream repository) changes and you want to 
#incorporate the changes into the hms_DC_develop branch
#that you are working on then follow these steps:
#git checkout hms_DC_develop
#git rebase -i master


#One should branch off the master into a new branch in order
#to avoid erasing any code under development by the user
#Once finished working on code, one should > git add (stage) and 
#git commit (save) on one's own branch. This branch should be pushed 
#to origin (forked hallc). Then, while updating hcana, the user work 
#on their own branch should be safe

#FOLLOW THESE STEPS WHILE on hms_DC_develop branch

#git add filename1, ...
#git commit filename1, ..
#git push origin hms_DC_develop  (update you hms_DC_develop branch on forked copy)
