#!/bin/bash
DATA='.'
while [[ ! -z $DATA ]]
do
read DATA ; (( somma = $somma + $DATA ))
done
echo $somma