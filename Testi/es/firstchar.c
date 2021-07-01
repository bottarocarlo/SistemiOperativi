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

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYANO "\033[0;36m"
#define WHITE "\033[0;37m"
#define DF "\033[0m"

int pid[10];
int proc;
char tmp[10];

int main(int argc,char ** argv){
if(argc<2 || argc >10){
    fprintf(stderr,"error usage..");
    return -1;
}    
pid[0] = getpid();

for(int i =1 ;i <argc ; i++){
    pid[i] = fork();
    if(getpid()!=pid[0]){
        printf("%sCreated %d%s\n",GREEN,getpid(),DF);
        pid[i] = getpid();
        proc = i;
        break;
    }
   
}
if(getpid()==pid[0]){
    //father
    
    while(wait(NULL)>0);

}else{
    //child
    int openFile = open(argv[proc],O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
    int canRead = read(openFile,tmp,1);
    if(canRead>0){
        if((tmp[0]>='0' && tmp[0]<='9') || (tmp[0]>='a' && tmp[0]<='z')){
            printf("[%d][%d]: %c\n",getpid(),proc,tmp[0]);
        }else{
            printf("[%d][%d]: -\n",getpid(),proc);
        }
    }else{
        printf("[%d][%d]: ?\n",getpid(),proc);
    }
    
    close(openFile);
}


    return 0;
}