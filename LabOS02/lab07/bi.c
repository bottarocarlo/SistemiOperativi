#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"
#define READ 0
#define WRITE 1


int main(){
    int fd[2];
    int esito = pipe(fd); //Create unnamed pipe 
    if(esito == 0){
        write(fd[1],"writing",8); //Write to pipe using fd[1] 
        char buf[50];
        int c = read(fd[0],&buf,50); //Read from pipe using fd[0] 
        printf("Read '%s' (%d)\n",buf,c);
    }
    return 0;
}