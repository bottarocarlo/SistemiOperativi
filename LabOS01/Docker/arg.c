#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc,char ** argv){
    if(argc<2){
        printf("usage <arg>\n");
        exit (-2);
    }

    for(int i=1;i<argc;i++){

        if(strcmp(argv[i],"-h")==0){
            printf("help\n");
        }else if(strcmp(argv[i],"-l")==0){
            printf("list\n");
        }else if(strcmp(argv[i],"-c")==0){
            printf("color\n");
        }else {
             fprintf(stderr,"arg\033[0;31m %s \033[0;0mnot found", argv[i]);
        }
    }

    return 0;
}