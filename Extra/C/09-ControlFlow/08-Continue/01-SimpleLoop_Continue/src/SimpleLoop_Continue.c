#include <stdio.h>

int main(void) {
	
	int rsk_i;

	printf("\n\n");

	printf("Printing Even Numbers From 0 to 100: \n\n");

	for (rsk_i = 0; rsk_i <= 100; rsk_i++) {
	
		if (rsk_i%2 != 0) {
			continue;
		}
		else {
			printf("\t%d\n",rsk_i);
		}

	}

	printf("\n\n");

	return(0);
}