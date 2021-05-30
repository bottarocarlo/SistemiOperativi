#include <stdio.h> //(f)printf
#include <string.h> //string
#include <stdlib.h> //exit
#include <unistd.h>//fork
#include <fcntl.h> //open
#include <sys/wait.h> //wait

int main(int argc,char ** argv){
    if (argc<2){
        fprintf(stderr,"usage\n");
        exit(-1);
    }
    


    return 0;
}
