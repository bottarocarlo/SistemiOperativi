#include <stdio.h>
#include <stdlib.h>

int main(int argc,char ** argv){
    int status =0;
    status = system("cd ; ls");
    fflush (stdout);
    return status;
}