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
    int fd;
    char * fifoname = "/tmo/fifo1";
    char str1[80],* str2 = "I'm a writer";
    mkfifo(fifoname,S_IRUSR|S_IWUSR);
    fd = open(fifoname,O_RDONLY);
    write(fd,str2,strlen(str2)+1);
    close(fd);
    fd = open(fifoname, O_RDONLY);
    read(fd,str1,sizeof(str1));
    printf("reader is writing: %s\n",str1);
    close(fd);

    return 0;
}