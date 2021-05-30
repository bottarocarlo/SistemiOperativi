#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void myHandler(int sigNum){
    printf("PROCESS KILLED\n");
    exit(-1);
}


int main(){
    signal(SIGALRM,myHandler);
    printf("Hi, i am: %d\n",getpid());
    while (1);

    return 0;
}