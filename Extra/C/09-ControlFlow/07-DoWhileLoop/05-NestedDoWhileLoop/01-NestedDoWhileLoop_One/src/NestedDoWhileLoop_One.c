#include <stdio.h>

int main(void) {

    int rsk_i,rsk_j;

    printf("\n\n");

    rsk_i = 1;

   do {

        printf("i = %d\n",rsk_i);
        printf("--------\n\n");
        
        rsk_j = 1;
        do {
                printf("\tj = %d\n", rsk_j);
                rsk_j++;
        }while( rsk_j <= 5);
        
        rsk_i++;
        printf("\n\n");

    }while(rsk_i <= 10);
    
    return(0);
} 