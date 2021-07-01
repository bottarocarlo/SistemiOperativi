#!/usr/bin/env bash
echo Commit name
read $COMMIT
cd /Users/BandaBottaro/Documents/Università/SistemiOperativi
git add *
git commit -m "'($COMMIT)'"
git push origin master 
