#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(){
    int status = 0;
    char buf[50];
    int fd = open("name.txt",O_RDWR);
    int cpy = dup(fd);//FUNZIONE DUP()
    read(cpy,buf,50);
    printf("Content: %s\n",buf);
    printf("\n\n%d , %d",fd,cpy);
    close(cpy);
    close(fd);

    //PERMESSI
    int test = open("name.txt",O_RDWR); 

    fchown(test,0,0);



    close(test);


    

    



    return status;
}