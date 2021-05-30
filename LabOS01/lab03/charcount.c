#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define N 100
#define MAX 1000

int main(int argc, char ** argv){
    int status =0;
    char buf[N];
    char text[MAX];
    int cont=0;
    

    //printf("%s",buf);
    if(argc<2){
        printf("usage: .... | ./charcount.o \"a\" \n");
        exit(-1);
    }
    
    while( fgets(buf,sizeof(buf),stdin) ){
        //printf("%s\n",buf);
        strcat( text, buf );
    }


    for(int i=0; i<=sizeof(text) ;i++){
        if(text[i] == argv[1][0] ){
            cont++;
            //printf("\033[0;31m%c\033[0;0m",text[i]);
        }else{
            
            printf("%c",text[i]);
        }
    }
    printf("ci sono: %d (%c)",cont,argv[1][0]);
    return status;
}
