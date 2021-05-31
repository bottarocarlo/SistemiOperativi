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

#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"
#define MAX 5
//0 -> false
//1 -> true

int numChildren;
int pid[MAX];

int isString(char *buffer){
    while(buffer[i]=='\n'){
        if(buffer[i]<=0 || buffer[i]>=9){
            return 1;
        }
    }
    return 0;
}

void ctrlc(int sigNum){
    for(int i =1 ;i<numChildren;i++){
        kill(pid[i],SIGTERM);
        printf("%s[%d] killed %s\n",RED,pid[i],DF);
    }
    printf("%s[MAIN][%d] killed %s\n",RED,pid[i],DF)
}

int main(int argc,char ** argv){
    char buffer[20];
    signal(SIGINT,ctrlc);
    printf("%s[MAIN][%d]%s\n",GREEN,getpid(),DF);    
    pid[0] = getpid();


    while(1){
        read(STDIN_FILENO,buffer,20);
        if(isString(buffer)==0){
            //is number
            printf("is number\n");
        }else{
            //is string
            printf("is string\n");
        }

    }

    return 0;
}