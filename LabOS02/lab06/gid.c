#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc , char ** argv){
    int child = !fork();
    printf ("PID : %d \nGID : %d\n",getpid(),getpgid(0));
    if(child){
        child = !fork();
        if(!child){
            setpgid(0,getpid()); //become group leader
            fork();
            printf ("PID : %d \nGID : %d\n",getpid(),getpgid(0));
        }
    }
    while(wait(NULL)>0);
    return 0;
}