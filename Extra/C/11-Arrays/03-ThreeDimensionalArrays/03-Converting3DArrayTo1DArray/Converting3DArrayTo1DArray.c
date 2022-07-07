#include <stdio.h>

#define NUM_ROWS 5
#define NUM_COLS 3
#define DEPTH 2

int main(void) {

    int rsk_iArray[5][3][2] = { { {9,  18}, {27, 53}, {45, 54} },
                            { {8,  16}, {23, 33}, {32, 11} },
                            { {99, 90}, {11, 12}, {12,  2} },
                            { {12, 32}, {12,  1}, {12, 33} },
                            { {35,  2}, {16,  2}, {27, 12} }  };

    int i,j,k;

    int rsk_iArray1D[NUM_ROWS * NUM_COLS * DEPTH];

    printf("\n\n");
    printf("Elements of In 3D Array\n");

    for(i = 0; i < NUM_ROWS; i++) {
        
        printf("****** ROW %d ******\n\n", (i+1));
        for(j = 0; j < NUM_COLS; j++) {

            printf("****** COLOUMN %d *******\n\n",(j+1));
            for(k = 0; k < DEPTH; k++) {

                printf("iArray[%d][%d][%d] = %d\n", i, j, k, rsk_iArray[i][j][k]);

            }
            printf("\n");
        }
        printf("\n\n");
    }

    //CONVERITNG 3D Array TO 1D Array

    for(i = 0; i < NUM_ROWS; i++) {
        
        for(j = 0; j < NUM_COLS; j++) {

            for(k = 0; k < DEPTH; k++) {

                rsk_iArray1D[((i * NUM_COLS * DEPTH) + (j * DEPTH) + k)] = rsk_iArray[i][j][k];
            }
        }
    }

    printf("\n\n");
    
    for(i = 0; i < (NUM_ROWS * NUM_COLS * DEPTH); i++) {

        printf("iArray[%d] = %d\n", i, rsk_iArray1D[i]);
        printf("\n\n");
    }

    return(0);

}

