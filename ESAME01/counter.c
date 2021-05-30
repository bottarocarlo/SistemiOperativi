#include <stdio.h> //(f)printf
#include <string.h> //string
#include <stdlib.h> //exit
#include <unistd.h>//fork
#include <fcntl.h> //open
#include <sys/wait.h> //wait

int main(int argc,char ** argv){
    for (int i = 1; i < argc; i++)
    {
        if(argv[i]!="-l" || argv[i]!="-c"){
            char texto = argv[i]+".txt";
            int fd = open(texto,O_RDWR | O_CREAT, S_IRUSR | S_IWUS);
        }
    }
    

    /*
    for(int i=1;i<argc;i++){
        int cont=0;
        if(argv[i]=="-c"){
            char cr;
            while((cr=getchar())!=EOF){
            if(cr==argv[1][0]){
            printf("\033[0;31m%c\033[0;0m",cr);
            cont++;
        }
        }
    }
    */
   close(fd);


    return 0;
}