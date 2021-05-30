#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>


int main(){
    remove("/tmp/log.txt");
    int openFile = open("/tmp/log.txt",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
    int canRead;
    char tmp[40];

    //READ

    do{
        canRead= read(openFile,tmp,40);
        printf("%s\n",tmp);
    }while(canRead>0);

    //WRITE

    write(openFile,"Hello world!",strlen("Hello world!"));

    lseek(openFile,50,SEEK_SET);
    write(openFile,"Hello world!",strlen("Hello world!"));
 
    //PRINT 

    printf ("ciao %d\n", canRead);
    fprintf(stdout,"ciao %d",canRead);

    // %[flag][larghezza][.precisione][lunghezza]tipo

    printf("int: %lu\n",sizeof(int));
    printf("float: %lu\n",sizeof(float));
    printf("char: %lu\n",sizeof(char));
    printf("long: %lu\n",sizeof(long));



    
    exit(5);// posso usare in maniera indifferente return 0; o la funzione exit
}   