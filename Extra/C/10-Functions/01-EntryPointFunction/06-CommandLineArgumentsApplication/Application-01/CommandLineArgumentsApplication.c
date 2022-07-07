#include <ctype.h>  //atoi()
#include <stdio.h>  
#include <stdlib.h> //exit()

int main(int argc, char *argv[], char *envp[]) {
	//code

	int rsk_i;
    int rsk_num;
    int rsk_sum = 0;

    if(argc ==1 ){
        printf("\n\nNo Arguments Are Given\n\nEXITTING NOW.....!\n\n");
        exit(0); //stdlib
    }

	printf("\n\n");
	
	printf("Sum of All Integer Command Line Argument Is :\n\n");

	for(rsk_i = 1; rsk_i < argc; rsk_i++) {
        
        rsk_num = atoi(argv[rsk_i]);
        rsk_sum  = rsk_sum + rsk_num;
	}
	printf("SUM : %d \n\n",rsk_sum);
	printf("\n\n");

	return(0);
}

