#!/usr/bin/env bash 
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
        >&1 echo $long 
        >&2 echo $r
        break
    fi
done 

