#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLS 3

int main(void ) {

    int rsk_iArray[NUM_ROWS][NUM_COLS];
    int i,j;

    for(i=0;i<NUM_ROWS; i++) {
        for(j = 0; j < NUM_COLS; j++) {
            *(rsk_iArray[i] + j) = (i + 1) * (j + 1);
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");

    for(i = 0; i < NUM_ROWS; i++) {
        for(j = 0; j <NUM_COLS; j++ ) {
            printf("*(iArray[%d] + %d) = %d\t\t At Addresses (iArray[i] + j) : %p\n", i, j, *(rsk_iArray[i] + j), (rsk_iArray[i] + j));
        }
        printf("\n\n");

    }
    
    return(0);
}

