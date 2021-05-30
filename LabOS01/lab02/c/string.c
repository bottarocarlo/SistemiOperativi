#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void iprint(int array[],int lenght){
    for (int i = 0; i < lenght; i++)
    {
        printf("%d : %d\n",i+1,array[i]);
    }
}

int main(int argc, char ** argv){
    int status=0;

    int array[] = { 2, 3 , 4 , 5};
    iprint(array,sizeof(array)/sizeof(array[0]));
    
    char str[] = {'c','i','a','o','0'};
    //str è dunque un puntatore a char (primo elemento) e si ha che:
    //str[n] corrisponde a *(str+n)
    //str[0] = *(str+0) = *(str) = *str

    //char c; carattere singolo
    //char *c; vettore di caratteri
    //char **c; vettore di vettori di caratteri
        //capiamo ora il perchè di **argv
    if(argc<2){
        fprintf(stderr,"COLLLIONE");
        exit(-1);
    }else {
        printf("numero di argomenti: %d\n",argc);
        for (int i = 1; i < argc; i++)
        {
            printf("%d : %s\n",i,argv[i]);
        }
        
    }

    return status;
}