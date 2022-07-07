#include <stdio.h>
#include <stdlib.h>

#define NUM_ROWS 5
#define NUM_COLS 5

int main(void) {

    int *iArray[NUM_ROWS]; 
    int i, j;

    printf("\n\n");
    printf("******** MEMORY ALLOCATION 2D INTEGER ARRAY*******\n\n");

    for(i = 0; i < NUM_ROWS; i++) {

        iArray[i] = (int *)malloc((NUM_COLS - i) * sizeof(int ));

        if(iArray[i] == NULL) {
            printf("FAILED TO ALLOCATE MEMOEY TO ROW %d OF 2D INTEGER ARRAY !!!EXITING NOW...\n\n",i);
            exit(0);
        } else {
            printf("MEMORY ALLOCATION TO ROW %d OF 2D INTEGER ARRAY SUCCEDED !!!\n\n", i);
        }

    }

    for(i = 0; i < NUM_ROWS; i++) {
        for(j = 0; j < (NUM_COLS - i); j++) {

            iArray[i][j] = (i * 1) +(j * 1);

        }
    }

    printf("**** DISPLAY 2D ARRAY *********\n\n");

    for(i = 0; i < NUM_ROWS; i++) {
        for(j = 0; j < (NUM_COLS - i); j++) {
            printf("iArray[%d][%d] = %d\t\tAt Address : %p\n" ,i,j,iArray[i][j],&iArray[i][j]);
        }
        printf("\n");;
    }


    for(i= (NUM_COLS - 1); i>= 0; i--) {

        if(iArray[i]) {
            free(iArray[i]);
            iArray[i] = NULL;
        }
        printf("MEMORY ALLOCATE TO ROW oF 2D INTEGER ARRAY IS SUCCESSFULLY FREED!!!\n\n");
    }

    return(0);
}

