#include <sys/ipc.h> //ftok.c 
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h> 
#include <sys/msg.h>
#include <sys/stat.h>
#include <fcntl.h>


int main(){
    creat("/tmp/unique",0777);
    
    key_t queue1Key = ftok("/tmp/unique", 1);
    key_t queue2Key = ftok("/tmp/unique", 2);
    printf("[1] %d\n[2] %d\n",(int) queue1Key,(int)queue2Key);

    remove("/tmp/unique");
    return 0;
}  