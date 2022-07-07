#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {
	//code

	int rsk_i;

	printf("\n\n");
	printf("Hello World !!!\n");
	printf("Number of argument = %d \n\n",argc);
	
	printf("Command Line Argument Passed To This Program Are : \n\n");

	for(rsk_i = 0; rsk_i < argc; rsk_i++) {
		printf("Argument Number %d = %s \n",(rsk_i+1), argv[rsk_i]);
	}
	printf("\n\n");
    
    printf("First 20 Environmental Variable Passed To This Program Are : \n\n");
   	for(rsk_i = 0; rsk_i < 20; rsk_i++) {
		printf("Environmental Variable Number %d = %s \n",(rsk_i+1), envp[rsk_i]);
	} 
	printf("\n\n");

	return(0);
}

