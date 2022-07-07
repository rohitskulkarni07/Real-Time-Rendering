#include <stdio.h>

int main(void) {

    printf("\n\n");

    printf("Printing Digits 1 to 10 and 10 to 100 : \n\n");
    
    int rsk_i, rsk_j;

    for(rsk_i = 1, rsk_j = 10; rsk_i <= 10, rsk_j<=100; rsk_i++, rsk_j = rsk_j + 10){
        printf("\t %d \t %d\n", rsk_i, rsk_j);
    }

    printf("\n\n");

    return (0);

}