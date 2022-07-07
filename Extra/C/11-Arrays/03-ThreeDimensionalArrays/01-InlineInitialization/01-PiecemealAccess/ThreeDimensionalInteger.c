#include <stdio.h>

int main(void) {

    int rsk_iArray[5][3][2] = { { {9,  18}, {27, 53}, {45, 54} },
                            { {8,  16}, {23, 33}, {32, 11} },
                            { {99, 90}, {11, 12}, {12,  2} },
                            { {12, 32}, {12,  1}, {12, 33} },
                            { {35,  2}, {16,  2}, {27, 12} }  };

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

//____________________________________________________
    
    printf("****** ROW 1 ******\n\n");
    printf("****** COLOUMN 1 *******\n");

    printf("iArray[0][0][0] = %d\n",rsk_iArray[0][0][0]);
    printf("iArray[0][0][1] = %d\n",rsk_iArray[0][0][1]);

    printf("\n");
    printf("****** COLOUMN 2 *******\n");

    printf("iArray[0][1][0] = %d\n",rsk_iArray[0][1][0]);
    printf("iArray[0][1][1] = %d\n",rsk_iArray[0][1][1]);

    printf("\n");
    printf("****** COLOUMN 3 *******\n");

    printf("iArray[0][2][0] = %d\n",rsk_iArray[0][2][0]);
    printf("iArray[0][2][1] = %d\n",rsk_iArray[0][2][1]);

//____________________________________________________


    printf("\n");
    printf("****** ROW 2 ******\n\n");
    printf("****** COLOUMN 1 *******\n");

    printf("iArray[1][0][0] = %d\n",rsk_iArray[1][0][0]);
    printf("iArray[1][0][1] = %d\n",rsk_iArray[1][0][1]);

    printf("\n");
    printf("****** COLOUMN 2 *******\n");
  
    printf("iArray[1][1][0] = %d\n",rsk_iArray[1][1][0]);
    printf("iArray[1][1][1] = %d\n",rsk_iArray[1][1][1]);

    printf("\n");
    printf("****** COLOUMN 3 *******\n");
  
    printf("iArray[1][2][0] = %d\n",rsk_iArray[1][2][0]);
    printf("iArray[1][2][1] = %d\n",rsk_iArray[1][2][1]);


//____________________________________________________

    printf("\n");
    printf("****** ROW 3 ******\n\n");
    printf("****** COLOUMN 1 *******\n");
  
    printf("iArray[2][0][0] = %d\n",rsk_iArray[2][0][0]);
    printf("iArray[2][0][1] = %d\n",rsk_iArray[2][0][1]);

    printf("\n");
    printf("****** COLOUMN 2 *******\n");
  
    printf("iArray[2][1][0] = %d\n",rsk_iArray[2][1][0]);
    printf("iArray[2][1][1] = %d\n",rsk_iArray[2][1][1]);

    printf("\n");
    printf("****** COLOUMN 3 *******\n");
  
    printf("iArray[2][2][0] = %d\n",rsk_iArray[2][2][0]);
    printf("iArray[2][2][1] = %d\n",rsk_iArray[2][2][1]);


//____________________________________________________


    printf("\n");
    printf("****** ROW 4 ******\n\n");
    printf("****** COLOUMN 1 *******\n");
  
    printf("iArray[3][0][0] = %d\n",rsk_iArray[3][0][0]);
    printf("iArray[3][0][1] = %d\n",rsk_iArray[3][0][1]);

    printf("\n");
    printf("****** COLOUMN 2 *******\n");
  
    printf("iArray[3][1][0] = %d\n",rsk_iArray[3][1][0]);
    printf("iArray[3][1][1] = %d\n",rsk_iArray[3][1][1]);

    printf("\n");
    printf("****** COLOUMN 3 *******\n");
    printf("iArray[3][2][0] = %d\n",rsk_iArray[3][2][0]);
    printf("iArray[3][2][1] = %d\n",rsk_iArray[3][2][1]);


//____________________________________________________



    printf("\n");
    printf("****** ROW 5******\n\n");
    printf("****** COLOUMN 1 *******\n");
 
    printf("iArray[4][0][0] = %d\n",rsk_iArray[4][0][0]);
    printf("iArray[4][0][1] = %d\n",rsk_iArray[4][0][1]);

    printf("\n");
    printf("****** COLOUMN 2 *******\n");
 
    printf("iArray[4][1][0] = %d\n",rsk_iArray[4][1][0]);
    printf("iArray[4][1][1] = %d\n",rsk_iArray[4][1][1]);

    printf("\n");
    printf("****** COLOUMN 3 *******\n");
 
    printf("iArray[4][2][0] = %d\n",rsk_iArray[4][2][0]);
    printf("iArray[4][2][1] = %d\n",rsk_iArray[4][2][1]);

//____________________________________________________

    return(0);

}