#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLS 3

int main(void ) {

    int i,j;
    int **ptr_iArray = NULL;
    
    printf("\n\n");

    ptr_iArray = (int **)malloc(NUM_ROWS * sizeof(int *));

    if(ptr_iArray == NULL) {

        printf("MEMORY ALLOCATION TO THE 1D ARRAY OF BASE ADDRESSES OF %d ROWS FAILED !!! EXITING NOW...\n\n", NUM_ROWS);
        exit(0);
    } else {
        printf("MEMORY ALLOCATION TO THE 1D ARRAY OF BASE ADDRESSES OF %d ROWS HAS SUCCEDDED !!!\n\n",NUM_ROWS);
    }

    for(i=0;i<NUM_ROWS; i++) {

        ptr_iArray[i] = (int *) malloc(NUM_COLS * sizeof(int));

          if(ptr_iArray == NULL) { 

            printf("MEMORY ALLOCATION TO THE COLOUMNS OF ROW %d FAILED !!! EXITING NOW...\n\n", NUM_ROWS);
            exit(0);
        } else {
            printf("MEMORY ALLOCATION TO THE COLOUMNS OF ROWS %d HAS SUCCEDDED !!!\n\n",NUM_ROWS);
        }

    }

    for(i = 0; i < NUM_ROWS; i++) {

        for(j = 0; j < NUM_COLS; j++) {

            *(*(ptr_iArray + i) + j) = (i + 1) * (j + 1); //ptr_iArray[i][j] = (i+1) * (j+1)
        }
    }

    printf("\n\n");
    printf("2D Integer Array Elements Along With Addresses : \n\n");

    for(i = 0; i < NUM_ROWS; i++) {
        for(j = 0; j <NUM_COLS; j++ ) {
            printf("ptr_iArray_Row[%d][%d] = %d\t\t At Addresses &ptr_iArray_Row[%d][%d]: %p\n", i, j, ptr_iArray[i][j], i, j, &ptr_iArray[i][j]);
        }
        printf("\n\n");

    }

    for(i = (NUM_ROWS - 1); i >= 0; i--) {
        if(*(ptr_iArray + i)) {
            
            free(*(ptr_iArray + i));
            *(ptr_iArray + i) = NULL;
            printf("MEMORY ALLOCATED TO ROW %d HAS BEEN SUCCESSFULLY FREED !!!\n\n",i);

        }
        if(ptr_iArray) {
            
            free(ptr_iArray + i);
            ptr_iArray  = NULL;
            printf("MEMORY ALLOCATED TO ptr_iArray HAS BEEN SUCCESSFULLY FREED !!!\n\n",i);

        }
    }
    
    return(0);
}

