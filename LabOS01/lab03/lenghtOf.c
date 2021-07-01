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

char buffer[64];



int main(int argc,char ** argv){
    if(argc<2){
        printf("error");
        return -1;
    }

    int lenght;
    for(int j=1;j<argc;j++){
        for(int i =0; argv[j][i]!='\0';i++){
            lenght++;
        }
    }
    printf("Lengh: %d\n",lenght);

    return 0;
}