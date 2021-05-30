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
#include <pthread.h>

void *my_fun(void *param){
    printf("this is the function I asked for %d\n",*(int *)param);
    sleep(2);
    return (void *)3;
}


int main(){
    pthread_t t_id;
    void * retFromThread;
    int arg = 10;
    pthread_create(&t_id,NULL,my_fun,(void *)&arg);
    pthread_join(t_id,&retFromThread);
    printf("Executed thread with ID: %d\nRetVal: %d\n",(int)t_id,(int)retFromThread);

    return 0;
}