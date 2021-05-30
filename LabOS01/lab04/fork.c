#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char ** argv){
    int status=0;    
    int return_value;

    printf("Forking process\n");
    return_value = fork();
    printf("The process id is %d and return value is %d \n", 
    getpid(), return_value);
    system("ls");
    if(return_value==0){
        //SONO IL FIGLIO
        printf("NOOOOOOO\n");
    }else if(return_value>0){
        //SONO TUO PADRE
        printf("SONO TUO PADREEH\n");
    }

    return status;
}