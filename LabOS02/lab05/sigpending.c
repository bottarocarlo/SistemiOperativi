#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

int i=0;

void myHandler(int signo){
    printf("signal riceived\n");
    i++;
}

int main(){
    printf("Hi, i am: %d\n",getpid());
    signal(SIGUSR1,myHandler);
    sigset_t mod , old;
    //sigfillset(&mod);
    sigemptyset(&mod);
    sigaddset(&mod,SIGUSR1);
    //sigismember(&mod,SIGALRM);
    //sigdelset(&mod,SIGALRM);

    //sigprocmask(SIG_BLOCK,&mod,&old);

    while(1) if(i==2) exit(0);

    return 0;
}