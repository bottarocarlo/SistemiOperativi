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
#define MAX 1000
char buf[64];
char text[MAX];

int cont(char x){
    int appo=0;
    for(int i=0;i<strlen(text);i++){
        if(text[i]==x){
            appo++;
            printf("%s%c%s",RED,text[i],DF);fflush(NULL);
        }else
            printf("%c",text[i]);fflush(NULL);
    }
    return appo;
}
int main(int argc,char ** argv){
    if(argc<2){
        fprintf(stderr,"error usage..");
        return -1;
    }
    
    //se metto questo costrutto funziona tutto correttamente
    while(fgets(buf,sizeof(buf),stdin)){
        strcat( text , buf );
    }
    //printf("%s\n",text);
    printf("ci sono %d %c nel testo\n",cont(argv[1][0]),argv[1][0]);



    return 0;
}