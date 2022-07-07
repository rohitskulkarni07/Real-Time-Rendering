#include <stdio.h>
#include <stdlib.h>

int main(void) {

    int **ptr_iArray = NULL;

    int i,j;
    int num_rows, num_columns;

    printf("\n\n");
    printf("Enter Number Of Rows : ");
    scanf("%d", &num_rows);

    printf("\n\n");
    printf("Enter Number Of Columns : ");
    scanf("%d", &num_columns);

    printf("\n\n");
    printf("*********** MEMORY ALOCATION TO 2D INTIGER ARRAY ***********\n\n");
    ptr_iArray = (int**)malloc(num_rows * sizeof(int));

    if(ptr_iArray == NULL) {
        printf("FAILED TO ALLOCATE TO %d ROWS OF 2D INTEGET ARRAY !!! EXITING MOW...\n\n",num_rows);
        exit(0);
    } else {
        printf("MEMORY ALLOCATION TO %d ROWS OF 2D INTEGER ARRAY SUCCEEDED !!!\n\n",num_rows);

    }


    for(i = 0; i < num_rows; i++) {

        ptr_iArray[i] = (int*)malloc(num_columns * sizeof(int));

        if(ptr_iArray[i] == NULL) {
            printf("FAILED TO ALLOCATE TO %d COLUMNS OF ROWS OF 2D INTEGET ARRAY !!! EXITING MOW...\n\n",i);
            exit(0);
        } else {
            printf("MEMORY ALLOCATION TO %d COLUMNS OF ROWS OF 2D INTEGER ARRAY SUCCEEDED !!!\n\n",i);

        }

    }

    for(i = 0; i < num_rows; i++) {

        for(j = 0; j < num_columns; j++) {
            ptr_iArray[i][j] = (i * 1) + (j * 1);
        }
    }

    for(i = 0; i < num_rows; i++) {

        printf("Base Address of Row %d : ptr_iArray[%d] = %p \t At Address : %p\n",i, i, ptr_iArray[i], &ptr_iArray[i]);
    }
    printf("\n");

    for(i = 0; i < num_rows; i++) {
        for(j = 0; j < num_columns; j++) { 
            printf("ptr_iArray[%d][%d] = %d\t At Address: %p\n",i ,j ,ptr_iArray[i][j], &ptr_iArray[i][j]);
        }
        printf("\n");
    }

    for(i = (num_rows-1); i >= 0; i--) {

        if(ptr_iArray[i]) {
            free(ptr_iArray[i]);
            ptr_iArray[i] = NULL;
            printf("MEMORY ALLOCATED TO ROW %d HAS BEEN SUCCESSFULLY FREED !!!\n\n",i);
        }
    }

    if(ptr_iArray) {
        free(ptr_iArray);
        ptr_iArray = NULL;
        printf("MEMORY ALLOCATED TO ptr_iArray HAS BEEN SUCCESSFULLY FREED !!!\n\n");
    }

    return(0);
}