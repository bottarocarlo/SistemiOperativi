#include <stdio.h> //(f)printf
#include <string.h> 
#include <stdlib.h> //exit
#include <unistd.h>//fork
#include <fcntl.h> //open
#include <sys/wait.h> //wait

void printout(char c){
    printf("%c",c); fflush(stdout);
}

int main(int argc,char ** argv){

if(argc<2){
    fprintf(stderr,"!usage: ./sol1.o <file>\n");
    exit(-1);
}
int fd=0;
char chr[1];
int canread;

    for (int i = 1;i<argc;i++){
        int isChild = fork();
        if(isChild<0){
            fprintf(stderr,"fork fallita");
            exit(isChild);
        }else if(isChild == 0){
            //printf("is child!\n");
            fd = open (argv[i],O_RDONLY);
            lseek(fd,0,SEEK_SET);
            canread = read (fd,chr,1);
            //printf("%d",canread);
            printout(chr[0]);  
            close(fd);          
            exit(0);
        }

    while(wait(NULL)>0);
    }
    return 0;
}