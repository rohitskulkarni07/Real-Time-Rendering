#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLS_ONE 3
#define NUM_COLS_TWO 8

int main(void) {

    int *iArray[NUM_ROWS];

    int i, j;

    printf("\n\n");
    printf("**************** FIRST MEMORY ALLOCATION TOW 2D INTEGER ARRAY ****************\n\n");

    for(i = 0; i < NUM_ROWS; i++ ) {
        
        iArray[i] = (int *)malloc(NUM_COLS_ONE * sizeof(int));

        if(iArray[i] == NULL) {
            printf("FAILED TO ALLOCATE MEMORY TO ROW %d OF 2D INTEGER ARRAY !!! EXITING NOW\n\n",i);
            exit(0);
        } else {
            printf("MEMORY ALLOCATION TO ROW %d OF 2D ARRAY SUCCEEDED !!!\n\n",i);

        }
    }


    for(i = 0; i < NUM_ROWS; i++) {   

        for(j = 0; j < NUM_COLS_ONE; j++) {

            iArray[i][j] = (i + 1) * (j + 1);    
        }

    }

    printf("\n\n");
    printf("Displaying 2D Array: \n\n");

    for(i = 0; i < NUM_ROWS; i++) {   

        for(j = 0; j < NUM_COLS_ONE; j++) {

                printf("iArray[%d][%d]  =  %d\n",i,j, iArray[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");

    for(i = (NUM_ROWS - 1); i >= 0; i --) {
        free(iArray[i]);
        iArray[i] = NULL;

        printf("MEMORY ALLOCATION TO ROW %d OF 2D INTEGER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n",i);
    }
//______________________________________________________________________________________________________________________
//______________________________________________________________________________________________________________________
    
    printf("\n\n");
    printf("**************** SECOND MEMORY ALLOCATION TOW 2D INTEGER ARRAY ****************\n\n");

    for(i = 0; i < NUM_ROWS; i++ ) {
        
        iArray[i] = (int *)malloc(NUM_COLS_TWO * sizeof(int));

        if(iArray[i] == NULL) {
            printf("FAILED TO ALLOCATE MEMORY TO ROW %d OF 2D INTEGER ARRAY !!! EXITING NOW\n\n",i);
            exit(0);
        } else {
            printf("MEMORY ALLOCATION TO ROW %d OF 2D ARRAY SUCCEEDED !!!\n\n",i);

        }
    }


    for(i = 0; i < NUM_ROWS; i++) {   

        for(j = 0; j < NUM_COLS_TWO; j++) {

            iArray[i][j] = (i + 1) * (j + 1);    
        }

    }

    printf("\n\n");
    printf("Displaying 2D Array: \n\n");

    for(i = 0; i < NUM_ROWS; i++) {   

        for(j = 0; j < NUM_COLS_TWO; j++) {

                printf("iArray[%d][%d]  =  %d\n",i,j, iArray[i][j]);
        }
        printf("\n\n");
    }
    printf("\n\n");

    for(i = (NUM_ROWS - 1); i >= 0; i --){
        free(iArray[i]);
        iArray[i] = NULL;

        printf("MEMORY ALLOCATION TO ROW %d OF 2D INTEGER ARRAY HAS BEEN SUCCESSFULLY FREED !!!\n\n",i);
    }
    return(0);
}
