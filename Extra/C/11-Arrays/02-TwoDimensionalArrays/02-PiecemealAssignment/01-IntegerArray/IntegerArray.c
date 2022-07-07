#include <stdio.h>


int main(void) {

    int rsk_iArray[3][5];

    int rsk_iSize;
    int rsk_iArraySize;
    int rsk_iArrayNumElements, rsk_iArrayNumRows, rsk_iArrayNumCols;

    rsk_iSize = sizeof(int );
    rsk_iArraySize = sizeof(rsk_iArray );
    printf("\n\n");
    
    printf("Size of 2D Integer Array is =  %d\n\n",rsk_iArraySize);

    rsk_iArrayNumRows = rsk_iArraySize / sizeof(rsk_iArray[0]);
    rsk_iArrayNumCols = sizeof(rsk_iArray[0]) / rsk_iSize;
    rsk_iArrayNumElements = rsk_iArrayNumRows * rsk_iArrayNumCols;

    printf("Number of Rows     in 2D Array: %d\n",rsk_iArrayNumRows);
    printf("Number of Columns  in 2D Array: %d\n",rsk_iArrayNumCols);
    printf("Number of Elements in 2D Array: %d\n",rsk_iArrayNumElements);
    
    printf("\n\n");
    printf("Elements in 2D Array : ");
    printf("\n\n");
    
    //***** ROW 1 *****
    rsk_iArray[0][0] = 11;
    rsk_iArray[0][1] = 12;
    rsk_iArray[0][2] = 13;
    rsk_iArray[0][3] = 14;
    rsk_iArray[0][4] = 15;

     //***** ROW 2 *****
    rsk_iArray[1][0] = 21;
    rsk_iArray[1][1] = 22;
    rsk_iArray[1][2] = 23;
    rsk_iArray[1][3] = 24;
    rsk_iArray[1][4] = 25;

    //***** ROW 1 *****
    rsk_iArray[2][0] = 31;
    rsk_iArray[2][1] = 32;
    rsk_iArray[2][2] = 33;
    rsk_iArray[2][3] = 34;
    rsk_iArray[2][4] = 35;

    for(int i = 0; i < rsk_iArrayNumRows; i++) {
        
        printf("***** ROW %d *****\n",(i+1));

        for(int j = 0; j < rsk_iArrayNumCols; j++) {

            printf("iArray[%d][%d] = %d\n", (i+1), (j+1), rsk_iArray[i][j]);
        }
        printf("\n\n");
    }

    printf("\n\n");

    return(0);
}

