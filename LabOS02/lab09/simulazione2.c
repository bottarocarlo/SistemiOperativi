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
#define GREEN "\033[32m"
#define DF "\033[0m"
#define MAX_DEPHT 5
#define WR 1
#define RD 0


int br[MAX_DEPHT];
int numChildren = 1;
int pip[MAX_DEPHT][2];
char buffer[20];
int openFile;

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

void loop(){
    int r;
    while(1){
        r = read(pip[numChildren][RD],buffer,20);
        if(r!=-1){
            printf("[%d]received something : %s",getpid(),buffer);
            fflush(NULL);
        }
    }
}

void quit(int sigNum){
    for(int i = 1; i <numChildren;i++){
        kill(br[i],SIGKILL);
        printf("%s[CHD][%d] terminated!%s\n",RED,br[i],DF);
    }
    while(wait(NULL)>0);
    printf("%s[MAIN][%d] terminated!%s\n",RED,getpid(),DF);
    close(openFile);
    exit(0);
}

void* my_fun(){
    for(int i =1;i<numChildren;i++){
        write(pip[i][WR],buffer,20);
    }
    sleep(1);
    for(int i = 1; i <numChildren;i++){
        kill(br[i],SIGKILL);
        printf("%s[CHD][%d] terminated!%s\n",RED,br[i],DF);
    }
    numChildren=1;
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

    signal(SIGINT,quit);  //CTRL+C
    signal(SIGTSTP,quit);   //CTRL+Z
    signal(SIGUSR1,handler);
    signal(SIGUSR2,handler);

    printf("%s[MAIN][%d]%s\n",GREEN,getpid(),DF);
    openFile = open("name.txt",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);

    char readIn[20];
    br[0]=getpid();
    do{
        read(STDIN_FILENO,readIn,20);
        if(isNumber(readIn)==0){
            //STRINGA
            strcpy(buffer,readIn);//saved in memory
            printf("%s[MAIN]msg receive!%s\n",GREEN,DF);
        }else{
            //NUMERO
            //creo figlio e lo metto in wait
            if(numChildren<=5){
                pipe(pip[numChildren]);
                br[numChildren] = fork();
                if(getpid()!=br[0]){
                    loop();
                }
                printf("%s[CHD][%d] created%s\n",GREEN,br[numChildren],DF);
                numChildren++;
            }else{
                printf("%sToo much children!%s\n",RED,DF);
            }
        }

    }while(1);


    return 0;
}