#include <stdio.h>
#include <stdlib.h>

#define INT_SIZE sizeof(int)
#define FLOAT_SIZE sizeof(float)
#define DOUBLE_SIZE sizeof(double)
#define CHAR_SIZE sizeof(char)

int main(void) {

    int *ptr_iArray = NULL;
    unsigned int intArrayLength = 0;
    
    float *ptr_fArray = NULL;
    unsigned int floatArrayLength = 0;

    double *ptr_dArray = NULL;
    unsigned int doubleArrayLength = 0;
    
    char *ptr_cArray = NULL;
    unsigned int charArrayLength = 0;
    
    int i;

//_____________________________________________________________________________
//                          
//                         INTEGER ARRAY                                 
//_____________________________________________________________________________
    printf("\n\n");
    printf("Enter The Number Of Elements You Want In Your Integer Array:");
    scanf("%u", &intArrayLength);

    ptr_iArray = (int *)malloc(INT_SIZE * intArrayLength);

    if(ptr_iArray == NULL) {

        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY HAS FAILED !!!EXITING NOW...");
        printf("\n\n");

    } else {

        printf("\n\n");
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY HAS SUCCEEDED !!!\n\n");
    }

    printf("\n\n");
    
    printf("Enter %d Elements For The INTEGER Array \n\n", intArrayLength);

    printf("\n\n");

    for(i = 0; i < intArrayLength;i++) {
        scanf("%d",(ptr_iArray + i));
    }
    printf("\n\n");


//_____________________________________________________________________________
//                          
//                         FLOAT ARRAY                                 
//_____________________________________________________________________________
    printf("\n\n");
    printf("Enter The Number Of Elements You Want In Your Float Array:");
    scanf("%u", &floatArrayLength);

    ptr_fArray = (float *)malloc(FLOAT_SIZE * floatArrayLength);

    if(ptr_fArray == NULL) {

        printf("\n\n");
        printf("MEMORY ALLOCATION FOR FLOAT ARRAY HAS FAILED !!!EXITING NOW...");
        printf("\n\n");
        exit(0);
    } else {

        printf("\n\n");
        printf("MEMORY ALLOCATION FOR FLOAT ARRAY HAS SUCCEEDED !!!\n\n");
    }

    printf("\n\n");
    
    printf("Enter %d Elements For The FLOAT Array \n\n", floatArrayLength);

    printf("\n\n");

    for(i = 0; i < floatArrayLength;i++) {
        scanf("%f",(ptr_fArray + i));
    }
    printf("\n\n");


//_____________________________________________________________________________
//                          
//                         DOUBLE ARRAY                                 
//_____________________________________________________________________________
    printf("\n\n");
    printf("Enter The Number Of Elements You Want In Your DOUBLE Array:");
    scanf("%u", &doubleArrayLength);

    ptr_dArray = (double *)malloc(DOUBLE_SIZE * doubleArrayLength);

    if(ptr_dArray == NULL) {

        printf("\n\n");
        printf("MEMORY ALLOCATION FOR DOUBLE ARRAY HAS FAILED !!!EXITING NOW...");
        printf("\n\n");
        exit(0);

    } else {

        printf("\n\n");
        printf("MEMORY ALLOCATION FOR DOUBLE ARRAY HAS SUCCEEDED !!!\n\n");
    }

    printf("\n\n");
    
    printf("Enter %d Elements For The DOUBLE Array \n\n", doubleArrayLength);

    printf("\n\n");

    for(i = 0; i < doubleArrayLength;i++) {
        scanf("%lf",(ptr_dArray + i));
    }
    printf("\n\n");


//_____________________________________________________________________________
//                          
//                         CHARACTER ARRAY                                 
//_____________________________________________________________________________
    printf("\n\n");
    printf("Enter The Number Of Elements You Want In Your CHARACTER Array:");
    scanf("%u", &charArrayLength);

    ptr_cArray = (char *)malloc(CHAR_SIZE * charArrayLength);

    if(ptr_cArray == NULL) {

        printf("\n\n");
        printf("MEMORY ALLOCATION FOR CHARACTER ARRAY HAS FAILED !!!EXITING NOW...");
        printf("\n\n");
        exit(0);

    } else {

        printf("\n\n");
        printf("MEMORY ALLOCATION FOR CHARACTER ARRAY HAS SUCCEEDED !!!\n\n");
    }

    printf("\n\n");
    
    printf("Enter %d Elements For The CHARACTER Array \n\n", charArrayLength);

    printf("\n\n");

    for(i = 0; i < charArrayLength; i++) {
        *(ptr_cArray + i) = getch();
        printf("%c",*(ptr_cArray + i));
    }
    printf("\n\n");

    
//_________________________________________________________________________________________________
//
//                          DISPLAY INTEGER ARRAY
//_________________________________________________________________________________________________
    printf("The Integer Array Entered By You, Consisting Of %d Elements : \n",intArrayLength);

    printf("\n\n");
    
    for(i = 0; i < intArrayLength;i++) {
        printf("*(ptr_iArray + %d) = %d\t At Address (ptr_iArray + %d) : %p\n", i, *(ptr_iArray + i), i, (ptr_iArray + i));

    }

    printf("\n\n");

   
//_________________________________________________________________________________________________
//
//                          DISPLAY FLOAT ARRAY
//_________________________________________________________________________________________________

    printf("The Float Array Entered By You, Consisting Of %d Elements : \n",floatArrayLength);

    printf("\n\n");
    
    for(i = 0; i < floatArrayLength;i++) {
        printf("*(ptr_fArray + %d) = %f\t At Address (ptr_fArray + %d) : %p\n", i, *(ptr_fArray + i), i, (ptr_fArray + i));

    }

    printf("\n\n");

//_________________________________________________________________________________________________
//
//                          DISPLAY DOUBLE ARRAY
//_________________________________________________________________________________________________

    printf("The Double Array Entered By You, Consisting Of %d Elements : \n",doubleArrayLength);

    printf("\n\n");
    
    for(i = 0; i < doubleArrayLength;i++) {
        printf("*(ptr_dArray + %d) = %lf\t At Address (ptr_lfArray + %d) : %p\n", i, *(ptr_dArray + i), i, (ptr_dArray + i));

    }

    printf("\n\n");


//_________________________________________________________________________________________________
//
//                          DISPLAY CHARACTER ARRAY
//_________________________________________________________________________________________________

    printf("The Character Array Entered By You, Consisting Of %d Elements : \n",charArrayLength);

    printf("\n\n");
    
    for(i = 0; i < charArrayLength;i++) {
        printf("*(ptr_cArray + %d) = %c\t At Address (ptr_cArray + %d) : %p\n", i, *(ptr_cArray + i), i, (ptr_cArray + i));

    }

    printf("\n\n");

//_________________________________________________________________________________________________
//
//                  FREEING MEMORY OCCUPIED BY ARRAYS
//_________________________________________________________________________________________________

    if(ptr_cArray) {
        free(ptr_cArray);
        ptr_cArray = NULL;
        printf("MEMORY ALLOCATION FOR CHARACTER ARRAY HAS SUCCESSFULLY FREED !!!\n\n");

    }

    if(ptr_dArray) {
        free(ptr_dArray);
        ptr_dArray = NULL;
        printf("MEMORY ALLOCATION FOR DOUBLE ARRAY HAS SUCCESSFULLY FREED !!!\n\n");

    }

    if(ptr_fArray) {
        free(ptr_fArray);
        ptr_fArray = NULL;
        printf("MEMORY ALLOCATION FOR FLOAT ARRAY HAS SUCCESSFULLY FREED !!!\n\n");

    }

    if(ptr_iArray) {
        free(ptr_iArray);
        ptr_iArray = NULL;
        printf("MEMORY ALLOCATION FOR INTEGER ARRAY HAS SUCCESSFULLY FREED !!!\n\n");

    }

    printf("\n\n");

    return(0);
}
