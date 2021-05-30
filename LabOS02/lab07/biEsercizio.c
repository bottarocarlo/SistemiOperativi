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
    if(!isChild){
        //sono il figlio P2
        close(pipe1[WRITE]);
        close(pipe2[READ]);

        //sleep(1);

        read (pipe1[READ],&buffer,50);
        printf("[P2]received : '%s'\n",buffer);
        write(pipe2[WRITE],"received",9);

        //sleep(1);

        read (pipe1[READ],&buffer,50);
        printf("[P2]received : '%s'\n",buffer);
        write(pipe2[WRITE],"received",9);

        //sleep(1);

        read (pipe1[READ],&buffer,50);
        printf("[P2]received : '%s'\n",buffer);
        write(pipe2[WRITE],"received",9);

        //sleep(1);

        read (pipe1[READ],&buffer,50);
        printf("[P2]received : '%s'\n",buffer);
        write(pipe2[WRITE],"received",9);


        close(pipe2[WRITE]);
        close(pipe1[READ]);
    }else{
        //sono il padre P1
        close(pipe2[WRITE]);
        close(pipe1[READ]);

        write (pipe1[WRITE],"ciao carlo",11);
        read(pipe2[READ],&buffer,50);
        printf("[P1]received : '%s'\n",buffer);

        //sleep(1);

        write (pipe1[WRITE],"come va?",9);
        read(pipe2[READ],&buffer,50);
        printf("[P1]received : '%s'\n",buffer);

        //sleep(1);

        write (pipe1[WRITE],"apposto dai",12);
        read(pipe2[READ],&buffer,50);
        printf("[P1]received : '%s'\n",buffer);

        //sleep(1);

        write (pipe1[WRITE],"ci sentiamo, grande!",21);
        read(pipe2[READ],&buffer,50);
        printf("[P1]received : '%s'\n",buffer);

        close(pipe1[WRITE]);
        close(pipe2[READ]);
    }

    while(wait(NULL)>0);

    return 0;
}