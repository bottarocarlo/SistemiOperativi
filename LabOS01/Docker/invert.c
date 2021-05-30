#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * stringrev(char * str){
    int len = strlen(str);
    int cont =0;
    char appo[len];
    for(int i= len-1;i>=0;i--,cont++){
        appo[cont]=str[i];
    }
    str = appo;
    return str;
}

int stringpos(char * str,char c){
    for(int i= 0;i<strlen(str);i++){
        if(str[i]==c){
            return i+1;
        }

    }
    return -1;
}

int main(int argc,char ** argv){
    char fgh;
    if(argc<2){
        printf("usage");
        exit(-1);
    }
    for(int i=1;i<argc;i++){
        printf("%s : %s\n",argv[i],stringrev(argv[i]));
    }
    
    printf("il carattere c si trova al posto %d", stringpos(argv[1],'c'));
    

}