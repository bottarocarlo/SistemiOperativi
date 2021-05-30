#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"
#define READ 0
#define WRITE 1

int main(){
    int pipe1[2],pipe2[2];
    pipe(pipe1),pipe(pipe2);
    char buffer[50];
    int isChild = fork();

    if(isChild){
        //sono il padre
        close(pipe1[WRITE]);
        close(pipe2[READ]);

        for(int i=0;i<3;i++){
            write(pipe2[WRITE],"ciao mbare",11);
            sleep(1);
        }
        write(pipe2[WRITE],"quit",11);

        close(pipe1[READ]);
        close(pipe2[WRITE]);

    }else{
        sleep(1);
        close(pipe1[READ]);
        close(pipe2[WRITE]);

        do{
            read(pipe2[READ],&buffer,50);
            printf("[P2] : %s\n",buffer);
            sleep(1);
        }while ( strncmp(buffer,"quit",5)==0 );
        printf("[P2] : QUITTED..\n");
        close(pipe1[WRITE]);
        close(pipe2[READ]);

    }


    return 0;
}