#!/bin/bash

#
#if [ -f prova.txt ]
# then
#       echo Il file esiste!
#  else
#       echo Il file non esiste!
#fi
#
if [[ $# -ne 1 ]]; then echo "?Usage: $0 <num>" 1>&2 ; exit 2 ; fi
nargs=$1
while [[ $nargs -gt 0 ]]
do 
    ( echo $BASHPID )
    nargs=$(( nargs-1 ))
done