#include <unistd.h>
#include <stdio.h>

int main(){
    char s[100];
    chdir("..");
    printf("%s\n",getcwd(s,100));
    
}