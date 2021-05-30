#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char ** argv){

    if(argc<2){
        printf("usage <string>\n");
        exit(-2);
    }
    int cont=0;
    
    while(argv[1][cont]!='\0'){
        cont++;
    }
    printf("lunghezza: %d\n",cont);
    printf("strlen(%s): %lu",argv[1],strlen(argv[1]));
    return 0;
}