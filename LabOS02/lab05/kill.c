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
    }else if(sigNum==SIGKILL){
        printf("kill process\n");
        
    }
    exit(-1);

}


int main(){
    signal(SIGKILL,myHandler);
    int child = fork();
    printf("pid: %d\nchildID: %d",getpid(),child);
    if(!child) while(1);
    sleep(1);
    printf("sending kill to child..\n");
    kill(child,SIGKILL);
    sleep(1);
    printf("sending term to child..\n"); 
    kill(child,SIGTERM);
    while (wait(NULL)>0);


    return 0;
}