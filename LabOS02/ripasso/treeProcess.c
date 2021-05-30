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

#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"
#define MAX_DEPHT 6
#define MAX 5

struct msg_buffer
{
    char payload[3];
    int type;
}msg;

int pid[MAX_DEPHT];
int children[MAX];
int numChildren=0;
int depht;

void tab(int level){
    for(int i =0;i<level;i++){
        printf("\t");
    }
    fflush(NULL);
}

void loop(){
    while(1);
    exit(0);
}

void quit(){
    for(int i =1;i<MAX_DEPHT;i++){
        printf("%s[%d]killed%s\n",RED,pid[i],DF);
        kill(-pid[i],SIGTERM);
    }
    exit(0);
}

void create(int sigNum){
    int master = getpid();
    if(numChildren<MAX){
        children[numChildren] = fork();
        if(getpid()!=master){
            printf("%s[%d]created grandchildren at level [%d]%s\n",GREEN,getpid(),depht,DF);
            loop();
        }
        numChildren++;
    }
}

void killP(int sigNum){
    for(int i =0;i<numChildren;i++){
        printf("%s[%d]killed grandchildren at level [%d]%s\n",RED,children[i],depht,DF);
        kill(children[i],SIGTERM);
    }
    numChildren=0;
}

void print(int sigNum){
    for(int i =0;i<numChildren;i++){
            tab(depht);
            printf("%s[ID %d - Parent %d] depht %d%s\n",GREEN,children[i],getpid(),depht,DF);
    }
}

void foo(){
    for(int i =0;i<MAX_DEPHT;i++){
        tab(i);
        if(i!=0){
            printf("%s[ID %d - Parent %d] depht %d%s\n",GREEN,pid[i],pid[i-1],i,DF);
        }else{
            printf("%s[MAIN][ID %d - Parent 0] depht %d%s\n",GREEN,pid[i],i,DF);
        }
        kill(pid[i],SIGALRM);
    }
}

int main(int argc,char ** argv){
    signal(SIGUSR1,create);
    signal(SIGUSR2,killP);
    signal(SIGALRM,print);

    int level;
    char buffer[3];

    pid[0] = getpid();
    depht=0;
    for(int i =1;i<MAX_DEPHT;i++){
        pid[i] = fork();
        if(pid[0]!=getpid()){
            //sono il figlio
            printf("%s[%d]created%s\n",GREEN,getpid(),DF);
            depht=i;
            setpgid(0,getpid());
            break;
        }
    }

    if(getpid()==pid[0]){
       
       do{
            sleep(1);
           printf("Next command:\t");
           fflush(NULL);
           read(STDIN_FILENO,buffer,3);
           if(buffer[0]=='q'){
               quit();
               while(wait(NULL));
           }else if(buffer[0]=='c'){
               //create
                level = atoi(buffer+1);
                kill(pid[level],SIGUSR1);
           }else if(buffer[0]=='k'){
               //kill level
                level = atoi(buffer+1);
                kill(pid[level],SIGUSR2);
           }else if(buffer[0]=='p'){
               //print tree
                printf("printing tree..\n");
                foo();   
           }

        }while(1);

    }else{
        while (1);
    }
    return 0;
}