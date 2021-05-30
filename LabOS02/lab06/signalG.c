#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> //alarm.c 
#include <sys/wait.h>

void handler(int signo){
    printf("[%d,%d]signal received!\n",getpid(),getpgid(0));
    fflush(NULL);
    sleep(1);
    exit(0);
}


int main(){
    signal(SIGUSR1,handler);
    signal(SIGUSR2,handler);
    int ancestor = getpid();
    int group1 = fork();
    int group2;

    if(getpid()!=ancestor){
        printf("i'm the children [%d]\n",getpid());
        setpgid(0,getpid());
        fork();fork();
    }else{
        group2 = fork();
        if(getpid()!=ancestor){
            printf("i'm the children [%d]\n",getpid());
            setpgid(0,getpid());
            fork();fork();
        }
    }
    if(getpid()==ancestor){
        
        sleep(1);
        printf("[%d]Ancestor and I'll send signals\n",getpid());
        kill(-group2,SIGUSR1); //Send signals to group2 
        kill(-group1,SIGUSR2); //Send signals to group1
    }else{
        printf("[%d,%d]chld waiting signal\n", getpid(),getpgid(0));
        while(1); }
    fflush(NULL);
    while(wait(NULL)>0);

    return 0;
}