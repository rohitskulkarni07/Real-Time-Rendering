#include <stdio.h>

int main(void) {

    int rsk_i,rsk_j;

    printf("\n\n");

    rsk_i = 1;

    while(rsk_i <= 10) {

        printf("i = %d\n",rsk_i);
        printf("--------\n\n");
        
        rsk_j = 1;
        while( rsk_j <= 5) {
                printf("\tj = %d\n", rsk_j);
                rsk_j++;
        }
        rsk_i++;
        printf("\n\n");
    }
    
    return(0);
} 