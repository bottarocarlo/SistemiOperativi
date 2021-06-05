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

#define READ 0
#define WRITE 1
//color
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYANO "\033[0;36m"
#define WHITE "\033[0;37m"
#define DF "\033[0m"

#define MAX 10

struct msgbuf {
    long mtype;       /* message type, must be > 0 */
    char mtext[50];    /* message data */
}msgS,msgR;

extern int errno;

int main(int argc,char ** argv){
    int pid[10];
    int master;
    int id;
    int ran;

    srand(time(NULL));
    creat("/tmp/tree",0777);
	key_t k = ftok("/tmp/tree",1);
	int queueId=msgget(k,0777|IPC_CREAT);
    if(queueId==-1){
        fprintf(stderr, "errno queue = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }
    master = getpid();
    for(int i =0;i<MAX;i++){
        pid[i] = fork();
        if(master!=getpid()){
            id = i;
            break;
        }
    }
    
    if(master==getpid()){
        //father
        for(int i =0;i<MAX;i++){
            ran = rand()%100;
            sprintf(msgS.mtext,"%d",ran);
            msgS.mtype = i+1;
            int esito = msgsnd(queueId , &msgS, sizeof(msgS.mtext),0);
            if (esito>=0){
                printf("%s[MAIN][%d]:%s%s\n",GREEN,i,msgS.mtext,DF);fflush(NULL);
            }else{
                fprintf(stderr, "errno = %d\n", errno);
                perror("Error printed by perror");
                fprintf(stderr,"Strerror: %s\n", strerror(errno));
            }
            sleep(1);
        }
        msgctl(queueId, IPC_RMID, NULL);
        while(wait(NULL)>0);
    }else{
        //child
        msgrcv(queueId,&msgR,sizeof(msgR.mtext),id+1,0);
        printf("%s[RCV][%d]:%s%s\n",GREEN,id,msgR.mtext,DF);fflush(NULL);
        exit(0);
    }
    


    return 0;
}