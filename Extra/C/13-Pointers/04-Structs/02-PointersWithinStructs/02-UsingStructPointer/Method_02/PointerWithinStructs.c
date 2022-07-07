#include <stdio.h>

struct MyData {

    int *ptr_i;
    int i;

    float *ptr_f;
    float f; 

    double *ptr_d;
    double d;
};

int main(void) {

    struct MyData *pData = NULL;

    printf("\n\n");
    pData = (struct MyData *)malloc(sizeof(struct MyData));

    if(pData == NULL) {

        printf("Failed To Allocate Memory To 'struct MyData' !!! EXITING NOW...\n\n");

    } else {
        printf("Successfully Allocated Memory To 'struct MyData' !!!\n\n");

    }

    pData->i = 9;
    pData->ptr_i = &(pData->i);

    pData->f = 11.45f;
    pData->ptr_f = &(pData->f);

    pData->d = 30.12191995;
    pData->ptr_d = &(pData->d);

    printf("\n\n");
    printf("i = %d\n", *(pData->ptr_i));
    printf("Address Of 'i' : %p \n", pData->ptr_i);

    printf("\n\n");
    printf("f = %f\n", *(pData->ptr_f));
    printf("Address Of 'f' : %p \n", pData->ptr_f);

    printf("\n\n");
    printf("d = %lf\n", *(pData->ptr_d));
    printf("Address Of 'd' : %p \n", pData->ptr_d);
    
    if(pData) {

        free(pData);
        pData = NULL;
        printf("Memory Allocated To 'struct MyData' Has Been Successfully !!!\n\n");
        
    }

    return(0);

}

