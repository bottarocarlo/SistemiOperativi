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

extern int errno;
int br[MAX];
int fd;
char * fifoName= "/tmp/unique";

void quit(int depth){
    for(int i=1;i<= depth ;i++){
        printf("[%d][%d]killed\n",i,br[i]);
        kill(br[i],SIGTERM);
    }
    while (wait(NULL)>0);
    exit(0);
}

void sender(int signo){
    printf("[%d]is sending..\n",getpid());
    mkfifo(fifoName,S_IRUSR|S_IWUSR);
    fd = open(fifoName,O_WRONLY);
    write(fd,"ciao",5);
    close(fd);
}

void receiver(int signo){
    printf("[%d]is receiving..\n",getpid());
    char appo[10];
    mkfifo(fifoName,S_IRUSR|S_IWUSR);
    fd = open(fifoName,O_RDONLY);
    read(fd,appo,sizeof(appo));
    printf("received %s\n",appo);
    close(fd);
}

int main(int argc,char ** argv){
    if(argc<2){
        printf("%susage%s",RED,DF);
        exit(0);
    }
    int send;
    int receive;
    char buffer[10];
    signal(SIGUSR1,sender);
    signal(SIGUSR2,receiver);

    br[0] = getpid();
    int depth = atoi(argv[1]);
    for(int i =1 ; i <= depth; i++){
        br[i]= fork();
        if(br[i]==0){
            break;
        }
    }
    if(getpid()==br[0]){
        for (int i = 0; i < depth; i++)
        {
            /* code */
            printf("[%d][%d]\n",i,br[i]);
        }
        do{
            //sono il padre
            printf("Next command:\n");
            read(STDIN_FILENO,buffer,10);
            if(buffer[0]=='q'){
                quit(depth);
                break;
            }else{
                switch (buffer[1])
                {
                case '>':
                    send = atoi(&buffer[0]);
                    receive = atoi(&buffer[2]);
                    kill(br[send],SIGUSR1);
                    sleep(1);
                    kill(br[receive],SIGUSR2);
                    sleep(1);
                    //printf("[%d]%c[%d]\n",send,buffer[1],receive);
                    break;
                case '<':
                    send = atoi(&buffer[2]);
                    receive = atoi(&buffer[1]);
                    kill(br[send],SIGUSR1);
                    sleep(1);
                    kill(br[receive],SIGUSR2);
                    sleep(1);
                    //printf("[%d]%c[%d]\n",send,buffer[1],receive);
                    break;
                default:
                    break;
                }
            }
        }while(1);
    }else{
        while(1);
    }
    return 0;
}