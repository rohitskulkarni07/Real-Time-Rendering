#include <stdio.h>

int main(void) {

    int rsk_i;
    int rsk_j;
    int rsk_k;

    printf("\n\n"); 

    rsk_i = 1;
    do {

        printf("i = %d\n", rsk_i);
        printf("-----------\n\n");
        
        rsk_j = 1 ;
        do {
            
            printf("j = %d\n", rsk_j);
            printf("\t-----------\n\n");

            rsk_k = 1;
            do {
                
                printf("\tk = %d\n", rsk_k);
                rsk_k++;
            } while(rsk_k <= 3);

            rsk_j++;
            printf("\n\n");

        }while(rsk_j <= 5);
        rsk_i++;
        printf("\n\n");

    }while(rsk_i <= 10) ;
    printf("\n\n");

    return(0);
}