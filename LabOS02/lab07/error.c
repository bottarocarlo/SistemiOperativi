#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#define MAX_ARG 10
#define MAX_DEPTH 5
#define MAX_BROADTH 10
#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"

extern int errno;

int main(int argc,char ** argv){
    int sys = kill (3333,SIGUSR1);
    if(sys==-1){
        fprintf(stderr,"%s errno=%d%s\n",RED,errno,DF);
        perror("Error printed by error:");
        fprintf(stderr,"%s%s%s\n",RED,strerror(errno),DF);
    }else{
        printf("%sbad lucky%s",GREEN,DF);
    }


    return 0;
}