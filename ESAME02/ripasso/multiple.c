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
    char mtext[10];
    long mtype;
}msg;

int pid[MAX_DEPHT] , master , id;

int main(int argc,char ** argv){
    master = getpid();

    for(int i =0;i<MAX_DEPHT;i++){
        pid[i] = fork();
        if(getpid()!=master){
            printf("[%d][%d]create!\n",i,getpid());
            id = i;
            break;
        }
    }

    if(getpid()==master){
        
    }

    return 0;
}