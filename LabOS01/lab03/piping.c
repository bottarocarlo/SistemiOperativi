#include <stdio.h>

char* upDown(char buf[]){
   for(int i=0;i<sizeof(buf);i++){
       if(buf[i] >= 'a' && buf[i]<= 'z'){
           buf[i] -= 32;
       }else if(buf[i] >= 'A' && buf[i]<= 'Z'){
           buf[i] += 32;
       }
   }
   return buf;
  
}

int main(){
    char buffer[10];
    fgets(buffer,sizeof(buffer),stdin);
    //
    //trasform a in A
    char* foo = upDown(buffer);
    printf("%s",foo);
    return 0;
}