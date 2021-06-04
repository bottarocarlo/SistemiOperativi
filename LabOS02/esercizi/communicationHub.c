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
#define LENGTH 20

int slave, master, pid[MAX];
long id;
char buffer[LENGTH];
int sender,receiver;
extern int errno;
int queueId;


int fd; 
char * fifoName = "/tmp/fifo";

struct msg_buffer
{
    char messg[LENGTH];
    long type;
}msg;

void quit(){
    for(int i=0;i<slave;i++){
        kill(pid[i],SIGTERM);
        printf("%s[%d] killed!%s\n",RED,pid[i],DF);
    }
    while(wait(NULL)>0);
    exit(0);
}

void send(int sigNum){
    printf("[%ld]Send '%s'\n", id ,buffer);
    
    fd = open(fifoName, O_WRONLY); // Open FIFO for write only 
    write(fd, buffer, sizeof(buffer)); // write and close
    close(fd);
}

void receive(int sigNum){
    
    fd = open(fifoName, O_RDONLY); // Open FIFO for Read only 
    read(fd, buffer, sizeof(buffer)); // Read from FIFO
    printf("[%ld]Received '%s'\n", id ,buffer);
    close(fd);

}

void message(){
    char appo[LENGTH];
    for(int i = 0; i< strlen(buffer);i++){
        appo[i] = buffer[i+3];
    }
    strcpy(buffer,appo);
}

int main(int argc,char ** argv){
    int r;

    signal(SIGUSR1,send);
    signal(SIGUSR2,receive);

    if(argc < 2){
        fprintf(stderr,"usage error\n");
        exit(-1);
    }
    slave = atoi(argv[1]);
    if(slave>MAX){
        fprintf(stderr,"usage parameter\n");
        exit(-2);
    }
    master = getpid();

    mkfifo(fifoName,S_IRUSR|S_IWUSR);

    creat("/tmp/tree",0777);
	key_t k = ftok("/tmp/tree",1);
	queueId=msgget(k,0777|IPC_CREAT);
    if(queueId==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }

    for(int i =0;i<slave;i++){
        pid[i] = fork();
        if(getpid()!=master){
            id = i;
            printf("%s[%d][%ld] created!%s\n",GREEN,getpid(),id,DF);
            break;
        }
    }

    if(getpid()==master){
        while(1){
            sleep(1);
            printf("Next command:\t");fflush(NULL);
            read(STDIN_FILENO,buffer,10);
            if(buffer[0]=='q'){
                msgctl(queueId, IPC_RMID, NULL);
                quit();
            }else if(buffer[1]=='<' || buffer[1]=='>'){
            switch (buffer[1])
            {
            case '<':
                /* code */
                sender = atoi(&buffer[2]);
                receiver = atoi(&buffer[0]);
                break;
            case '>':
                /* code */
                sender = atoi(&buffer[0]);
                receiver = atoi(&buffer[2]);
                break;
            default:
                break;
            }
            message();
            
            strcpy(msg.messg,buffer);
            msg.type = (long) sender;
            int esito = msgsnd(queueId , &msg, sizeof(msg.messg),0);
            if (esito<0){
                fprintf(stderr, "SEND ERROR: %d\n", errno);
                perror("Error printed by perror");
                fprintf(stderr,"Strerror: %s\n", strerror(errno));
            }

            //SEND
            sleep(1);
            kill(pid[sender],SIGUSR1);
            sleep(1);
            kill(pid[receiver],SIGUSR2);
            }
        }
        while(wait(NULL)>0);
    }else{
        int outcome;
        while(1){
            outcome = msgrcv(queueId,&msg,sizeof(msg.messg),id,0);
            
            if(outcome!=-1){
                strcpy(buffer,msg.messg);
                printf("RECEIVED SMTH %d , %s\n",getpid(),msg.messg);
            }
        }
    }

    return 0;
}