#include <stdio.h>
#include <stdlib.h>

int main(void) {

    void MultiplyArrayElementsByNumber(int *, int, int);

    int *iArray = NULL;
    int num_elemtents;
    int i, num;

    printf("\n\n");
    printf("Enter How Many Elements You Want In The Integer Array :");
    scanf("%d", &num_elemtents);

    iArray = (int *)malloc(num_elemtents * sizeof(int ));

    if(iArray == NULL) {

        printf("Memory Allocation To 'iarray' Has Failed !!! Exiting Now...\n\n");
        exit(0);

    }


    printf("\n\n");
    printf("Enter %d Elements For The Integer Array : \n\n", num_elemtents);;
    for(i = 0; i < num_elemtents;i++) {
        scanf("%d", &iArray[i]);
    }
    
    printf("\n\n");
    
    printf("Array Before Passing To Fuction MultiplyArrayElementsByNumber():\n\n");
    for(i = 0; i < num_elemtents; i++) {
        printf("iArray[%d] = %d\n", i, iArray[i]);
    }

    printf("\n\n");

    printf("Enter The Number By Which You Want To Multiply Each Array Element : ");
    scanf("%d", &num);

    MultiplyArrayElementsByNumber(iArray, num_elemtents, num);

    printf("\n\n");
    printf("Array Returned By Function MultiplyArrayElementsByNumber() : \n\n");

    for(i = 0; i < num_elemtents; i++) {
        printf("iArray[%d] = %d\n", i, iArray[i]);
    }
  
    if(iArray) {
        free(iArray);
        iArray = NULL;
        printf("\n\n");
        printf("Memory Allocated To 'iArray' Has Been Successfully Freed !!!\n");
    }

    return(0);
}

void MultiplyArrayElementsByNumber(int *arr, int iNumElements, int n) {
    int i;

    for(i = 0; i < iNumElements; i++) {
        arr[i] = arr[i] * n;
    }

}
