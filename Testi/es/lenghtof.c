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


int main(int argc,char ** argv){
    if(argc < 2){
        fprintf(stderr,"error usage");
        return -1;
    }
    int appo=0;
    while(argv[1][appo]!='\0'){
        appo++;
    }
    
    printf("la frase %s'%s'%s è lunga %d\n",GREEN,argv[1],DF,appo);
    return 0;
}