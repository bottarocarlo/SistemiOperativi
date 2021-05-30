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

#define MAX 5
#define READ 0
#define WRITE 1
#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"


int main(){

    int fd;
    char * fifoName= "/tmp/unique";
    mkfifo(fifoName,S_IRUSR|S_IWUSR);
    //--------------------------
    


    return 0;
}