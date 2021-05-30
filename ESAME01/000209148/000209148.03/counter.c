#include <stdio.h> //(f)printf
#include <string.h> //string
#include <stdlib.h> //exit
#include <unistd.h>//fork
#include <fcntl.h> //open
#include <sys/wait.h> //wait

int main(int argc,char ** argv){
    int status=0;
    //0 piping
    //1 diretto
    int file;
    int l=0;
    int contl=0;
    int contc=0;
    int c=0;
    for (int i = 1; i < argc; i++)
    {
        if(strcmp(argv[i],"-l")!=0 && strcmp(argv[i],"-c")!=0){
            if(argv[i][strlen(argv[i])-1]=='t' && argv[i][strlen(argv[i])-2]=='x' && argv[i][strlen(argv[i])-3]=='t'&& argv[i][strlen(argv[i])-4]=='.'){
            status = 1;
            file = i;
            //printf("file\n");
            }else {
            fprintf(stderr,"?opzione errata\n");
            exit(1);
            
            }
        }
        if(strcmp(argv[i],"-l")==0){
            l=1;
            //printf("-l\n");
        }
        if(strcmp(argv[i],"-c")==0){
            c=1;
            //printf("-c\n");
        }
    }
    
    int child = !fork();
    

    if(child == 0){
        if(status == 1){
            int fd = open(argv[file],O_RDONLY);
            if (fd<0) {
                fprintf(stderr,"file error"); 
                exit(4);
                } 
            char content[1]; int canRead;
            do{
                canRead = read(fd,content,1);
                //printf("%c",content[0]);
                
                if(content[0]=='\n'){
                    contl++;
                }
                    contc++;
                
            } while(canRead >0);
            close(fd); 
        }else if (status == 0){
                int cont;
                char cr;
                while((cr=getchar())!=EOF){
                    if(cr=='\n'){
                        contl++;}
                    contc++;
                }
        }
            
            for (int i = 1; i < argc; i++)
            {
                if(strcmp(argv[i],"-l")==0){
                    printf("%d ",contl); fflush(stdout);
                }
                if(strcmp(argv[i],"-c")==0){
                    printf("%d ",contc); fflush(stdout);
                }
            }
            exit(0);
        }else if(child > 0){
            wait(NULL);
        }else if(child < 0){
            fprintf(stderr,"error..\n");
            exit(2);
        }


    return 0;
}