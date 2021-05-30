#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void myHandler(int sigNum){
    if(sigNum==SIGTSTP){
        printf("CTRL+C\n");
        exit(0);
    }else if(sigNum==SIGINT){
        printf("CTRL+Z\n");
        exit(0);
    }else if(sigNum==SIGCHLD){
        printf("child terminated\n");
        exit(0);
    }
    exit(-1);

}


int main(){
    signal(SIGINT,myHandler);//Ignore signal
    //signal(SIGCHLD,SIG_DFL);//use default handler
    signal(SIGTSTP,myHandler);//use myHandler
    signal(SIGCHLD,myHandler);


    printf("DFL: %p\n",signal(SIGINT,SIG_IGN));
    printf("IGN: %p\n",signal(SIGINT,myHandler));
    printf("Custom: %p == %p \n",signal(SIGINT,SIG_DFL),myHandler);
    
    int child = fork();
    if(!child){
        return 0;
    }
    while (wait(NULL)>0)
    
    
    return 0;
}