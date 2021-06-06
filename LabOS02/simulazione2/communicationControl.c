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
int depht, pid[MAX_DEPHT],master,proc,level;
int input[MAX_DEPHT][2] , output[MAX_DEPHT][2],outcome;
char buffer[20],buf[20];

void loop(){
    
}

void quit(){
    for(int i=0;i<depht;i++){
        kill(pid[i],SIGTERM);
    }
    while(wait(NULL)>0);
    exit(0);
}

int main(int argc,char ** argv){
    signal(SIGTSTP,quit);
    if(argc<2){
        fprintf(stderr,"error usage\n");
        exit(-1);
    }
    depht = atoi(argv[1]);
    if (depht>MAX_DEPHT){
        fprintf(stderr,"error depht\n");
        exit(-1);
    }
    printf("[MAIN] creating %d process\n",depht);
    master = getpid();
    for(int i =0;i<depht;i++){
        pipe(input[i]);
        pipe(output[i]);
        pid[i] = fork();
        if(getpid()!=master){
            //child
            close(input[i][WRITE]);
            close(output[i][READ]);
            proc = i;
            printf("%s[CHILD][%d] created%s\n",GREEN,getpid(),DF);
            break;
        }else{
            //father
            close(input[i][READ]);
            close(output[i][WRITE]);
            
        }
    }
    sleep(1);
    if(master==getpid()){
        while(1){
            printf("Next command:\t");fflush(NULL);
            read(STDIN_FILENO,buffer,20);
            if(buffer[0]=='q'){
                quit();
            }else if(buffer[0]=='i' || buffer[0]=='r'){
                level = atoi (buffer+1);
                write(input[level][WRITE],buffer,10);
                sleep(1);
                read(output[level][READ],&outcome,10);
                printf("[MAIN] child %d told me '%d'\n",pid[level],(int)outcome);
            }
        }
    }else{
        srand(time(NULL));
        int random;
        int pido;
        int r;
        while(1){
        r = read(input[proc][READ],&buf,10);
            if(r!=-1){
                switch (buf[0]){
                case 'i':
                    /* pid */
                    pido = getpid();
                    write(output[proc][WRITE],&pido,2);
                    printf("%sChild sending own pid...%s\n",GREEN,DF);
                    break;
                case 'r':
                    random = rand() % 100;
                    /* random */
                    write(output[proc][WRITE],&random,2);
                    printf("%sChild computing random...%s\n",GREEN,DF);
                    
                    break;
                default:
                    break;
                }
            }
        }
    }

    return 0;
}