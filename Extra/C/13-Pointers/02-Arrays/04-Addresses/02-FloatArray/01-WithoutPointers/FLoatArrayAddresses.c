#include <stdio.h>

int main(void) {

    float rsk_fArray[10];
    int i;

    for(i = 0; i < 10; i++) {
        rsk_fArray[i] = (float)(i+1.2) * 1.5;

    }

    printf("\n\n");
    printf("Elements of The Float Array\n");
    for(i = 0; i < 10; i++) {
        printf("fArray[%d] = %f\n", i, rsk_fArray[i]);

    }

    printf("\n\n");
    printf("Elements And Their Respect Address Of The Float Array\n");

    for(i = 0; i < 10; i++) {
        printf("fArray[%d] = %f\t\t Address = %p\n", i, rsk_fArray[i], &rsk_fArray[i]);

    }

    printf("\n\n");

    return(0);
}