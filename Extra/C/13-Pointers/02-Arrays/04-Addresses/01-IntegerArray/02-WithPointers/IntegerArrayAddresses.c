#include <stdio.h>

int main(void) {

    int rsk_iArray[10];
    int *ptr_iArray = NULL;
    int i;

    for(i = 0; i < 10; i++) {
        rsk_iArray[i] = (i+1) * 3;

    }

    printf("\n\n");
    printf("Elements of THe Integer Array\n");
    for(i = 0; i < 10; i++) {
        printf("iArray[%d] = %d\n", i, *(rsk_iArray + i));

    }

    printf("\n\n");
    printf("Elements And Their Respect Address Of THe Integer Array\n");

    ptr_iArray = rsk_iArray;    //  same as ptr_iArray = &iArray[0]
    for(i = 0; i < 10; i++) {
        printf("iArray[%d] = %d\t\t Address = %p\n", i, *(ptr_iArray + i), (ptr_iArray + i));

    }

    printf("\n\n");

    return(0);
}