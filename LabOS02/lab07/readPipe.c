#include <stdio.h>
#include <unistd.h>

int main(int argc,char ** argv){
    int fd[2];
    char buf[50];
    int esito = pipe(fd);
    if(esito==0){
        write(fd[1],"writing",8);
        int r = read (fd[0],&buf,50);
        printf("Last read %d.Received: '%s'\n",r,buf);
        r = read (fd[0],&buf,50);
        printf("Last read %d.Received: '%s'\n",r,buf);
    }


    return 0;
}