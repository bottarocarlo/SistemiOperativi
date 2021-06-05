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

//struct of queue
struct msg_buffer{
    long type;
    int payload;
}msg,msgR;



extern int errno;

int main(int argc,char **argv){
    srand(time(NULL));
    //creazione della coda
    //remove("/tmp/tree");
	creat("/tmp/tree",0777);
	key_t k = ftok("/tmp/tree",1);
	int queueId=msgget(k,0777|IPC_CREAT);

    if(queueId==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }
    
    printf("%s[SND][queue key]: %d%s\n",GREEN,k,DF);
    printf("%s[SND][queue ID]: %d%s\n",GREEN,queueId,DF);

    if(fork()==0){
        msg.payload=rand()%100;
        msg.type = 1;
        int esito = msgsnd(queueId , &msg, sizeof(msg.payload),0);
        if (esito>=0){
            printf("%s[SND][msg.txt]:%d%s\n",GREEN,msg.payload,DF);
        }else{
            fprintf(stderr, "errno = %d\n", errno);
            perror("Error printed by perror");
            fprintf(stderr,"Strerror: %s\n", strerror(errno));
        }
        exit(0);
    }else{
        while(wait(NULL)>0);
        msgrcv(queueId,&msg,sizeof(msg.payload),1,0);
        printf("%s[RCV][msg.txt]:%d%s\n",GREEN,msg.payload,DF);
        msgctl(queueId, IPC_RMID, NULL);
    }

    
    return 0;
}