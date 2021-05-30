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
}msgpRcv;

int main(){
    creat("/tmp/unique", O_CREAT|O_EXCL);
    key_t queueKey = ftok("/tmp/unique",1);
    int queueId = msgget(queueKey,0777);

    printf("%s[RCV][queue key]: %d%s\n",GREEN,queueKey,DF);
    printf("%s[RCV][queue id]: %d%s\n",GREEN,queueId,DF);

    int outcome = msgrcv(queueId , &msgpRcv, sizeof(msgpRcv.mtext),1,0);
    if(outcome>=0){
        printf("%s[RCV][text]%s%s\n",GREEN,msgpRcv.mtext,DF);
    }else{
        printf("%s[RCV][error]%s\n",RED,DF);
    }

    return 0;
}