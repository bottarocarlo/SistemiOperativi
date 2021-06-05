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
#define RED "\033[0;31m"
#define GREEN "\033[32m"
#define DF "\033[0m"

void * my_fun(void * param){
    printf("Thread %ld started\n",*(pthread_t *)param); sleep(3); 
    char * str = "Returned string";
    pthread_exit((void *)str); //or ‘return (void *) str;’
}

int main(){
    pthread_t t_id;
    void * retFromThread; //This must be a pointer to void! 
    pthread_create(&t_id, NULL, my_fun, (void *)&t_id); //Create 
    pthread_join(t_id,&retFromThread); // wait thread
    // We must cast the returned value!
    printf("Thread %ld returned '%s'\n",t_id,(char *)retFromThread);

    return 0;
}