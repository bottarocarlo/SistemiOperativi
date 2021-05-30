#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv){
    int status =0;
    char val[10][64];
    int cont = 0;
    for (int i = 1; i < argc; i++)
    {
        if(strcmp("-h",argv[i])==0){
            strcpy(val[cont],"help");
            cont++;
        }else if(strcmp("-k",argv[i])==0){
            strcpy(val[cont],"key");
            cont++;
        }else if(strcmp("-r",argv[i])==0){
            strcpy(val[cont],"reverse");
            cont++;
        }else if(strcmp("-l",argv[i])==0){
            strcpy(val[cont],"list");
            cont++;
        }else{
            printf("%s COMANDO NON PERVENUTO...\n", argv[i]);
        }

    }
    

    //print
    for (int i = 0; i < cont; i++)
    {
        printf("%d : %s\n", i, val[i]);
    }
    

    return status;
}