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
    printf("***** ROW 1 *****");
    printf("\n\n");

    printf("iArray[0][0] =  %d\n",rsk_iArray[0][0]);
    printf("iArray[0][1] =  %d\n",rsk_iArray[0][1]);
    printf("iArray[0][2] =  %d\n",rsk_iArray[0][2]);

    printf("\n\n");
    printf("***** ROW 2 *****");
    printf("\n\n");

    
    printf("iArray[1][0] =  %d\n",rsk_iArray[1][0]);
    printf("iArray[1][1] =  %d\n",rsk_iArray[1][1]);
    printf("iArray[1][2] =  %d\n",rsk_iArray[1][2]);

    printf("\n\n");
    printf("***** ROW 3 *****");
    printf("\n\n");

    printf("iArray[2][0] =  %d\n",rsk_iArray[2][0]);
    printf("iArray[2][1] =  %d\n",rsk_iArray[2][1]);
    printf("iArray[2][2] =  %d\n",rsk_iArray[2][2]);

    printf("\n\n");
    printf("***** ROW 4 *****");
    printf("\n\n");
    
    printf("iArray[3][0] =  %d\n",rsk_iArray[3][0]);
    printf("iArray[3][1] =  %d\n",rsk_iArray[3][1]);
    printf("iArray[3][2] =  %d\n",rsk_iArray[3][2]);

    printf("\n\n");
    printf("***** ROW 5 *****");
    printf("\n\n");

    printf("iArray[4][0] =  %d\n",rsk_iArray[4][0]);
    printf("iArray[4][1] =  %d\n",rsk_iArray[4][1]);
    printf("iArray[4][2] =  %d\n",rsk_iArray[4][2]);

    printf("\n\n");

    return(0);
}