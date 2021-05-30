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

int master[5];
int children[10];
int level;
int numNipoti=0;
int depht=0;

void tab(int foo){
    for(int i=0;i<foo;i++){
        printf("\t");
        fflush(NULL);
    }
}

void quit(){
    for(int i =1;i <5;i++){
        printf("%s[%d] killed%s\n",RED,master[i],DF);
        kill(-master[i],SIGTERM);
    }
}

void create(int signo){
    children[numNipoti]=fork();
    if(children[numNipoti]==0){
        pause();
    }else{
        printf("%s[%d][%d] children%s\n",GREEN,getpid(),children[numNipoti],DF);
        numNipoti++;
    }
}
void killChildren(int signo){
    for(int i =0;i<numNipoti;i++){
        kill(children[i],SIGTERM);
        printf("%s[%d] children killed%s\n",RED,getpid(),DF);
    }
    numNipoti=0;
}
void print (int signo){
    for(int i=0;i<numNipoti;i++){
        tab(depht+i);
        printf("%s[ID - %d][parent - %d] : depth %d%s\n",GREEN,children[i],master[depht-1],depht,DF);
    }
    sleep(1);

}

int main(int argc,char ** argv){
    signal(SIGUSR1,create);
    signal(SIGUSR2,print);
    signal(SIGKILL,killChildren);


    char buffer[10];
    int finish=0;
    
    //---creating master process
    master[0]= getpid();
    for(int i =1;i <5;i++){
        master[i]=fork();
        setpgid(0,getpid());
        
        if(master[0]!=getpid()){
            depht=i;
            printf("%s[%d] created%s\n",GREEN,getpid(),DF);
            break;
        }
    }
    
    if(getpid()==master[0]){    
    sleep(1);
        do{
            printf("Next Command:\n");
            read(STDIN_FILENO,buffer,10);
            switch (buffer[0])
            {
            case 'c':
                /* create figli al livello n */
                level= atoi(buffer+1);
                if(level>=0 && level<5){
                    printf("creating children al level %d\n",level);
                    kill(master[level],SIGUSR1);
                }else{
                    printf("comando 'c n' errato..\n");
                }
                break;
            case 'k':
                /* kill children at level */
                level= atoi(buffer+1);
                if(level>=0 && level<5){
                    kill(master[level],SIGKILL);
                }else{
                    printf("comando 'k n' errato..\n");
                }
                break;
            case 'p':
                /* print */
                printf("%s[ID - %d][parent - %d] : depth %d%s\n",GREEN,master[0],0,0,DF);
                kill(master[0],SIGUSR2);
                for(int i = 1; i < 5 ; i++){
                    tab(i);
                    printf("%s[ID - %d][parent - %d] : depth %d%s\n",GREEN,master[i],master[i-1],i,DF);
                    kill(master[i],SIGUSR2);
                }
                break;
            case 'q':
                /* code */
                finish=1;
                break;
            default:
                printf("comando non esistente..\n");
                break;
            }
            sleep(1);
        }while(finish==0);
        quit();
        exit(0);
        while(wait(NULL)>0);//aspetto i figli
    }else{
        while (1);
    }

    return 0;
}