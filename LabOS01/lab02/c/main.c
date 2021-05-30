#include <stdio.h>
typedef int martina;

int main(int argc,char * argv[]){
    int status = 0;
    #ifdef DEBUG
    printf("%d\n",argc);
    printf("%s\n",argv[0]);
    printf("la variabile DEBUG è presente e vale: %d\n",DEBUG);
    #endif
    int c , d; 
    while ((c= getchar())!=EOF)
    {
        d = c;
        if(c>='a' && c <= 'z') d -= 32;
        if(c>='A' && c <= 'Z') d += 32;
        putchar(d);
    }
    return status;
