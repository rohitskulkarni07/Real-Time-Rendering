#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLS 3

int main(void ) {

    int rsk_iArray[NUM_ROWS][NUM_COLS];
    int i,j;

    int *ptr_iArray_Row = NULL;

    for(i=0;i<NUM_ROWS; i++) {

        ptr_iArray_Row = rsk_iArray[i];

        for(j = 0; j < NUM_COLS; j++) {
        
            *(ptr_iArray_Row + j) = (i + 1) * (j + 1);
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");

    for(i = 0; i < NUM_ROWS; i++) {
        for(j = 0; j <NUM_COLS; j++ ) {
            printf("*(ptr_iArray_Row + %d ) = %d\t\t At Addresses (ptr_iArray_Row + j) : %p\n",  j, *(ptr_iArray_Row + j), (ptr_iArray_Row + j));
        }
        printf("\n\n");

    }
    
    return(0);
}

