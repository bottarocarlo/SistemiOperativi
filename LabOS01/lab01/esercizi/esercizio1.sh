#leggo il file in input
read DATA
echo "FILE: "
[[ -f $DATA ]] && echo "T" || echo "F"
echo "DIRECTORY: "
[[ -d $DATA ]] && echo "T" || echo "F" 
echo "SO: "
[[ -f $DATA ]] && echo "FILE" 
[[ -d $DATA ]] && echo "DIRECTORY"
[[ ! -e $DATA ]] && echo "DOES NOT EXIST"

#stampa il valore di una semplice operaizone aritmentica contenuta in un file
read DATA

[[ -e $DATA ]] && echo $(( $(cat $DATA) )) || echo "????"