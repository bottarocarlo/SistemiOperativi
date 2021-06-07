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

struct msg_buffer
{
    char mtext[20];
    long mtype;
}msg;

extern int errno;
int pid[MAX_DEPHT] , master ;
long id;

int main(int argc,char ** argv){
    master = getpid();

    creat("/tmp/unique",0777);
	key_t k = ftok("/tmp/unique",1);
	int queueId=msgget(k,0777|IPC_CREAT);

    if(queueId==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }

    for(int i =0;i<MAX_DEPHT;i++){
        pid[i] = fork();
        if(getpid()!=master){
            printf("[%d][%d]create!\n",i,getpid());
            id = i;
            break;
        }
    }

    if(getpid()==master){

        for(int i =0;i<MAX_DEPHT;i++){
            printf("[MAIN]sending message to: %d!\n",i);
            strcpy(msg.mtext,"first message");
            msg.mtype=i;
            
            int esito = msgsnd(queueId , &msg, sizeof(msg.mtext),0);
            if (esito<0){
                fprintf(stderr, "errno = %d\n", errno);
                perror("Error printed by perror");
                fprintf(stderr,"Strerror: %s\n", strerror(errno));   
            }
            sleep(1);
        }
        while(wait(NULL)>0);

    }else{
        int outcome = msgrcv(queueId,&msg,sizeof(msg.mtext),0,id);
        printf("[RCV]:%s\n",msg.mtext);
    }

    return 0;
}