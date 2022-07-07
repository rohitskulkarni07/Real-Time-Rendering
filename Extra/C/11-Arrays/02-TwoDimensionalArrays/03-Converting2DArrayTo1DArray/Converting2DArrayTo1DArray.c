#include <stdio.h>

#define NUM_ROWS 5
#define NUM_COLS 3

int main(void) {

    int rsk_iArray2D[NUM_ROWS][NUM_COLS];
    int rsk_iArray1D[NUM_ROWS * NUM_COLS];

    int i,j;
    int rsk_num;

    printf("Enter Elements Of Your Choice To FillUp The Integer 2D Array : \n\n");

    for(i = 0; i < NUM_ROWS; i++) {
        
        printf("For Row Number %d :\n", (i+1));

        for(j = 0;j < NUM_COLS; j++) {

            printf("Enter Element Number %d : \n", (j+1));
            scanf("%d",&rsk_num);
            rsk_iArray2D[i][j] = rsk_num;
        }
        printf("\n\n");

    }

    printf("Array You Entered Is: \n\n");
    for(i = 0; i < NUM_ROWS; i++) {
        
        printf("***** Row %d ***** \n", (i+1));
        for(j = 0;j < NUM_COLS; j++) {
            printf("iArray_2D [%d][%d] = %d \n",i+1,j+1,rsk_iArray2D[i][j] );
        }
        printf("\n\n");
        
    }

//___________________________________________

    for(i = 0; i < NUM_ROWS; i++) {
        
        printf("For Row Number %d :\n", (i+1));

        for(j = 0;j < NUM_COLS; j++) {

            printf("(%d * %d) + %d = \n ",i , NUM_COLS, j,(i * NUM_COLS) + j);
            rsk_iArray1D[(i * NUM_COLS) + j] = rsk_iArray2D[i][j];

        }
        printf("\n\n");

    }

    printf("Converted 1DArray Is: \n\n");

    for(i = 0; i < NUM_ROWS * NUM_COLS; i++) {

            printf("iArray_1D[%d] = %d \n",i+1,rsk_iArray1D[i]);
    }

    printf("\n\n");

    return(0);
}
