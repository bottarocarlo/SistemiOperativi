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

struct msg_buffer
{
    char mtext[10];
    long mtype;
}msg,msgp;

char directory[64];
char pid[6];
int br[10];
int master;
char name[64],str[32];
key_t queue_key;
int   queue_id;

int main(int argc,char ** argv){
    system("rm -rf info");
    int depth = atoi(argv[2]);
    if(depth >10){
        fprintf(stderr,"too much chlildren bro");
        return -5;
    }
    if(argc < 2){
        fprintf(stderr,"error usage");
        return -2;
    }
    if(chdir(argv[1])==-1){
        fprintf(stderr,"directory not existing");
        return -1;
    }
    if(mkdir("info",755)==-1){
        fprintf(stderr,"error making direcotory");
        return -3;
    }
    sprintf(directory,"%s/info/key.txt",argv[1]);

    int fd = open(directory,O_CREAT|O_RDWR);    
    if(fd<0){
        fprintf(stderr,"error making file");
        return -4;
    }
    sprintf(pid,"%d\n",getpid());
    write(fd,pid,strlen(pid));
    close(fd);

    master = getpid();

    for(int i=0;i<depth;i++){
        br[i] = fork();
        if(getpid()!=master){
            printf("%d ",getpid());fflush(NULL);
            sprintf(name,"%s/info/%d.txt",argv[1],getpid());
            int fd = open(name,O_CREAT|O_RDWR);    
            if(fd<0){
                fprintf(stderr,"error making file");
                return -4;
            }
            close(fd);
            break;
        }
    }

    if(getpid()==master){
        queue_key = ftok(directory, 32);
        queue_id  = msgget(queue_key, 0777 | IPC_CREAT); // create or get if already set
        if (queue_id<0){
            fprintf(stderr,"queue id");
            return -6;
        }
        int e=msgctl(queue_id, IPC_RMID, NULL);              // remove (so to clean-up)
        if (e<0){
            fprintf(stderr,"msgctl error");
            return -7;
        }
        queue_id = msgget(queue_key , 0777 | IPC_CREAT); // create brand new one
        if (queue_id<0){
            fprintf(stderr,"queue id");
            return -6;
        }
        sprintf(str, "%d", getpid()); msgp.mtype = 1; strcpy(msgp.mtext, str);
        int esito = msgsnd(queue_id, &msgp, sizeof(msgp.mtext), 0);
        if (esito<0){
            fprintf(stderr,"error sending message");
            return -8;
        }



        while(wait(NULL)>0);
    }

    
    return 0;
}