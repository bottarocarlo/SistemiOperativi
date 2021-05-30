#include <stdio.h> //(f)printf
#include <string.h> //string
#include <stdlib.h> //exit
#include <unistd.h>//fork
#include <fcntl.h> //open
#include <sys/wait.h> //wait

int main(int argc,char ** argv){
    if (argc<2){
        fprintf(stderr,"usage\n");
        exit(-1);
    }
    int cont=0;
    char cr;
    while((cr=getchar())!=EOF){
        if(cr==argv[1][0]){
            printf("\033[0;31m%c\033[0;0m",cr);
            cont++;
        }else{
           printf("%c",cr); 
        }
    }
    printf("\n\nCI SONO %d %c",cont,argv[1][0]);

    return 0;
}
