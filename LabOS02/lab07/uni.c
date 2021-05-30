#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define MAX_ARG 10
#define MAX_DEPTH 5
#define MAX_BROADTH 10
#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"


int main(int argc,char ** argv){
    int fd[2];
    int outcome = pipe(fd); 
    int Child = fork();
    char buf[50];
    if(!Child){
        //sono il figlio P2
        close(fd[0]); //chiudo il lato lettura
        write(fd[1],"writing",8);
        close(fd[1]);
        exit(0);
    }else{
        while(wait(NULL)>0);
        //sono il padre P1
        close(fd[1]); //chiudo il lato scrittura
        read(fd[0],&buf,50);
        printf("read from buffer='%s'\n",buf);
        close(fd[0]);
    }
    while(wait(NULL)>0);
    return 0;
}