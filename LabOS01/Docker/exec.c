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
    int out = open ("out.txt", O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
    int err = open ("err.txt", O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);

    dup2(out,1);
    dup2(err,2);
    fprintf(stdout,"out:\n");
    fprintf(stderr,"err:\n");
    for(int i=1;i<argc;i++){
        int child = !fork();
        if (child){
            printf("child!\n");
            char * arg[]= {argv[i],NULL};
            execv(argv[i],arg);
            wait(NULL);
            exit(0);
        }
    }    

    close(out);
    close(err);

    return 0;
}