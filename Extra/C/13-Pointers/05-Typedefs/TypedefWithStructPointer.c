#include <stdio.h>
#include <stdlib.h>

struct MyData {

    int rsk_i;
    float rsk_f;
    double rsk_d;

};

int main(void) {

    int rsk_iSize;
    int rsk_fSize;
    int rsk_dSize;
    int struct_MyData_size;
    int pointer_to_struct_MyData_size;

    typedef struct MyData* MyDataPtr;

    MyDataPtr pData;

    printf("\n\n");

    pData = (MyDataPtr)malloc(sizeof(struct MyData));
    if(pData == NULL) {

        printf("Failed To Allocate Memory To 'struct MyData' !!! Exiting Now...");
        exit(0);
    } else {
        printf("Successfully Allocated Memory To 'struct MyData' !!!\n\n");

    }

    pData->rsk_i = 30;
    pData->rsk_f = 3.30f;
    pData->rsk_d = 3.123456789;

    printf("\n\n");
    printf("Data Members Of 'struct MyData' Are : \n\n");
    printf("%d", pData->rsk_i);
    printf("%f", pData->rsk_f);
    printf("%lf", pData->rsk_d);

    rsk_iSize = sizeof(pData->rsk_i);
    rsk_fSize = sizeof(pData->rsk_f);
    rsk_dSize = sizeof(pData->rsk_d);

    printf("\n\n");
    printf("Size (in bytes) Of Data Members Of 'strct MyData' Are \n\n");
    printf("Size of 'i' = %d bytes\n", rsk_iSize);
    printf("Size of 'i' = %d bytes\n", rsk_fSize);
    printf("Size of 'i' = %d bytes\n", rsk_dSize);

    struct_MyData_size = sizeof(struct MyData);
    pointer_to_struct_MyData_size = sizeof(MyDataPtr);

    printf("\n\n");
    printf("Size Of 'struct MyData' : %d bytes\n\n",struct_MyData_size);
    printf("Size Of Pointer To 'struct MyData' : %d bytes\n\n",pointer_to_struct_MyData_size);

    if(pData) {
        free(pData);
        pData = NULL;
        printf("Memory Allocated To 'struct MyData' Has Been Successfully Freed !!!\n\n");
    }

    return(0);
}

