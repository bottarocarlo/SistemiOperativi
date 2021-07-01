/*
Sviluppare un’applicazione in C che generi un binario denominato 
counter che simula in parte il comando wc di bash
*/
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
#include <errno.h>

//color
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYANO "\033[0;36m"
#define WHITE "\033[0;37m"
#define DF "\033[0m"



int main(int argc,char ** argv){
    char flag[4];
    int cont=0;
    int filen = 0;
    char file[64];
    int c=0,l=0;



    if(argc<2){
        printf("error usage");
        return -1;
    }
    
    for(int i =1;i<argc;i++){
        if(strcmp("-l",argv[i])==0){
            flag[cont] = 'l';
        }else if(strcmp("-c",argv[i])==0){
            flag[cont] = 'c';
        }else{
            filen++;
            strcpy(file,argv[i]);
        }
    }
    int child=fork();
    if(child){ //if (child != 0)
        //sono il padre
        while(wait(NULL)>0);
        return 0;
    }


    if(filen>0){
        char content[1]; int canRead;
        //diretta
        int fd = open(file,O_RDONLY);
        do{
            canRead = read(fd,content,1);
            if(content[0]=='\n'){
                l++;
            }
                c++;
            
        } while(canRead!=0);
        

        close(fd);
    }else{
        //piping 
        char cr;
        while((cr = getchar())!=EOF){
            if(cr == '\n'){
                l++;
            }else{
                c++;
            }
        }
    }

    for(int i =1;i<argc;i++){
        if(strcmp("-l",argv[i])==0){
            printf("[-l]:%d\n",l);
        }else if(strcmp("-c",argv[i])==0){
            printf("[-c]:%d\n",c);
        }
    }

    return 0;
}