#include<stdio.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
#include<signal.h>
#define MAX_CHILDS 10
#define RED "\e[0;31m"
#define GREEN "\e[0;32m"
#define DF "\e[0m"
#define RCV_MESS "rcv"

void cerr(char *s){
    fprintf(stderr,"%s%s%s\n",RED,s,DF);
}

void quit(){
    kill(-getpid(),SIGKILL);
}

typedef struct msg{
    long mtype;
    char mtext[50];
}msg;


int main(int argc,char **argv){
    if(argc!=2 || !atoi(argv[1])){
        cerr("Input errato!");
        exit(1);
    }
    signal(SIGINT,quit);
    int n= atoi(argv[1]);
    if(n>MAX_CHILDS){
        cerr("Troppi figli!");
        exit(2);
    }
    srand(time(NULL));
    remove("/home/kativen/gatto");
    creat("/home/kativen/gatto",0777);
    key_t key = ftok("/home/kativen/gatto",1);
    char appo[50];
    int queueID= msgget(key,0777 | IPC_CREAT);
    for(int i=1;i<=n;i++){
        if(!fork()){
            int x;
            msg msgr1;
            msg msgs2;
            while(1){
                memset(msgr1.mtext,'\0',50);
                msgrcv(queueID,&msgr1,sizeof(msgr1.mtext),i,0);
                if(!strncmp(msgr1.mtext,RCV_MESS,4)){
                    x=rand()%100;
                    sprintf(appo,"%d",x);
                    msgs2.mtype= 12;
                    strcpy(msgs2.mtext,appo);
                    msgsnd(queueID,&msgs2,sizeof(msgs2.mtext),0);
                }else{ // sto inviando il messaggio con il mio numero di nuovo al padre! 
                    printf("I am %d and this is the number: %s\n",i,msgr1.mtext);
                }
            }
        }
    }
    msg msgs1;
    msg msgr2;
    int r,num1,num2;
    char gatt[2];
    while(1){
        printf("Inserisci due numeri: "); fflush(stdout);
        r = read(STDIN_FILENO,&appo,sizeof(appo));
        appo[r] = '\0';
        if(strlen(appo)>3){
            cerr("Stringa troppo lunga!");
        }else{
            gatt[0] = appo[0];
            gatt[1] = '\0';
            num1= atoi(gatt);
            num2=  atoi(appo+1);
            if(num1!=num2 && num1<=n && num2<=n){
                msgs1.mtype= num1;
                strcpy(msgs1.mtext,RCV_MESS);
                msgsnd(queueID,&msgs1,sizeof(msgs1.mtext),0);
                msgrcv(queueID,&msgr2,sizeof(msgr2.mtext),12,0);
                msgs1.mtype=num2;
                strcpy(msgs1.mtext,msgr2.mtext);
                msgsnd(queueID,&msgs1,sizeof(msgs1.mtext),0);
            }else cerr("Errore!");
        }
        sleep(2);
    }
    return 0;
}