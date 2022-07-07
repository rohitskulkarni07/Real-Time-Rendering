#include <stdio.h>

int main(void) {

    int iArrayOne[5];
    int iArrayTwo[5][3];
    int iArrayThree[100][100][5];

    int num_rows_2D;
    int num_columns_2D;
    
    int num_rows_3D;
    int num_columns_3D;
    int depth_3D;

    printf("\n\n");
    printf("Sizes Of 1-D Integer Array iArrayOne = %lu\n\n", sizeof(iArrayOne));
    printf("Number Of Elements I 1-D Integer Array iArrayOne = %lu\n", (sizeof(iArrayOne)/sizeof(int)));

    printf("\n\n");
    
    printf("Size Of 2-D Integer iArrayTwo = %lu\n", sizeof(iArrayTwo));
    
    printf("\n\n");
    
    printf("Number Of Rows in 2-D Integer iArrayTwo = %lu\n", sizeof(iArrayTwo)/sizeof(iArrayTwo[0]));
    num_rows_2D = (sizeof(iArrayTwo) / sizeof(iArrayTwo[0]));

    printf("\n\n");

    printf("Number Of Columns in 2-D Integer iArrayTwo = %lu\n", sizeof(iArrayTwo[0])/sizeof(iArrayTwo[0][0]));
    num_columns_2D = (sizeof(iArrayTwo[0]) / sizeof(iArrayTwo[0][0]));

    printf("\n\n");

    printf("Number Of Elements in 2-D Integer iArrayTwo = %lu\n", (num_rows_2D * num_columns_2D));

    printf("\n\n");
    printf("\n\n");

    printf("Size Of 3-D Integer iArrayThree = %lu\n", sizeof(iArrayThree));

    printf("\n\n");

    printf("Number Of Rows in 3-D Integer iArrayThree = %lu\n", sizeof(iArrayThree)/sizeof(iArrayThree[0]));
    num_rows_3D = (sizeof(iArrayThree) / sizeof(iArrayThree[0]));

    printf("\n\n");

    printf("Number Of Columns in 3-D Integer iArrayThree = %lu\n", sizeof(iArrayThree[0])/sizeof(iArrayThree[0][0]));
    num_columns_3D = (sizeof(iArrayThree[0]) / sizeof(iArrayThree[0][0]));

    printf("\n\n");
 
    printf("Number Of Depth in 3-D Integer iArrayThree = %lu\n", sizeof(iArrayThree[0][0])/sizeof(iArrayThree[0][0][0]));
    depth_3D =  (sizeof(iArrayThree[0][0]) / sizeof(iArrayThree[0][0][0])); 

    printf("\n\n");

    printf("Number Of Elements in 2-D Integer iArrayThree = %lu\n", (num_rows_3D * num_columns_3D));

    return(0);
}


