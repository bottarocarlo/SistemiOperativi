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

#define MAX_ARG 10
#define MAX_DEPTH 5
#define MAX_BROADTH 10
#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"

struct msg_buffer{
    long mtype;
    char mtext[100];
}msg;

int main(){
    creat("/tmp/unique", O_CREAT|O_EXCL);
    key_t queueKey = ftok("/tmp/unique",1);
    printf("%s[RCV][queue key]: %d%s\n",GREEN,queueKey,DF);
    int queueId = msgget(queueKey,0777|IPC_CREAT|IPC_EXCL);
    //msg created 
    printf("%s[RCV][queue ID]: %d%s\n",GREEN,msgget(queueKey,0777),DF);
    msgrcv(queueId,&msg,sizeof(msg.mtext),20,0);
    printf("%s[RCV][msg.txt]:%s%s",GREEN,msg.mtext,DF);

    return 0;
}