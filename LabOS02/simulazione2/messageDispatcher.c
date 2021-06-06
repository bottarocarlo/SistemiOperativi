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

#define READ 0
#define WRITE 1
//color
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYANO "\033[0;36m"
#define WHITE "\033[0;37m"
#define DF "\033[0m"

#define MAX_DEPHT 5

int master , pid[MAX_DEPHT], numChildren=0 ,proc;
char buffer[20] , mem[20];
int pip[MAX_DEPHT][2];

void loop(){
    int r;
    int len;
    r = read(pip[proc][READ],&buffer,30);
    while(mem[len]!='\n'){
        len++;
    }
    if(r!=-1){
        printf("[CHD] receive:%s",buffer);fflush(NULL);
    }
    exit(0);
}

int isString(){
    for(int i =0; buffer[i]!='\n';i++){
        if(buffer[i]<'0' || buffer[i]>'9')
            return 1;
    }
    return 0;
}

void quit(int sigNum){
    for(int i=0;i<numChildren;i++){
        kill(pid[i],SIGTERM);
    }
    while(wait(NULL)>0);
    exit(0);
}

void handler(int sigNum){
    int len = 0;
    do{
        len++;
    }while(mem[len]!='\n');
    for(int i =0;i<numChildren;i++){
        sprintf(buffer,"%d%d%s",pid[i],i,mem);
        write(pip[i][WRITE],buffer,30);
    }
    numChildren=0;
}

int main(int argc,char ** argv){
    signal(SIGTSTP,quit);
    signal(SIGUSR1,handler);

    printf("%s[MAIN] '%d'%s\n",GREEN,getpid(),DF);
    master = getpid();

    while(1){
        read(STDIN_FILENO,buffer,20);
        switch (isString())
        {
        case 1:
            /* string */
            printf("%s[MAIN]msg saved%s\n",YELLOW,DF);
            strcpy(mem,buffer);
            break;
        case 0:
            /* number */
            if(numChildren<MAX_DEPHT){
                pipe(pip[numChildren]);
                pid[numChildren] = fork();
                if(getpid()!=master){
                    proc = numChildren;
                    printf("%s[CHD] created%s\n",GREEN,DF);
                    loop();
                }
                numChildren++;
            }else{
                printf("too much children!\n");
            }
            break;

        
        default:
            break;
        }
        sleep(1);
    }




    return 0;
}