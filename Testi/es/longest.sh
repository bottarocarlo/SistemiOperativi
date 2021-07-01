#!/bin/bash

longest=''
DATA='.'
quit="quit"
r=0
while read DATA; do 
lun=${#DATA}
    if [ $lun -ge $r ]; then
        longest=$DATA
        r=$lun
    fi
if [ "$DATA" = "$quit" ]; then
        echo $longest
        break
    fi
done 
echo $longest