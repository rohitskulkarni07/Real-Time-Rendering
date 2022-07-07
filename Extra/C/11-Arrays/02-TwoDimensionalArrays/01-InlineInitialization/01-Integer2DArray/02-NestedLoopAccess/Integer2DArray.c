#include <stdio.h>

int main(void) {

    int rsk_iArray[5][3] = { {1, 2, 3} , {4, 5, 6} , {7, 8, 9} , {1, 9, 5}, {2, 4, 2} };

    int rsk_iSize;

    int rsk_iArraySize;

    int rsk_iArrayNumElements, rsk_iArrayNumRows, rsk_iArrayNumCols;

    printf("\n\n");

    rsk_iSize = sizeof(int );
    rsk_iArraySize = sizeof(rsk_iArray );
    
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

    for(int i = 0;i<rsk_iArrayNumRows;i++) {

        printf("***** ROW %d *****\n",(i+1));

        for(int j=0;j<rsk_iArrayNumCols;j++){

            printf("iArray[%d][%d] =  %d\n",(i),(j),rsk_iArray[i][j]);
        }
        printf("\n\n");
    }
    
    return(0);
}