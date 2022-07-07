#include <stdio.h>

int main(void) {

    int rsk_i;
    int rsk_j;
    int rsk_k;

    printf("\n\n"); 

    for(rsk_i = 1; rsk_i <= 10; rsk_i++) {

        printf("i = %d\n", rsk_i);
        printf("-----------\n\n");
        
        for(rsk_j = 1 ;rsk_j <= 5; rsk_j++) {
            
            printf("j = %d\n", rsk_j);
            printf("-----------\n\n");

            for(rsk_k = 1; rsk_k <= 3; rsk_k++) {
                
                printf("k = %d\n", rsk_k);
                printf("-----------\n\n");
            }
            printf("\n\n");

        }
        printf("\n\n");

    }
    printf("\n\n");

    return(0);
}