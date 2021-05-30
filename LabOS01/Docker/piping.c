 #include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main(int argc,char ** argv){
    if(argc!=3){
        exit(-1);
    }


    int out = open ("out.txt",O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    dup2(out,1);
    dup2(out,0);
    int child = fork();
    if(child==0){
        system(argv[1]);
        exit(0);
    }else{
        wait(NULL);
        lseek(out, 0, SEEK_SET);
        system(argv[2]);
    }
    
    

    close(out);

    return 0;
}