#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void ctrlz(int sigNum){
    printf("CTRL+Z\n");
    exit(0);
}

void ctrlc(int sigNum){
    printf("CTRL+C\n");
    exit(0);
}

void alarmone(int sigNum){
    printf("alarm\n");
}

void handler(int sigNum){
    printf("MyHandler\n");
}

int main(int argc,char ** argv){


    //signal(SIGINT,SIG_IGN);//ignore signal
    signal(SIGCHLD,SIG_DFL);//default handler
    signal(SIGTSTP,ctrlz);//Use CTRL+Z
    //signal(SIGINT,ctrlc);//use CTRL+C
    signal(SIGALRM,alarmone);
    signal(SIGINT,handler);

    pause();//aspetto un segnale!!

    /*
    SIGALRM (alarm clock) quit
    SIGCHLD (child terminated) ignore
    SIGCONT (continue, if stopped) ignore
    SIGINT  (terminal interrupt, CTRL + C) quit
    SIGKILL (kill process) quit
    SIGSYS  (bad argument to syscall) quit with dump
    SIGTERM (software termination) quit
    SIGUSR1 (user signal 1) quit
    SIGUSR2 (user signal 2) quit
    SIGSTOP (stopped) quit
    SIGTSTP (terminal stop, CTRL + Z) quit

    ● pid > 0: segnale al processo con PID=pid
    ● pid = 0: segnale ad ogni processo dello stesso gruppo
    ● pid = -1: segnale ad ogni processo possibile (stesso UID/RUID)
    ● pid < -1: segnale ad ogni processo del gruppo |pid|
    */

    int child = fork();
    if (!child) 
        while(1); // this is the child

    sleep(1);
    kill(child,SIGALRM);
    sleep(1);
    kill(child,SIGTERM); // send TERM: default is to terminate while
    while(wait(NULL)>0);
    
    return 0;
}