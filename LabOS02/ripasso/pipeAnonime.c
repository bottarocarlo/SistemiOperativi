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

int main(int argc,char ** argv){

    /*
    int fd[2];
    int esito = pipe(fd); //Create unnamed pipe 
    if(esito == 0){
        write(fd[1],"writing",8); //Write to pipe using fd[1] 
        char buf[50];
        int c = read(fd[0],&buf,50); //Read from pipe using fd[0] 
        printf("Read '%s' (%d)\n",buf,c);
    }
    */

    int pipe1[2], pipe2[2]; char buf[50];
    pipe(pipe1); pipe(pipe2); //Create two unnamed pipe
    int p2 = !fork();
    if(p2){
        close(pipe1[WRITE]); close(pipe2[READ]);
        int r = read(pipe1[READ],&buf,50); //Read from pipe 
        close(pipe1[READ]); 
        printf("P2 received: ‘%s’\n",buf); 
        write(pipe2[WRITE],"Msg from p2",12); // Writes to pipe 
        close(pipe2[WRITE]);
    }else{
        close(pipe1[READ]); close(pipe2[1]); 
        write(pipe1[WRITE],"Msg from p1",12); // Writes to pipe 
        close(pipe1[WRITE]);
        int r = read(pipe2[READ],&buf,50); //Read from pipe 
        close(pipe2[READ]); printf("P1 received: ‘%s’\n",buf);
    }
    
    while(wait(NULL)>0);

    return 0;
}