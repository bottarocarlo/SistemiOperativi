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
#include <errno.h>

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

#define MAX 10

int master;

int main(int argc,char ** argv){
    if(argc < 2){
        fprintf(stderr,"error usage...");
        exit(-1);
    }
    char * fifoname = "/tmo/fifo1";
    mkfifo(fifoname,S_IRUSR|S_IWUSR);
    int numChildren = atoi(&argv[1]);
    printf("creating %d children..\n",numChildren);
    int br[numChildren];
    master = getpid();
    for(int i =0 ;i<numChildren;i++ ){
        br[i] = fork();
        if(getpid()!=master){
            break;
        }
    }

    if(getpid()==master){
        //master
    }else{
        while (1);
    }
    return 0;
}