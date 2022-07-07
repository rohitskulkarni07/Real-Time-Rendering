#include <stdio.h>

struct MyData {

    int i;
    float f; 
    double d;
};

int main(void) {

    int iSize;
    int fSize;
    int dSize;

    int struct_MyData_size;
    int pointer_to_struct_MyData_size;

    struct  MyData *pData = NULL;

    printf("\n\n");

    pData = (struct MyData *)malloc(sizeof(struct MyData));

    if(pData == NULL) {
        printf("Failed To Allocate Memory To 'struct MyData' !!! Exiting Now...\n\n");
        exit(0);
    } else {
        printf("Successfully Allocated Memory To 'struct MyData' !!!\n\n");
    }
    

    (*pData).i = 30;
    (*pData).f = 11.45f;
    (*pData).d = 1.29995;

    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");
    printf("i = %d\n",(*pData).i);
    printf("f = %f\n",(*pData).f);
    printf("d = %lf\n",(*pData).d);

    iSize = sizeof((*pData).i);
    fSize = sizeof((*pData).f);
    dSize = sizeof((*pData).d);

    printf("\n\n");
    printf("SIZES (in Bytes) OF DATA MEMBERS OF 'struct MyData' ARE: \n\n");
    printf("Size of 'i' = %d bytes\n", iSize);
    printf("Size of 'f' = %d bytes\n", fSize);
    printf("Size of 'd' = %d bytes\n", dSize);

    struct_MyData_size = sizeof(struct MyData);
    pointer_to_struct_MyData_size = sizeof(struct MyData *);

    printf("\n\n");
    printf("Size of 'struct MyData' : %d bytes \n\n",struct_MyData_size);
    printf("Size of Pointer To 'struct MyData' : %d bytes\n\n", pointer_to_struct_MyData_size);

    if(pData) {
        free(pData);
        pData = NULL;
        printf("Memory Allocated To 'struct MyData' Has Been Successfully Freed !!! \n\n");

    }

    return(0);

}

