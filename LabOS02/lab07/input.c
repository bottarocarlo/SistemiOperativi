#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(){
    char buffer[50];
    int c = read (0,buffer,50);
    //read restutuisce il numero di bit letti
    if (c>0){
        fprintf(stdout,"Read '%s' \n",buffer);
    }

    return 0;
}