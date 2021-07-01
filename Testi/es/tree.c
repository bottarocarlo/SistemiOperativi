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

#define MAX_DEPHT 5

//--------------------
int br[MAX_DEPHT],proc,master,level,children[MAX_DEPHT],numChildren=0;
char buffer[3];
//--------------------
void tab(int x){
    for(int i =0;i<x;i++){
        printf("\t");fflush(NULL);
    }
}
void createLvl(int signum){
    if(numChildren>5){
        printf("too much children..\n");
    }else{
        children[numChildren] = fork();
        if(children[numChildren]==0){
            exit(0);
        }else{
            numChildren++;
            printf("%s[%d]created children at level %d%s\n",GREEN,getpid(),proc,DF);
        }
    }
}
void print(int signum){
    for(int i =0;i<numChildren;i++){
        tab(proc);
        printf("[ID %d - Parent %d] depth %d\n",children[i],getpid(),proc);
    }
}
void killLvl(int signum){
    numChildren=0;
}

int main(int argc,char ** argv){
    signal(SIGUSR1,createLvl);
    signal(SIGUSR2,print);
    signal(SIGALRM,killLvl);


    master = getpid();
    for( int i=0 ; i<MAX_DEPHT ; i++ ){
        br[i] = fork();
        if(getpid()!=master){
            //child
            setpgid(0,getpid());
            printf("%s[%d]created!%s \n",GREEN,getpid(),DF);
            proc = i;
            break;
        }
    }
    sleep(1);
    if(getpid()==master){
        //father
        while(1){
            
            printf("Next command:\n");
            read(STDIN_FILENO,buffer,3);
            switch (buffer[0])
            {
            case 'c':
                /* create */
                level=atoi(&buffer[1]);
                kill(br[level],SIGUSR1);
                break;
            case 'k':
                /* kill level */
                level=atoi(&buffer[1]);
                kill(br[level],SIGALRM);
                break;
            case 'p':
                /* code */
                printf("[ID %d - Parent 0] depth 0\n",br[0]);
                for(int i = 1;i<MAX_DEPHT;i++){
                    tab(i);
                    printf("[ID %d - Parent %d] depth %d\n",br[i],br[i-1],i);
                    kill(br[i],SIGUSR2);
                }
                break;
            case 'q':
                /* quit */
                for(int i=0;i<MAX_DEPHT;i++){
                    kill(br[i],SIGTERM);
                    printf("%s[%d]killed!%s \n",RED,br[i],DF);
                }
                while(wait(NULL)>0);
                return 0;
                break;
            default:
                break;
            }
            sleep(1);
        }
    }else{
        //children
        while(1);
    }


    return 0;
}