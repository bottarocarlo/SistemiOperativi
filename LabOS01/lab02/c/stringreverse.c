#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char *stringrev(char *str){
    int tmp = strlen(str);
    char rev[tmp];
    
    for(int i=0 ; i<tmp ; i++){
        rev[i] = str[tmp-i-1];
    }
    strcpy(str,rev);
    return str;
}

int stringpos(char *str,char chr){
    int pos=-1;
    int tmp = strlen(str);
    for(int i =0 ; i< tmp ;i++){
        if(str[i]==chr){
            return i+1;
        }
    }

    return pos;
}

int main(int argc, char ** argv){
    int status = 0;
    char src;
    if(argc<2){
        printf("scemo usage: ./stringreverse.o <string>");
        status = 2;
        exit (status);
    }else {
        printf("%s\nCarattere da cercare: ",stringrev(argv[1]));
        scanf("%c",&src);
        printf("\nil carattere %c si trova alla posizione %d", src,stringpos(argv[1],src));
    }

    return status;
}