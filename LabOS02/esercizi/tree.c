#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h> 
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

//color
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYANO "\033[0;36m"
#define WHITE "\033[0;37m"
#define DF "\033[0m"

#define MAX_DEPHT 5

int pid[MAX_DEPHT];
char buffer[5];

//----chlildren attributes----//
int children[MAX_DEPHT],proc,numChilden=0;

struct msg_buffer
{
    char payload[3];
    int type;
}msg;

void loop(){
    while (1);
    exit(0);
}

void quit(){
    for(int i =1;i<MAX_DEPHT;i++){
        kill(-pid[i],SIGTERM);
        printf("%s[%d] killed!%s\n",RED,pid[i],DF);
    }
    printf("%s[MAIN][%d]terminated %s\n",RED,getpid(),DF);
    kill(-pid[0],SIGTERM);
    while(wait(NULL)>0);
    exit(0);
}        

void create(int sigNum){
    if(numChilden<MAX_DEPHT){
        children[numChilden]=fork();
        if(children[numChilden]==0){
            printf("%s[%d] children created at level: %d %s\n",GREEN,getpid(),proc,DF);
            loop();
        }
        numChilden++;
    }else{
        printf("%sTOO MUCH CHILDREN AT THIS LEVEL%s\n",RED,DF);
    }
}

void killC(int sigNum){
    for(int i=0;i<numChilden;i++){
        kill(children[i],SIGTERM);
        printf("%s[%d] children killed at level: %d %s\n",RED,children[i],proc,DF);
    }
    numChilden=0;
}

void tab(int tab){
    for(int i=0;i<tab;i++){
        printf("\t");fflush(NULL);
    }
}

void print(int sigNum){
for(int i =0;i<numChilden;i++){
        tab(proc);
        printf("%s[ID %d - Parent %d] depht %d%s\n",BLUE,children[i],getpid(),proc,DF);
    }
}

void printAll(){
    printf("%s[ID %d - Parent 0] depht 0%s\n",BLUE,pid[0],DF);
    for(int i =1;i<MAX_DEPHT;i++){
        tab(i);
        printf("%s[ID %d - Parent %d] depht %d%s\n",BLUE,pid[i],pid[i-1],i,DF);
        kill(pid[i],SIGALRM);
    }

}

int main(int argc,char ** argv){
    int level;

    signal(SIGUSR1,create);
    signal(SIGUSR2,killC);
    signal(SIGALRM,print);
    pid[0] = getpid();
    proc = 0;
    printf("%s[MAIN][%d] created!%s\n",GREEN,getpid(),DF);

    for(int i =1;i<MAX_DEPHT;i++){
        pid[i] = fork();
        if(getpid()!=pid[0]){
            proc = i;
            setpgid(0,getpid());
            printf("%s[%d] created!%s\n",GREEN,getpid(),DF);
            break;
        }
    }

    if(getpid()==pid[0]){
        while(1){
            sleep(1);
            printf("Next Command: \t"); fflush(NULL);
            read(STDIN_FILENO,buffer,5);
            if(buffer[0]=='q'){
                quit();
            }else if(buffer[0] == 'c'){
                //create
                level = atoi(buffer+1);
                kill(pid[level],SIGUSR1);
            }else if(buffer[0] == 'k'){
                //create
                level = atoi(buffer+1);
                kill(pid[level],SIGUSR2);
            }else if(buffer[0] == 'p'){
                printAll();
            }
        }
    }else{
        while(1);
    }

    return 0;
}