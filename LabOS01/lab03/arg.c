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
#include <errno.h>

int main(int argc,char ** argv){
    if(argc<2){
        printf("scemo");
        return -1;
    }

    for (int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"-h")==0){
            printf("--help\n");
        }
        else if(strcmp(argv[i],"-l")==0){
            printf("--list\n");
        }
        else if(strcmp(argv[i],"-q")==0){
            printf("--quit\n");
        }
        else if(strcmp(argv[i],"-c")==0){
            printf("--color\n");
        }
    }
    

    return 0;
}