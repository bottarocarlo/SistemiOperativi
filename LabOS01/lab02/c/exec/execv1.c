#include <unistd.h>
#include <stdio.h>

void main(){
    char * argv[] = {"par1","par2",NULL};
    execv("./execv2.o",argv);
    printf("This is execv1!\n");

}