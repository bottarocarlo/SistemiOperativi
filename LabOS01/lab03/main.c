#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct Books
{
    char author[50];
    char title[50];
    int bookId;
} book1, book2;


int main(int argc, char **argv){
    #ifdef DEBUG
    printf("HAI DEFINITO DEBUG");
    printf("%d\n",argc);
    printf("%s\n",argv[1]);
    #endif
    /*
    int id;
    char str1[10],str2[10];
    char chAr[20];


    FILE *ptr;
    ptr = fopen("filename.txt","r+"); //r read w write

    do{
        fscanf(ptr, "%i %s %s", &id, str1,str2);
        printf("%i %s %s \n", id, str1,str2);
    }while(!feof(ptr));

    rewind(ptr);
    fgets(chAr,19,ptr);
    printf("\n\nstringa gets: %s",chAr);
    fclose(ptr);

    //file descriptor

    int fd = open("filename.txt",O_CREAT|O_RDWR);    
    char content[20];
    int canRead;

    do{
        canRead = read(fd,content,20);
        printf("%s",content);
    }while(canRead!=0);

    write(fd,"HELLO WORLD\n",strlen("HELLO WORLD\n"));
    lseek(fd,6,SEEK_SET);
    */
    int x=10;
    printf("variabile x=10   : %lu\n",sizeof x); 
    printf("variabile int    : %lu\n",sizeof (int));
    printf("variabile doble  : %lu\n",sizeof (double));

    int code =0;

    if(argc<2){
        printf("Usage: %s <carattere>", argv[0]);
        code =2;
    }else{
        printf("ASCII (%d) == CHAR (%c)\n",argv[1][0],argv[1][0]);
    }

    return code;
}