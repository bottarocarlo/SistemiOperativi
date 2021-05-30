#!/bin/bash
DATA='.'
while [[ ! -z $DATA ]] 
do 
echo "numero: "  ; read DATA ; (( SOMMA = $SOMMA + $DATA ))
done 

echo la somma vale $SOMMA