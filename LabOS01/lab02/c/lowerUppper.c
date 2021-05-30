#include <string.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc,char ** argv){
    int status = 0;
    for (int i=1;i< argc;i++){
       for (int j = 0; j < strlen(argv[i]); j++)
       {
           if(argv[i][j]>='a' && argv[i][j]<='z'){
               argv[i][j] -= 32; 
           }else if(argv[i][j]>='A' && argv[i][j]<='Z'){
               argv[i][j] += 32; 
           }
       }
    }
    printf ("%s",argv[1]);
    return status;
}
