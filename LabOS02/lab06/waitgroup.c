#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> //alarm.c 
#include <sys/wait.h>

void handler(int signo){
    printf("signal recived!\n");
    //exit(0);
}

int main(int argc,char ** argv){
    int group1,group2,ancestor=getpid();
    signal(SIGUSR1,handler);
    signal(SIGUSR2,handler);
    group1 = fork();
    if(!group1){//==0
        //sono il figlio
        setpgid(0,getpid());//divento un groupleader
        fork();fork();
        printf("[%d][%d]\n",getpid(),getpgid(0));
        fflush(stdout);
        //sleep(2);
        pause();
        printf("process terminating [%d]",getpid());
        exit(0);
    }else{
        //sono il padre
        group2 = fork();
        if(!group2){//==0
            //sono il figlio
            setpgid(0,getpid());//divento un groupleader
            fork();fork();
            printf("[%d][%d]\n",getpid(),getpgid(0));
            fflush(stdout);
            //sleep(4);
            pause();
            printf("process terminating [%d]",getpid());
            exit(0);
        }
    }
    /*
    sleep(2);
    while(waitpid(-group1,NULL,0)>0);
    printf("children in group %d terminated\n",group1);
    while(waitpid(-group2,NULL,0)>0);
    printf("children in group %d terminated\n",group2);
    return 0;
    */
   sleep(3);
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

}