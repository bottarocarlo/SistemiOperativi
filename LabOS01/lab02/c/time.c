#include <time.h>
#include <stdio.h>

int main(){
    int status =0;

    time_t whatTime = time (NULL);
    printf("Current time: %s\n",ctime(&whatTime));
    printf("Secondi da 1/1/1970: %lu",whatTime);
    return status;
}