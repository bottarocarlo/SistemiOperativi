#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <err.h>
#include <string.h>
#include <signal.h>

//!!!
extern int errno;

int main(int argc,char ** argv){
    //con i file
    FILE * pf;
    pf = fopen("nonesisto.bho","rb");//openeninfg file in reading
    if(pf==NULL){
        fprintf(stderr,"errno = %d\n",errno);
        perror("Error print by perror");
        fprintf(stderr,"Strerror: %s\n",strerror(errno));
    }else{
        fclose(pf);
    }

    printf("\n\n");

    //con i processi
    int sys = kill (3443,SIGUSR1);
    if(sys==-1){
        fprintf(stderr,"errno = %d\n",errno);
        perror("Error print by perror");
        fprintf(stderr,"Strerror: %s\n",strerror(errno));
    }else{
        printf("signal sent!\n");
    }


    return 0;
}