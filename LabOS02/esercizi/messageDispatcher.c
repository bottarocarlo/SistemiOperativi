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
#define READ 0
#define WRITE 1

#define MAX 5

int master , pid[MAX] ,numChildren ,pip[MAX][2],proc;
char buffer[10];
char mem[10];
int sender, receiver;

struct msg_buffer
{
    char payload[3];
    int type;
}msg;

void loop(){
    int r;
    while(1){
        r = read(pip[proc][READ],&buffer,10);
        if(r != -1){
            printf("%s[CHD] I received this message from MAIN: %s%s",YELLOW,buffer,DF);
            fflush(NULL);
        }
    }
    exit(0);
}

int isChar(){
    int len;
    for(int i =0 ; i<strlen(buffer);i++){
        if(buffer[i]=='\n'){
            len = i;
            break;
        }
    }
    for(int i =0 ;i<len;i++){
        if(buffer[i]<='0' || buffer[i]>='9'){
            return 0;
        }
    }
    return 1;
}

void handler(int sigNum){
    for(int i =0;i<numChildren;i++){
        write(pip[i][WRITE],mem,strlen(mem));
    }
    sleep(1);
    for(int i = 0; i <numChildren ; i++){
        kill(pid[i],SIGKILL);
        printf("%s[CHD][%d] terminated!%s\n",RED,pid[i],DF);
    }
    numChildren=0;
}

void term(int sigNum){
    if(getpid()==master){
        for(int i =0;i<numChildren;i++){
            kill(pid[i],SIGTERM);
            printf("%s[%d] killed%s\n",RED,pid[i],DF);
        }
        while(wait(NULL)>0);
        exit(0);
    }
}

int main(int argc,char ** argv){
    signal(SIGUSR1,handler);
    signal(SIGUSR2,handler);
    signal(SIGTSTP,term);
    signal(SIGINT,term);
    master = getpid();
    printf("%s[MAIN] Hi, nice to meet you, I'm %d%s\n",GREEN,getpid(),DF);
    do{
        read(STDIN_FILENO,buffer,10);
        if(isChar()==0){
            //è un char
            strcpy(mem,buffer);
            printf("%s[MAIN] msg saved!%s\n",GREEN,DF);
        }else{
            //è un numero
            if(numChildren<MAX){
                pipe(pip[numChildren]);
                pid[numChildren] = fork();
                if(getpid()!=master){
                    proc = numChildren;
                    printf("%s[CHD] I'm a new child with Id : %d, and I'm waiting for a message%s\n",YELLOW,getpid(),DF);
                    loop();
                }
                numChildren++;
            }
        }

    }while(1);


    return 0;
}