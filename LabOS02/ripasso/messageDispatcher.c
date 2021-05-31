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
#include <pthread.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYANO "\033[0;36m"
#define WHITE "\033[0;37m"
#define DF "\033[0m"
#define MAX 5
#define RD 0
#define WR 1
//0 -> false
//1 -> true

int numChildren=0;
int pid[MAX];
int master;
char buffer[20];
int pip[MAX][2];
int proc;
char mem[20];

void loop(){
    int r;
    while (1){
        r = read(pip[proc][RD],buffer,20);
        if(r!=-1){
            printf("%s[%d][%d]I received the following sentence: %s%s",MAGENTA,getpid(),proc,buffer,DF);
            fflush(NULL);
        }
    }
}

int isNumber(char * str){
    int len=0;
    for(int i =0; str[i] != '\n';i++){
        len++;
    }

    for(int i=0; i<len;i++){
        if(str[i]<'0' || str[i]>'9'){
            return 0;//è una stringa
        }
    }
    return 1;//true è un numero
}


void ctrlc(int sigNum){
    for(int i =0 ;i<numChildren;i++){
        kill(pid[i],SIGTERM);
        printf("%s[%d] killed %s\n",RED,pid[i],DF);
        fflush(stdout);
    }
    while(wait(NULL)>0);
    printf("%s[MAIN][%d] killed %s\n",RED,master,DF);
    exit(0);
}

void* my_fun(){
    for(int i =0; i<numChildren ; i++){
        write(pip[i][WR],mem,20);
    }
    sleep(1);
    for(int i = 0; i <numChildren ; i++){
        kill(pid[i],SIGKILL);
        printf("%s[CHD][%d] terminated!%s\n",RED,pid[i],DF);
    }
    numChildren=0;
    char * str = "Returned string";
    pthread_exit((void *)str);
}


void handler(int sigNum){
    pthread_t t_id;
    //void * retFromThread; //This must be a pointer to void! 
    pthread_create(&t_id, NULL, my_fun, (void *)&t_id); //Create 
    pthread_join(t_id,NULL); // wait thread
}

int main(int argc,char ** argv){
    signal(SIGTSTP,ctrlc);
    signal(SIGINT,ctrlc);
    signal(SIGUSR2,handler);
    signal(SIGUSR1,handler);
    printf("%s[MAIN][%d]%s\n",GREEN,getpid(),DF);  
    fflush(NULL);
    master = getpid();
    
    while(1){
        read(STDIN_FILENO,buffer,20);
        if(isNumber(buffer)==1){
            //is number
            if(numChildren<MAX){
                pipe(pip[numChildren]);
                pid[numChildren]=fork();
                if(getpid()!=master){
                    proc = numChildren;
                    printf("%s[CHD][%d] created %s\n",YELLOW,getpid(),DF);
                    loop();
                }
                numChildren++;
            }else{
                fprintf(stderr,"too much children!\n");
            }
        }else{
            //is string
            printf("%s[MAIN] Msg saved! %s\n",GREEN,DF);
            strcpy(mem,buffer);
        }

    }

    return 0;
}