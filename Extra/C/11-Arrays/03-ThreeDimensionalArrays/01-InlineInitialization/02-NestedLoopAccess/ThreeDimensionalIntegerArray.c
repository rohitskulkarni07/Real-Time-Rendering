#include <stdio.h>

int main(void) {

    int rsk_iArray[5][3][2] = { { {9,  18}, {27, 53}, {45, 54} },
                            { {8,  16}, {23, 33}, {32, 11} },
                            { {99, 90}, {11, 12}, {12,  2} },
                            { {12, 32}, {12,  1}, {12, 33} },
                            { {35,  2}, {16,  2}, {27, 12} }  };

    int i,j,k;
    int rsk_iSize;
    int rsk_iArray_size;
    int rsk_iArrayNumElements, rsk_iArrayWidth, rsk_iArrayHeight, rsk_iArrayDepth;

    printf("\n\n");

    rsk_iSize = sizeof(int);
    rsk_iArray_size = sizeof(rsk_iArray);
    rsk_iArrayWidth = rsk_iArray_size / sizeof(rsk_iArray[0]);
    rsk_iArrayHeight = sizeof(rsk_iArray[0]) / sizeof(rsk_iArray[0][0]);
    rsk_iArrayDepth = sizeof(rsk_iArray[0][0]) / rsk_iSize;
    rsk_iArrayNumElements = rsk_iArrayWidth * rsk_iArrayHeight * rsk_iArrayDepth;

    printf("Size Of 3D Array Is = %d\n\n",rsk_iArray_size);
    printf("Number Of Rows In 3D Array = %d", rsk_iArrayWidth);
    printf("Number Of Columns In 3D Array = %d", rsk_iArrayHeight);
    printf("Number Of Depth In 3D Array = %d", rsk_iArrayDepth);
    
    printf("\n\n");
    printf("Elements of In 3D Array\n");


    for(i = 0; i < rsk_iArrayWidth; i++) {
        
        printf("****** ROW %d ******\n\n", (i+1));
        for(j = 0; j < rsk_iArrayHeight; j++) {

            printf("****** COLOUMN %d *******\n\n",(j+1));
            for(k = 0; k < rsk_iArrayDepth; k++) {

                printf("iArray[%d][%d][%d] = %d\n", i, j, k, rsk_iArray[i][j][k]);

            }
            printf("\n");
        }
        printf("\n\n");
    }

    return(0);

}
