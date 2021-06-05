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
#define MAX_DEPHT 5
#define WR 1
#define RD 0

int pid[MAX_DEPHT];
int proc;
int children;

void quit(){
    for(int i=1 ;i < children ; i++){
        kill(pid[i],SIGTERM);
        printf("%s[%d] killed.. %s\n",RED,pid[i],DF);
    }
    printf("%s[MAIN][%d] killed.. %s\n",RED,getpid(),DF);
    exit(0);
}

int main(int argc,char ** argv){
    char buffer[3];
    int outcome;
    int level,r,random,pido;
    if(argc<2){
        fprintf(stderr,"%susage ./communicationControl.o # %s\n",RED,DF);
    }
    children = atoi(argv[1]);
    if(children>MAX_DEPHT){
        fprintf(stderr,"%stoo much children!%s\n",RED,DF);
    }
    int input[children][2],output[children][2];
    pid[0] = getpid();
    printf("%s[%d] : MAIN%s\n",GREEN,getpid(),DF);
    for(int i = 1; i<children;i++){
        pipe(input[i]);
        pipe(output[i]);
        pid[i] = fork();
        if(getpid()!=pid[0]){
            proc = i;
            printf("%s[%d] created%s\n",GREEN,getpid(),DF);
            close(input[i][RD]);
            close(output[i][WR]);
            break;
        }else{
            close(input[i][WR]);
            close(output[i][RD]);
        }
    }

    if(pid[0] == getpid() ){
        //padre
        sleep(1);
        while(1){
            printf("Next command: \t");fflush(NULL);
            read(STDIN_FILENO,buffer,3);
            if(buffer[0]=='q'){
                quit();
            }else if(buffer[0]=='i' || buffer[0]=='r'){
                level = atoi(buffer+1);
                if(level<=children){
                    write(output[level][WR],buffer,2); 
                    sleep(1);
                    read(input[level][RD],&outcome,2); 
                    printf("%sChild %d told me: '%d'%s\n",GREEN,pid[level],(int)outcome,DF);
                }else{
                    fprintf(stderr,"%sWrong target%s\n",RED,DF);
                }
            }else{
                fprintf(stderr,"%sSomething went wrong retry%s\n",RED,DF);
            }
            sleep(1);
        }

    }else{
        //figlio
        
        while (1){
            srand(time(NULL));
            r = read(output[proc][RD],buffer,254);
            if(r!=-1){
                switch (buffer[0])
                {
                case 'r':
                    /* random */
                    random = rand() % 100;
                    printf("[%d] CHD computing random....\n",getpid());
                    write(input[proc][WR],&random,2);
                    break;
                case 'i':
                    /* getting pid */
                    pido = getpid();
                    printf("[%d] CHD sending pid....\n",getpid());
                    write(input[proc][WR],&pido,2);
                    break;
                
                default:
                    break;
                }
            }
        }

    }
    return 0;
}