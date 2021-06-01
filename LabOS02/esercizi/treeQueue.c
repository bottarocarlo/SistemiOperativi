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

int pid[MAX_DEPHT],proc;
char buffer[10];
extern int errno;

//coda di messaggi
struct msg_buffer
{
    char payload;
    int type;
}msg,msgR;

void quit(){
    for(int i=1;i<MAX_DEPHT;i++){
        kill(-pid[i],SIGTERM);
        printf("%s[%d] killed %s\n",RED,pid[i],DF);
    }
    while(wait(NULL)>0);
    printf("%s[%d][MAIN] killed %s\n",RED,getpid(),DF);
    exit(0);
}

int main(int argc,char ** argv){
    int level=0;
    printf("%s[MAIN][%d] created%s\n",GREEN,getpid(),DF);
    pid[0]=getpid();

    creat("/tmp/tree",0777);
	key_t k = ftok("/tmp/tree",1);
	int queueId=msgget(k,0777|IPC_CREAT);

    if(queueId == -1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }

    for(int i =1;i < MAX_DEPHT;i++){
        pid[i]=fork();
        if(pid[0]!=getpid()){
            proc = i;
            setpgid(0,getpid());
            printf("%s[%d] created%s\n",GREEN,getpid(),DF);
            break;
        }
    }

    if(getpid()==pid[0]){
        while (1)
        {
            /* father */
            sleep(1);
            printf("Next command:\t");fflush(NULL);
            read(STDIN_FILENO,buffer,10);
            if(buffer[0]=='q'){
                msgctl(queueId, IPC_RMID, NULL);
                quit();
            }else if(buffer[0]=='k' || buffer[0]=='c'){
                level = atoi(buffer+1);
                msg.type = level;
                msg.payload = buffer[0];
                int esito = msgsnd(queueId , &msg, sizeof(msg.payload),0);
                
                if (esito>=0){
                    printf("%s[SND] %c , %d%s\n",GREEN,msg.payload,msg.type,DF);
                }else{
                    fprintf(stderr, "errno = %d\n", errno);
                    perror("Error printed by perror");
                    fprintf(stderr,"Strerror: %s\n", strerror(errno));
                }
            }
        }
        

    }else{
        while (1)
        {
            pause();
            printf("ciao\n");
            msgrcv(queueId,&msgR,sizeof(msgR.payload),proc,0);
            
           
            if(msgR.payload=='c'){
                printf("%s[RCV][%d]:%d%s\n",GREEN,getpid(),msgR.payload,DF);
            }
            sleep(1);
             fflush(NULL);
        }
        
    }





    return 0;
}