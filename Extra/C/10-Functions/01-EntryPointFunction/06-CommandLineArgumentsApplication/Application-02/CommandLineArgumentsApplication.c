#include <ctype.h>  //atoi()
#include <stdio.h>  
#include <stdlib.h> //exit()

int main(int argc, char *argv[], char *envp[]) {
	//code

	int rsk_i;

    if(argc != 4 ){
        printf("\n\nINVALID USAGE :\n\nEXITTING NOW.....!\n\n");
        printf("Usage : ommandLineArgumentsApplication <first name> <middle name> <surname>\n\n");
        exit(0); //stdlib
    }

	printf("\n\n");
	
	printf("Your Full-Name Is  :\n\n%-15s %-15s %-15s\n","first name" ,"middle name", "surname");

	for(rsk_i = 1; rsk_i < argc; rsk_i++) {
        printf("%-15s ", argv[rsk_i]);
	}
	printf("\n\n");

	return(0);
}

