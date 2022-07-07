#include <stdio.h>

int main(void) {

    int rsk_i;
    int rsk_j;
    int rsk_k;

    printf("\n\n"); 

    rsk_i = 1;
    while(rsk_i <= 10) {

        printf("i = %d\n", rsk_i);
        printf("-----------\n\n");
        
        rsk_j = 1 ;
        while(rsk_j <= 5) {
            
            printf("j = %d\n", rsk_j);
            printf("-----------\n\n");

            rsk_k = 1;
            while(rsk_k <= 3) {
                
                printf("k = %d\n", rsk_k);
                printf("-----------\n\n");
                rsk_k++;
            }

            rsk_j++;
            printf("\n\n");

        }
        rsk_i++;
        printf("\n\n");

    }
    printf("\n\n");

    return(0);
}