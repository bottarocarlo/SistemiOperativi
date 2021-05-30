#include <string.h>
#include <stdio.h>
#include <stdlib.h>



int main(int argc, char ** argv){
	int status =0;
    int cont;
	if(argc<2){
		printf("scemo usage : ./lengthof.o \"<string>\"");
		status = -1;
	}else{
		for(int i =0; argv[1][i] != '\0' ;i++){
			cont++;
		}
	}
	
	printf("lunghezza: %d \nlunghezza: %lu",cont,strlen(argv[1]));

	return status;
}