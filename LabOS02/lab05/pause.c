#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> //alarm.c 
short cnt = 0;


void myHandler(int sigNum){
    if(sigNum==SIGCONT){
        printf("continue!\n");
    }else if (sigNum==SIGUSR1)
    {
        printf("siguser1!\n");
    }else {
        printf("BHO %d\n",sigNum);
    }
    
    }

int main(){
    signal(SIGALRM,myHandler);
    signal(SIGCONT,myHandler);
    signal(SIGUSR1,myHandler);    
    printf("Hi i'm %d\n",getpid());


    pause();
    printf("continuato amore!\n");

    return 0;
}