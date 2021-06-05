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

#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"
#define MAX 10

extern int errno;

struct msg_buffer{
    long type;
    char payload[20];
}msg;

int children;
char buffer[20];
int fd; 
int id;
char * fifoName = "/tmp/unique";
char str[20];

void modify(){
    char appo[20];
    strcpy(appo,buffer);
    int len = strlen(buffer);
    for(int i =0;i<len;i++){
        buffer[i]=appo[i+3];
    }
}

void quit(int * pid){
    for (int i = 1; i < children; i++)
    {
        printf("%s[CHD][%d]killed%s\n",RED,pid[i],DF);
        kill(pid[i],SIGTERM);
    }
    printf("%s[MAIN][%d]killed%s\n",RED,pid[0],DF);
    exit(0);
}

void sender(int sigNum){

    fd = open(fifoName, O_WRONLY); // Open FIFO for write only 
    write(fd, msg.payload, strlen(msg.payload)+1); // write and close
    printf("%s[%d] is writing%s\n", GREEN,getpid(),DF);
    close(fd);
}

void receiver(int sigNum){

    fd = open(fifoName, O_RDONLY); // Open FIFO for Read only 
    read(fd, str, sizeof(str)); // Read from FIFO
    printf("%s[%d] is reading: %s%s\n", GREEN,getpid(),str,DF);
    close(fd);
}


int main(int argc,char ** argv){
    if(argc<2){
        fprintf(stderr,"%serr usage: ./communicationHub.o #n %s\n",RED,DF);
        exit(-1);
    }
    children = atoi(argv[1]);
    printf("creating %d slave(s)\n\n",children);

    signal(SIGUSR1,sender);
    signal(SIGUSR2,receiver);

    creat("/tmp/tree",0777);
	key_t k = ftok("/tmp/tree",1);
	int queueId=msgget(k,0777|IPC_CREAT);

    if(queueId==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }

    int sender,receiver;
    
    
    int pid[children];
    pid[0] = getpid();
    printf("[0][%d]\n",getpid());
    for(int i = 1; i<children ; i++){
        pid[i] = fork();
        if(getpid()!=pid[0]){
            //sono il figlio lasciami stare
            printf("[%d][%d]\n",i,getpid());
            id = i;
            break;
        }
    }
    
    if(getpid()==pid[0]){
        sleep(1);
        do{
            printf("Next command: \t");
            fflush(NULL);
            read(STDIN_FILENO,buffer,10);
            if(buffer[0] == 'q'){
                msgctl(queueId, IPC_RMID, NULL);
                quit(pid);
                exit(0);
            }
            if(buffer[1]=='>'){
                receiver = atoi( &buffer[2]);
                sender = atoi( &buffer[0]);
            }else if(buffer[1]=='<'){
                receiver = atoi( &buffer[0]);
                sender = atoi( &buffer[2]);
            }else if(receiver == -1 || receiver == -1){
                printf("errore atoi\n");
                quit(pid);
                exit(-1);
            }else{
                printf("altro errore generale\n");
                quit(pid);
                exit(-1);
            }
        if(receiver<children && sender<children){
            modify();
            strcpy(msg.payload,buffer);
            msg.type = sender;
            int esito = msgsnd(queueId , &msg, sizeof(msg.payload),0);
            if (esito<0){
                fprintf(stderr, "errno = %d\n", errno);
                perror("Error printed by perror");
                fprintf(stderr,"Strerror: %s\n", strerror(errno));   
            }
            //printf("sender: %d\nreceiver: %d\n",sender,receiver); 
            kill(pid[sender],SIGUSR1);
            sleep(1);
            kill(pid[receiver],SIGUSR2);
            sleep(1);
        }else{
            printf("usage #<# or #>#..\n");
        }
        }while(1);
        while (wait(NULL));
        

    }else{
        int outcome;
        while(1){
            outcome = msgrcv(queueId,&msg,sizeof(msg.payload),id,0);
            if(outcome!=1){
                strcpy(str,msg.payload);
            }
            outcome=0;
        }
    }


    return 0;
}