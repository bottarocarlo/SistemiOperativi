#!/bin/bash

read DATA
if [ -e $DATA ]
then
       echo $( wc -l $DATA )
    else
       echo Il file non esiste!
fi
