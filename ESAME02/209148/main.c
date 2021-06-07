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

#define PATH_MAXLEN 60 
#define CHILDREN_MAX 10 
#define PIDLEN 8 


struct msg_buffer{
    char mtext[10];
    long mtype;
}msg;

    extern int errno;
    int fd;
    char pido[PIDLEN];
    char dir[PATH_MAXLEN];
    int queueId;
    int pid[CHILDREN_MAX];
    char argvdir[PATH_MAXLEN];

void handler1(int sigNum){
    sprintf(dir , "%s/info/%d.txt" , argvdir,getpid());
    if ((fd = open(dir,  O_RDWR|O_CREAT,0755)) == -1)
    {
        fprintf(stderr,"file opening failed\n");
        exit(0);
    }
    lseek(fd, 0, SEEK_END);
    write(fd,"SIGUSR1\n",strlen("SIGUSR1\n"));
    close(fd);
}

void handler2(int sigNum){
    msg.mtype=1;
    sprintf(pido,"%d",getpid());
    strcpy(msg.mtext,pido);
    msgsnd(queueId , &msg, sizeof(msg.mtext),0);
}

int main(int argc,char ** argv){
    int master = getpid();

    if(argc<3){
        fprintf(stderr,"usage error\n");
        exit(1);
    }
    if(chdir( argv[1] ) == -1){
        fprintf(stderr,"directory not existing\n");
        exit(2);
    }
    if (mkdir("info", 0755) == -1){
        fprintf(stderr,"error making directory\n");
        exit(3);
    }
    //controllo quanti figli devo creare
    int level = atoi(argv[2]);
    if(level > CHILDREN_MAX || level <0){
        fprintf(stderr,"too much children\n");
        exit(3);
    }

    strcpy(argvdir,argv[1]); // questo mi serve dopo

    //apro il file (e lo creo key.txt)
    sprintf(dir , "%s/info/key.txt" , argv[1]);
    if ((fd = open(dir,  O_RDWR|O_CREAT,0755)) == -1){
        fprintf(stderr,"file opening failed\n");
        exit(4);
    }
    //sto scrivendo il pid sul file key.txt
    sprintf(pido , "%d\n" , getpid());
    write(fd,pido,strlen(pido));
    close(fd);

    //creo la queue
    sprintf(dir , "%s/info/key.txt" , argv[1]);
	key_t k = ftok(dir,32);
	queueId=msgget(k,0777|IPC_CREAT);
    msgctl(queueId, IPC_RMID, NULL);
    if(queueId==-1){
        fprintf(stderr, "errno = %d\n", errno);
        perror("Error printed by perror");
        fprintf(stderr,"Strerror: %s\n", strerror(errno));
    }

    //creo 'level' figli
    for(int i =0 ;i<level;i++){
        pid[i] = fork();
        if(master != getpid()){
            //creando per ogni figlio un file pid.txt
            sprintf(dir , "%s/info/%d.txt" , argv[1],getpid());
            if ((fd = open(dir,  O_RDWR|O_CREAT,0755)) == -1)
            {
                fprintf(stderr,"file opening failed\n");
                exit(0);
            }
            close(fd);
            break;
        }
    }

    msg.mtype=1;
    strcpy(msg.mtext,pido);
    msgsnd(queueId , &msg, sizeof(msg.mtext),0);

    if(master == getpid()){
        for(int i=0;i<level;i++){
            printf("%d ",pid[i]);fflush(NULL);
        } 
        printf("\n");fflush(NULL);
    }else{
        signal(SIGUSR1,handler1);
        signal(SIGUSR2,handler2);
        while(1);
    }
    
return 0;
}