#include <stdio.h>

int main(void) {

    int rsk_i,rsk_j;

    printf("\n\n");

    for(rsk_i = 1; rsk_i <= 10; rsk_i++) {
        printf("i = %d\n",rsk_i);
        printf("--------\n\n");
        
        for(rsk_j = 1; rsk_j <= 5; rsk_j++) {
                printf("\tj = %d\n", rsk_j);
        }
        printf("\n\n");
    }
    
    return(0);
} 