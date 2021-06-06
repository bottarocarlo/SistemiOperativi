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
    char text[100];
}msg1,msg2;

int queueId;

int main(int argc,char **argv){
    
   //creazione della coda
    creat("/tmp/tree",0777);
	key_t k = ftok("/tmp/tree",1);
	queueId=msgget(k,0777|IPC_CREAT); 
    
    printf("%s[SND][queue key]: %d%s\n",GREEN,k,DF);
    printf("%s[SND][queue ID]: %d%s\n",GREEN,queueId,DF);
    
    if(fork()==0){
        strcpy(msg1.text,"sending message");
        msg1.type = 1;
        int esito = msgsnd(queueId , &msg1, sizeof(msg1.text),0);
        if (esito>=0){
            //printf("%s[SND][msg.txt]:%s%s\n",GREEN,msg1.text,DF);
        }else{
            fprintf(stderr,"%ssend error%s\n",RED,DF);
        }
        while(1);
    }else{
        //while(wait(NULL)>0);
        long id = 1;
        msgrcv(queueId,&msg2,sizeof(msg2.text),id,0);
        printf("%s[RCV][msg.txt]:%s%s\n",GREEN,msg2.text,DF);
    }
    msgctl(queueId, IPC_RMID, NULL);
    return 0;
}