#!/bin/bash
DATA='.'
while [[ ! -z $DATA ]]
do
echo "Number: "; read DATA ; (( SOMMA=$SOMMA+$DATA ))
done
echo "La somma vale: " $SOMMA