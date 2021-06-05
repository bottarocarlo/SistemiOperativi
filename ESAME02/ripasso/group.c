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

#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"

void term(int sigNum){
    printf("%s[%d] finished%s\n",RED,getpid(),DF);
    exit(0);
}

int main(int argc,char ** argv){
    int master[5];

    signal(SIGTERM,term);

    master[0]= getpid();
    printf("%s[%d] created%s\n",GREEN,getpid(),DF);
    for(int i =1;i <5;i++){
        master[i]=fork();
        if(master[0]!=getpid()){
            printf("%s[%d] created%s\n",GREEN,getpid(),DF);
            break;
        }
    }

    if(master[0]==getpid()){
        sleep(1);
        printf("/-----waiting-----/\n");
        sleep(1);
        kill(-master[0],SIGTERM);

    }else{
        //children
        while (1);
    }
    while(wait(NULL)>0);
    return 0;
}