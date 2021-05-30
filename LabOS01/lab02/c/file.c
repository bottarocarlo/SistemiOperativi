/*
due tipi di interazione con i file
attraverso i file descriptor oppure attraverso gli stream
*/

#include <stdio.h>

int main(int argc , char * argv[]){
    //STREAM
    FILE *ptr;
    ptr = fopen("filename.txt","r+");

    char tmp[50];
    while(!feof(ptr)){
        fgets(tmp,50,ptr);
        printf ("%s\n",tmp);
    }

    rewind(ptr);

    //counting 
    char tmpc;
    int alfabeto[26];
    while(!feof(ptr)){
        tmpc=fgetc(ptr);
        switch (tmpc)
        {
        case 'a':
            alfabeto[0]++;
            break;
        case 'b':
            alfabeto[1]++;
            break;
        case 'c':
            alfabeto[2]++;
            break;
        case 'd':
            alfabeto[3]++;
            break;
        case 'e':
            alfabeto[4]++;
            break;
        case 'f':
            alfabeto[5]++;
            break;
        case 'g':
            alfabeto[6]++;
            break;
        case 'h':
            alfabeto[7]++;
            break;
        case 'i':
            alfabeto[8]++;
            break;
        case 'l':
            alfabeto[9]++;
            break;
        case 'm':
            alfabeto[10]++;
            break;
        case 'n':
            alfabeto[11]++;
            break;
        case 'o':
            alfabeto[12]++;
            break;
        case 'p':
            alfabeto[13]++;
            break;
        case 'q':
            alfabeto[14]++;
            break;
        case 'r':
            alfabeto[15]++;
            break; 
        case 's':
            alfabeto[16]++;
            break;
        case 't':
            alfabeto[17]++;
            break;
        case 'u':
            alfabeto[18]++;
            break;
        case 'v':
            alfabeto[19]++;
            break;
        case 'z':
            alfabeto[20]++;
            break;
        }
    }
    //printf ("%d",alfabeto[0]);

    printf("nel testo sono presenti..\n");
    for (int i = 0; i < 21; i++)
    {
        printf("%d : %d\n", i+97, alfabeto[i]);
    }
    
    fclose(ptr);;



   return 0;
}