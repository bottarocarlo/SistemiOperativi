#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"
#define READ 0
#define WRITE 1

int main(){


    char *fifoname = "/tmp/fifo1";

    mkfifo(fifoname,S_IRUSR|S_IWUSR);
    perror("created?");

    if(fork()==0){
        open(fifoname,O_RDONLY);
        printf("open read\n");
    }else{
        sleep(1);
        open(fifoname,O_WRONLY);
        printf("open write\n");
    }

    return 0;
}