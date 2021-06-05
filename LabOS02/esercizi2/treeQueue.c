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

#define READ 0
#define WRITE 1
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

extern int errno;
int id;
int pid[MAX_DEPHT],children[MAX_DEPHT],numChildren,father;

char buffer[4];

struct msg_buffer
{
    char mtext[10];
    long mtype;
}msgS,msgR;

void loop(){
    while(1);
}

void tab(int x){
    for(int i =0;i<x;i++){
        printf("\t");fflush(NULL);
    }
}

void quit(){
    for(int i =1;i<MAX_DEPHT;i++){
        printf("%s[%d] killed!%s\n",RED,pid[i],DF);
        kill(-pid[i],SIGTERM);   
    }
    while(wait(NULL)>0);
}

void print(int sigNum){
    for(int i =0;i<numChildren;i++){
        tab(id);
        printf("%s[ID %d - Parent %d] depht %d%s\n",GREEN,children[i],father,id,DF);
    }
}

int main(int argc,char ** argv){
    signal (SIGUSR1, print);
    int r;
    long level;
    creat("/tmp/tree",0777);
	key_t k = ftok("/tmp/tree",1);
	int queueId=msgget(k,0777|IPC_CREAT);

    if(queueId==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }

    pid[0] = getpid();
    for(int i=1;i<MAX_DEPHT;i++){
        father = getpid();
        pid[i] = fork();
        if(getpid()!=pid[0]){
            setpgid(0,getpid());
            id = i;
            printf("%s[%d] created!%s\n",GREEN,getpid(),DF);
            break;
        }
    }

    if(pid[0] == getpid()){
        while(1){
            sleep(1);
            printf("Next command:\t");fflush(NULL);
            read(STDIN_FILENO,buffer,3);
            if(buffer[0]=='q'){
                /* quit */
                quit();
                msgctl(queueId, IPC_RMID, NULL);
                exit(0);
            }else if(buffer[0]=='k' || buffer[0]=='c'){
                level = atoi(buffer+1);
                strcpy(msgS.mtext,buffer);
                msgS.mtype = level;
                int esito = msgsnd(queueId , &msgS, sizeof(msgS.mtext),0);
                if (esito<0){
                    fprintf(stderr, "errno = %d\n", errno);
                    perror("Error printed by perror");
                    fprintf(stderr,"Strerror: %s\n", strerror(errno));
                }
            }else if(buffer[0] == 'p'){
                //printa
                for(int i =1;i<MAX_DEPHT;i++){
                    tab(i);
                    printf("%s[ID %d - Parent %d] depht %d%s\n",GREEN,pid[i],pid[i-1],i,DF);
                    kill(pid[i],SIGUSR1);
                }
            }else{
                printf("Something went wrong...\n");
            }
        }
    }else{
        while(1){
            msgrcv(queueId,&msgR,sizeof(msgR.mtext),0,id);
            switch (msgR.mtext[0])
            {
            case 'c':
                /* create */
                printf("[%d] create..\n",getpid());
                children[numChildren] = fork();
                if(children[numChildren] == 0){
                    loop();
                }
                numChildren++;
                break;
            case 'k':
                /* kill */
                printf("[%d] kill..\n",getpid());
                break;
            default:
                break;
            }
            
        }
        
    }

    return 0;
}