#!/usr/bin/env bash 
alias longest='
long=""
r=0
quit="quit"
while read data; do
    lun=${#data}
    if [ $lun -ge $r ]; then
        long=$data
        r=$lun
    fi
    if [ "$data" = "$quit" ]; then
        echo $long 1>out.txt
        echo $r 1>err.txt
        break
    fi
done'
