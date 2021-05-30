#1
read DATA
[[ -f $DATA ]] || [[ -d $DATA ]] && echo T || echo F
#2
[[ -f $DATA ]] && echo file 
[[ -d $DATA ]] && echo cartella 
[[ ! -e $DATA ]] && echo ? 
#3
[[ -f $DATA ]] && echo $(( $(cat $DATA) )) || echo "????"