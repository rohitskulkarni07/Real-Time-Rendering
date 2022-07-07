#include <stdio.h>
#include <stdlib.h>

struct MyData {

    int rsk_i;
    float rsk_f;
    double rsk_d;

};

int main(void) {

    void ChangeValues(struct MyData *);

    struct MyData *pData = NULL;

    printf("\n\n");

    pData = (struct MyData *)malloc(sizeof(struct MyData));
    
    if(pData == NULL) {
        printf("Failed To Allocate Memory To 'struct MyData' !!! Exiting Now...\n\n");
        exit(0);
    } else {
        printf("Successfully Allocated Memory To 'struct MyData' !!! \n\n");
    }

    pData->rsk_i = 30;
    pData->rsk_f = 30.03f;
    pData->rsk_d = 3.12345568;

    printf("\n\n");
    printf("Data Members Of 'struct MyData' Are : \n\n");
    printf("i = %d\n", pData->rsk_i);
    printf("f = %f\n", pData->rsk_f);
    printf("d = %lf\n", pData->rsk_d);

    ChangeValues(pData);

    printf("\n\n");
    printf("Data Members Of 'struct MyData' Are : \n\n");
    printf("i = %d\n", pData->rsk_i);
    printf("f = %f\n", pData->rsk_f);
    printf("d = %lf\n", pData->rsk_d);

    if(pData) {

        free(pData);
        pData = NULL;
        printf("Memory Allocated TO 'struct MyData' Has Been Successfully Freed !!! \n\n");
    }

    return(0);
}

void ChangeValues(struct MyData *pParam_Data) {

    pParam_Data ->rsk_i = 9;
    pParam_Data ->rsk_f = 9.99f;
    pParam_Data ->rsk_d = 9.9876543;
    
}