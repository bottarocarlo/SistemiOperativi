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

#define MAX_DEPHT 5

int master , pid[MAX_DEPHT] ,slaves;
int queueId;
long proc;
extern int errno;

typedef struct
{
    long type;
    char payload;
    
}msg;

void quit(int sigNum){
    for(int i =0;i<slaves;i++){
        kill(pid[i],SIGTERM);
    }
    while (wait(NULL)>0);
}

int main(int argc,char ** argv){
    srand(time(NULL));
    signal(SIGTSTP,quit);
    if(argc < 2){
        fprintf(stderr,"no argument\n");
        exit(-1);
    }

    creat("/tmp/tree",0777);
	key_t k = ftok("/tmp/tree",1);
	queueId=msgget(k,0777|IPC_CREAT);
    if(queueId==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }
    printf("%s[SND][queue key]: %d%s\n",GREEN,k,DF);
    printf("%s[SND][queue ID]: %d%s\n",GREEN,queueId,DF);

    slaves = atoi(argv[1]);
    master = getpid();
    for(int i =0;i< slaves; i++){
        pid[i] = fork();
        if(getpid()!=master){
            proc = i;
            printf("%s[%ld][%d] created!%s\n",GREEN,proc,getpid(),DF);
            break;
        }
    }

    if(getpid()==master){
        msg msgS;
        sleep(1);
        for(int i =0;i<slaves;i++){
            msgS.payload=rand()%100;
            msgS.type = i;
            int esito = msgsnd(queueId , &msgS, sizeof(msgS.payload),0);
            if (esito>=0){
                printf("%s[MAIN][msg.txt]:%d%s\n",GREEN,msgS.payload,DF);fflush(NULL);
            }else{
                fprintf(stderr, "errno = %d\n", errno);
                perror("Error printed by perror");
                fprintf(stderr,"Strerror: %s\n", strerror(errno));
            }
            sleep(1);
            kill(pid[i],SIGUSR1);
        }
        while(wait(NULL)>0);
        msgctl(queueId, IPC_RMID, NULL);
        
    }else{
        sleep(1);
        msg msgR;
        msgrcv(queueId,&msgR,sizeof(msgR.payload),0,proc);
        printf("[%ld][%d] ma che problemi ho?\n",proc,getpid());fflush(NULL);
        printf("%s[RCV][%d]:%d%s\n",GREEN,getpid(),msgR.payload,DF);fflush(NULL);
        exit(0);
    }
    return 0;
}