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
#include <time.h>


#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"
#define MAX 10
#define WR 1
#define RD 0


int main(int argc, char ** argv){
    
    signal(SIGUSR1,SIG_IGN);
    if (argc<2){
        fprintf(stderr,"%swrong number of parameters!%s",RED,DF);
        exit(-1);
    }
    int depht = atoi(argv[1]);
    if(depht>=MAX){
        fprintf(stderr,"%stoo much process!%s",RED,DF);
        exit(-2);
    }

    char buffer[3];
    int foo;
    int quit =0;
    int proc=0;
    int level,r;
    int master[depht];

    int input[depht][2];
    int output[depht][2];
    master[0]= getpid();
    printf("%s[%d]created%s\n",GREEN,getpid(),DF);
    for(int i = 1 ; i<depht ; i++){
        pipe(input[i]);
        pipe(output[i]);
        master[i]=fork();
        if(master[0]!=getpid()){
            //children
            proc=i;
            printf("%s[%d]created%s\n",GREEN,getpid(),DF);
            close(output[i][WR]);
            close(input[i][RD]);
            break;
            
        }else{
            //father
            close(input[i][WR]);
            close(output[i][RD]);
        }
    }
    if(getpid()==master[0]){
        sleep(1);
        do{
            printf("Next command: \n");
            read(STDIN_FILENO,buffer,3);

            if(buffer[0]=='q'){
                /* quit */
                for(int i =1;i<depht;i++){
                    kill(master[i],SIGTERM);
                    printf("%s[%d]terminated%s\n",RED,master[i],DF);
                }
                while(wait(NULL)>0);
                printf("%s[%d]terminated%s\n",RED,master[0],DF);
                exit(0);
            }else if(buffer[0]=='r' || buffer[0]=='i'){
                level = atoi(buffer+1);
                write(output[level][WR],buffer,1);
                sleep(1);
                read(input[level][RD],&foo,sizeof(foo));
                printf("[%d]read : '%d'\n",master[level],(int)foo);
            }

        }while(quit==0);
    }else{
        while(1){ 
        //Read parent instructions
        r = read(output[proc][RD],buffer,254);
        printf("[%d]\n",getpid());
            if(r!=-1){
            srand(time(NULL));
            if(buffer[0]=='r'){
                int random = rand()%100;
                printf("%s[%d]Child computing random....%s\n",GREEN,getpid(),DF);
                write(input[proc][WR],&random,sizeof(random)); //Send random
            }else if(buffer[0]=='i'){
                printf("%sChild sending own pid....%s\n",GREEN,DF);
                int tmp = getpid();
                write(input[proc][WR],&tmp,sizeof(tmp)); //Send pid
            }
            }     
        }
        
    }
    

    return 0;
}