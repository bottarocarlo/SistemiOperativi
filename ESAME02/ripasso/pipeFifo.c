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
#include <time.h>

int main(int argv,char ** argc){

    int fd; 
    char * fifoName = "/tmp/fifo1";
    char str1[80],* str2 = "I’m a writer";

    //write
    mkfifo(fifoName,S_IRUSR|S_IWUSR); //Create if doesn’t exist 
    fd = open(fifoName, O_WRONLY); // Open FIFO for write only 
    write(fd, str2, strlen(str2)+1); // write and close
    close(fd);
    
    //read
    fd = open(fifoName, O_RDONLY); // Open FIFO for Read only 
    read(fd, str1, sizeof(str1)); // Read from FIFO
    printf("Reader is writing: %s\n", str1);
    close(fd);

    return 0;
}