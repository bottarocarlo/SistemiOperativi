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

#define MAX_DEPHT 7
#define MAX_GRANDCHILDREN 5
int pid[MAX_DEPHT] , master , numChildren = 0;
int grandchildren[MAX_GRANDCHILDREN] , numGrandchildren;
int id;

void tab(int x){
    for(int i =0;i<x;i++){
        printf("\t");fflush(NULL);
    }
}

void loop(){
    while (1);
    exit(0);
}

void quit(){
    for(int i =0;i<MAX_DEPHT;i++){
        kill(-pid[i],SIGTERM);
        printf("%s[%d] killed%s\n",RED,pid[i],DF);
    }
}

void create(int sigNum){
    
    grandchildren[numGrandchildren] = fork();
    if(grandchildren[numGrandchildren]==0){
        printf("%s[%d] created at level %d%s\n",GREEN,getpid(),id,DF);
        loop();
    }
    numGrandchildren++;
}

void killL(int sigNum){
    for(int i=0;i<numGrandchildren;i++){
        kill(grandchildren[i],SIGTERM);
        printf("%s[%d] killed at level %d%s\n",RED,pid[i],id,DF);
    }
    numGrandchildren=0;
}

void print(int sigNum){

    for(int i=0;i<numGrandchildren;i++){
        tab(id);
        printf("%s[ID %d - Parent %d] depht %d%s\n",GREEN,grandchildren[i],master,id,DF);
    }
}



int main(int argc,char ** argv){
    int level;

    signal(SIGUSR1,create);
    signal(SIGUSR2,killL);
    signal(SIGALRM,print);

    char buffer[10];
    master = getpid();
    for(int i =0;i<MAX_DEPHT;i++){
        pid[i] = fork();
        if(getpid()!=master){
            id = i;
            setpgid(0,getpid());
            printf("%s[%d] created!%s\n",GREEN,getpid(),DF);
            break;
        }
    }

    if(master == getpid()){
        while(1){
            sleep(1);
            printf("Next command:\t");fflush(NULL);
            read(STDIN_FILENO,buffer,10);
           switch (buffer[0])
           {
           case 'q':
               /* quit */
               quit();
                while(wait(NULL)>0);
                exit(0);
               break;
           case 'k':
               /* kill */
               level = atoi(buffer+1);
               kill(pid[level],SIGUSR2);
               break;
            case 'c':
                /* create */
                level = atoi(buffer+1);
                kill(pid[level],SIGUSR1);
                break;
            case 'p':
                /* print */
                printf("%s[ID %d - Parent 0] depht %d%s\n",GREEN,pid[0],0,DF);
                for(int i=1;i<MAX_DEPHT;i++){
                    tab(i);
                    printf("%s[ID %d - Parent %d] depht %d%s\n",GREEN,pid[i],pid[i-1],i,DF);
                    kill(pid[i],SIGALRM);
                }
                break;
            default:
                printf("wrong command\n");
                break;
           }
        }
        while(wait(NULL)>0);
    }else{
        while(1);
    }

    return 0;
}