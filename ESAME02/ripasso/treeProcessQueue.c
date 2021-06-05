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
#define MAX_DEPHT 5

int pid[MAX_DEPHT];
extern int errno;
int depth=0;
int children[MAX_DEPHT];
int numChildren=0;

struct msg_buffer
{
    int type;
    char payload;
}msg;

void loop(){
    while(1);
    exit(-2);
}

void quit(){
    printf("%s[%d] killed%s\n",RED,getpid(),DF);
    for(int i=1;i<MAX_DEPHT;i++){
        kill(-pid[i],SIGTERM);
        printf("%s[%d] killed%s\n",RED,pid[i],DF);
    }
}


int main(int argc,char ** argv){
    int level;
    int esito;
    creat("/tmp/tree",0777);
    key_t k = ftok("/tmp/tree",1);
    int queueId=msgget(k,0777|IPC_CREAT);
    if(queueId==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }

    pid[0]= getpid();
    printf("%s[%d] created%s\n",GREEN,getpid(),DF);
    for(int i =1;i<MAX_DEPHT;i++){
        pid[i] = fork();
        if(pid[0]!=getpid()){
            depth = i;
            printf("%s[%d] created%s\n",GREEN,getpid(),DF);
            setpgid(0,getpid());
            break;
        }
    }

    if(getpid()==pid[0]){
        //sono il padre
        char buffer[10];
        while(1){
            sleep(1);
            printf("Next command:\t");
            fflush(NULL);
            read(STDIN_FILENO,buffer,10);
            if(buffer[0]=='q'){
                //killAll
                quit();
                msgctl(queueId, IPC_RMID, NULL);
                while(wait(NULL)>0);
                exit(0);
            }else if(buffer[0]=='c'){
                //create
                msg.type = atoi (buffer+1);
                msg.payload = 'c';
                if (msgsnd(queueId , &msg, sizeof(msg.payload),0) == -1)
                    perror("msgop: msgsnd failed");
            }
        }
    }else{
        //figli
        do{
            esito = msgrcv(queueId, &msg, 2, 1, 0);
            if(esito!=-1){
                switch (msg.payload)
                {
                case 'c':
                    /* code */
                    children[numChildren]= fork();    
                    if(getpid()!=pid[depth]){
                        printf("%s[%d] created%s\n",GREEN,getpid(),DF);
                        loop();
                    }
                    numChildren++;
                    break;
                case 'k':
                    /* code */
                    for(int i =0;i<numChildren;i++){
                        kill(children[i],SIGTERM);
                        printf("%s[%d] killed%s\n",GREEN,children[i],DF);
                    }
                    numChildren=0;
                    break;
                
                default:
                    break;
                }
            }
            sleep(1);
        }while(1);
    }


    return 0;

}