#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

extern int errno; // declare external global variable

/*
QUESTO è SOSTANZIALMENTE SEMPRE DA METTERE!!
    fprintf(stderr, "errno = %d\n", errno);
    perror("Error printed by perror");
    fprintf(stderr,"Strerror: %s\n", strerror(errno));
*/

int main(int argc,char ** argv){

    int sis = kill(3333,SIGUSR1);

    if(sis==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }else{
        printf("Signal sent\n");
    }  

    sleep(1); 

    FILE * pf;
    pf = fopen ("nonExistingFile.boh", "rb"); //Try to open file

    if (pf == NULL) {
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    } else {
       fclose (pf);
}


    return 0;
}