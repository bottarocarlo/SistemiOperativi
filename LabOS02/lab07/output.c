#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(int argv,char ** argc){
    sleep(2);
    fprintf(stdout,"ciao sono carlo e questo è una pipe");
    fflush(stdout);
    return 0;
}