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
#define WR 1
#define RD 0

int depht, master, proc,outcome;
int pid[MAX_DEPHT];
char buffer[10];

struct msg_buffer
{
    char payload[3];
    int type;
}msg;

void quit(){
    for(int i =0 ;i <depht;i++){
        kill(pid[i],SIGTERM);
        printf("%s[%d] destroyed..%s\n",RED,pid[i],DF);
    }
    while(wait(NULL)>0);
    exit(0);
}


int main(int argc,char ** argv){
    
    int level,random,r;
    int pido;

    if(argc < 2){
        fprintf(stderr,"non ci siamo, riprova!");
        exit(-1);
    }
    depht = atoi(argv[1]);
    if(depht > MAX_DEPHT){
        printf("scemotto, è troppo");
        exit(-2);
    }
    printf("Creating %d process\n",depht);
    int input[depht][2],output[depht][2];

    //creo i figli
    master = getpid();
    for(int i=0;i<depht;i++){
        pipe(input[i]);
        pipe(output[i]);
        pid[i] = fork();
        if(getpid()!= master){
            close(input[i][RD]);
            close(output[i][WR]);
            setpgid(0,getpid());
            proc = i;
            printf("%s[%d] created..%s\n",GREEN,getpid(),DF);
            break;
        }else{
            close(input[i][WR]);
            close(output[i][RD]);
        }
    }

    if(getpid()==master){
        //master
        do{
            sleep(1);
            printf("Next command:\t");fflush(NULL);
            read(STDIN_FILENO,buffer,10);
            
            if(buffer[0]=='q'){
                quit();
            }else if(buffer[0]=='i' || buffer[0]=='r'){
                level = atoi(buffer+1);
                if(level<=depht){
                    write(output[level][WR],buffer,2); 
                    sleep(1);
                    read(input[level][RD],&outcome,2); 
                    printf("%sChild %d told me: '%d'%s\n",GREEN,pid[level],(int)outcome,DF);
                }else{
                    fprintf(stderr,"%sWrong target%s\n",RED,DF);
                }
            }else{
                printf("%swrong command%s\n",RED,DF);
            }
        }while(1);

    }else{
        //children
        do{
            srand(time(NULL));
            r = read(output[proc][RD],buffer,2);
            if(r!=-1){
                switch (buffer[0])
                {
                case 'i':
                    /* pid */
                    pido = getpid();
                    printf("[%d] CHD sending pid....\n",getpid());
                    write(input[proc][WR],&pido,2); 
                    break;
                case 'r':
                    /* random */
                    random = rand() % 100;
                    printf("[%d] CHD computing random....\n",getpid());
                    write(input[proc][WR],&random,2);
                    break;
                    break;
                
                default:
                    break;
                }
            }
        }while(1);

    }


    return 0;
}