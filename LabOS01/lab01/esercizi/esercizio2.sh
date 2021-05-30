#dato in input un qualsiasi numero di argomenti li restituisce in maniera inverse

nargs=$#
while [[ nargs -ne 0 ]] ; do
    echo $( echo $nargs )
    (( nargs-- ))
done