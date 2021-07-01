read DATA
[[ -e  $DATA ]] && echo ok || echo no
#così valuto un if[[condizione]] && true || false
[[ -f  $DATA ]] && echo file
[[ -d  $DATA ]] && echo directory
[[ ! -e  $DATA ]] && echo not exist
echo $(( $(cat $DATA) )) || echo "????"
echo $BASHPID
echo $( echo  $BASHPID)