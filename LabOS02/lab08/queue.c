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

struct msg_buffer{
    long mtype;
    char mtext[100];
}msgp,msgpRcv;

int main(){
    if(fork()==0){
        //child
        remove("/tmp/unique");
        strcpy(msgp.mtext,"my fist message");
        msgp.mtype=1;
        creat("/tmp/unique", O_CREAT|O_EXCL);
        key_t queueKey = ftok("/tmp/unique",2);
        printf("%s[SND][queue key]: %d%s\n",GREEN,queueKey,DF);
        int queueId = msgget(queueKey,0777|IPC_CREAT|IPC_EXCL);
        //msgctl(queueId,IPC_RMID,NULL);
        //msg created 
        printf("%s[SND][quue ID]: %d%s\n",GREEN,msgget(queueKey,0777),DF);
        //get queue ID from Key by ometting IPC_CREAT
        //se metto IPC_EXCL non fa nulla perchè già la coda esiste con quell'id

        //SEND
        int esito = msgsnd(queueId , &msgp, sizeof(msgp.mtext),0);
        
        if (esito>=0){
            
            printf("%s[SND][msg.txt]:%s%s\n",GREEN,msgp.mtext,DF);
            
        }else{
            fprintf(stderr,"%ssend error%s\n",RED,DF);
        }
        exit(0);
    }else{
        while(wait(NULL)>0);
        
        creat("/tmp/unique", O_CREAT|O_EXCL);
        
        key_t queueKey = ftok("/tmp/unique",2);
        printf("%s[RCV][queue key]: %d%s\n",GREEN,queueKey,DF);
        int queueId = msgget(queueKey,0777);
        //msg created 
        sleep(1);
        printf("%s[RCV][queue ID]: %d%s\n",GREEN,msgget(queueKey,0777),DF);
        msgrcv(queueId,&msgpRcv,sizeof(msgpRcv.mtext),1,0);
        printf("%s[RCV][msg.txt]:%s%s\n",GREEN,msgpRcv.mtext,DF);
    }
    
    return 0;
}