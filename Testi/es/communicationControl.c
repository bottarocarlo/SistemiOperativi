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
#include <errno.h>

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

#define MAX 5

//----------------------------
int depth,master,proc,level;
int input[MAX][2],output[MAX][2],br[MAX];
char buf[4],pid[4];
//----------------------------

int main(int argc,char ** argv){
    if(argc < 2){
        printf("error usage\n");
        exit (-1);
    }
    depth = atoi(argv[1]);
    if(depth>10){
        printf("sei un cretino");
        exit(-2);
    }

    //creating children 
    master = getpid();
    for(int i =0;i<depth;i++){
        pipe(input[i]);
        pipe(output[i]);
        br[i] = fork();
        if(master == getpid()){
            //father
            close(input[i][WRITE]);
            close(output[i][READ]);
        }else{
            //children
            close(input[i][READ]);
            close(output[i][WRITE]);
            proc = i;
            printf("[%d][%d]created\n",proc,getpid());
            break;
        }
    }
    sleep(1);
    if(master == getpid()){
        while(1){
            printf("Next command:\n");
            read(STDIN_FILENO,buf,4);
            switch (buf[0])
            {
            case 'i':
                /* pid */
                level = atoi(&buf[1]);
                write(output[level][WRITE],"i",sizeof("i"));
                sleep(1);
                read(input[level][READ],pid,4);
                printf("Child %d told me: %s\n",level,pid);
                break;
            case 'r':
                /* random */
                level = atoi(&buf[1]);
                write(output[level][WRITE],"r",sizeof("r"));
                sleep(1);
                read(input[level][READ],pid,4);
                printf("Child %d told me: %s\n",level,pid);
                break;
            case 'q':
                /* quit */
                for(int i =0;i<depth;i++){
                    kill(br[i],SIGTERM);
                    printf("[%d][%d] killed\n",i,br[i]);
                }
                return 0;
                break;
            default:
                printf("%swrong parameter%s\n",RED,DF);
                break;
            }

        }
    }else{
        while(1){
            srand(time(NULL));
            read(output[proc][READ],buf,1);
            switch (buf[0])
            {
            case 'i':
                /* pid */
                printf("%sChild is sending is own pid%s\n",GREEN,DF);
                sprintf(pid,"%d",getpid());
                write(input[proc][WRITE],pid,strlen(pid));
                break;
            case 'r':
                /* random */
                printf("%sChild is computing random%s\n",GREEN,DF);
                sprintf(pid,"%d",rand()%100);
                write(input[proc][WRITE],pid,strlen(pid));
                break;
            default:
                break;
            }
        }
    }

    return 0;
}