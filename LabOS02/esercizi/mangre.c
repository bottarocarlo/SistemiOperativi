#include <stdio.h>
#include<unistd.h>
#include<sys/msg.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<signal.h>

#define RED "\e[0;31m"
#define DF "\e[0m"
#define MAX_LENGHT 100 
#define MAX_DEPTH 5

int isChild=0;
int childXlvl[MAX_DEPTH];
int lvl=0; 

typedef struct{
    long mtype;
    char mtext[MAX_LENGHT];
}msg;

void cerr(char *s){
    fprintf(stderr,"%s%s%s",RED,s,DF);
    fflush(stderr);
}

void stop(){
    kill(-getpid(),SIGKILL);
}

int main(int argc,char **argv){
    signal(SIGINT,stop);
    // creeo coda
    remove("/tmp/tree");
    creat("/tmp/tree", 0777);
    key_t key = ftok("/tmp/tree", 1);
    int queueID= msgget(key,0777 | IPC_CREAT);

    int r=0,val;
    char buffer[100];
    msg msgR;
    while(1){
        if(isChild==0){
            msg msgS;
            memset(msgS.mtext,'\0',100);
            sleep(1);
            printf("\nInserisci un comando: ");
            fflush(stdout);
            r= read(STDIN_FILENO,&buffer,10);
            buffer[r]='\0'; // per bloccare strlen !
            if(buffer[0]=='q'){ // siamo nel quit  
                strcat(msgS.mtext,"kill");
                for(int i=1;i<=MAX_DEPTH;i++){
                    msgS.mtype=i;
                    for(int l=0;l<childXlvl[i-1];l++){
                        msgsnd(queueID,&msgS,sizeof(msgS.mtext),0);
                    }
                    childXlvl[i-1]= 0;
                }
                exit(0);
            } else if(buffer[0]=='p' ){ // siamo nel print 
                strcat(msgS.mtext,"print");
                for(int i=1;i<=MAX_DEPTH;i++){
                    msgS.mtype=i;
                    for(int l=0;l<childXlvl[i-1];l++){
                        msgsnd(queueID,&msgS,sizeof(msgS.mtext),0);
                    }
                }
            }else if(atoi(buffer+1)!=0){ // abbiamo inserito un numero 
                val = atoi(buffer+1);
                if(val<=5){
                    switch (buffer[0]){
                        case 'c':
                            if(val==1){ // devo creare figlio 1 da padre 
                                printf("\n[MAIN] Creating child...\n");
                                childXlvl[0]++;
                                //printf("\nNumber of childs of child at lvl %d: %d\n",val,childXlvl[val-1]);
                                int id=fork();
                                if(id==0){
                                    lvl++;
                                    isChild=1;
                                    printf("New child %d at lvl %d\n",getpid(),lvl);
                                }
                            }else{
                                printf("\n[MAIN] Creating grandchild...\n");
                                msgS.mtype=val-1;
                                strcat(msgS.mtext,"create");
                                for(int i=0; i<childXlvl[val-2];i++) msgsnd(queueID,&msgS,sizeof(msgS.mtext),0);
                                childXlvl[val-1]= childXlvl[val-1] + childXlvl[val-2];
                                //printf("\nNumber of childs of child at lvl %d: %d",val,childXlvl[val-1]); fflush(stdout);
                                //printf("\nNumber of childs of parent at lvl %d: %d\n",val-1,childXlvl[val-2]); 
                            }
                            break;
                        case 'k':
                            strcat(msgS.mtext,"kill"); // guarda il primo \0
                            for(int k=MAX_DEPTH;k>=val;k--){ // uccide i precedenti 
                                msgS.mtype=k;
                                for(int l=0;l<childXlvl[k-1];l++){
                                    msgsnd(queueID,&msgS,sizeof(msgS.mtext),0);
                                }
                                childXlvl[k-1]= 0;
                            }
                            break;

                            default:
                            cerr(" Comando errato, riprova");
                            break;
                    }
                }else cerr(" Numero troppo alto");
            }else cerr(" Comando errato riprova");

        }else{ // childs         
            int rcv = msgrcv(queueID,&msgR,sizeof(msgR.mtext),lvl,0); // mette in un simil stop e aspetta messaggio
            //if(rcv!=-1) printf("received\n"); // debugging 
            if(!strcmp(msgR.mtext,"create")){
                int id=fork();
                if(id==0){
                    lvl++;
                    printf("New child %d at lvl %d\n",getpid(),lvl);
                }
            }else if (!strcmp(msgR.mtext,"kill")){
                printf("Child %d at lvl %d has been killed!\n",getpid(),lvl);
                break;
            }else if (!strcmp(msgR.mtext,"print")){
                for(int i=0;i<lvl;i++){
                    printf("\t");
                    fflush(stdout);
                }
                printf("I'm child %d at lvl %d\n",getpid(),lvl);
            }
        }
    }
    return 0;
}