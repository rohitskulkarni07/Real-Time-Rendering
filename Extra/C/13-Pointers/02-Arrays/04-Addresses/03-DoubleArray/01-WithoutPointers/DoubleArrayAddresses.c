#include <stdio.h>

int main(void) {

    double lfArray[10];
    int i;

    for(i = 0; i < 10 ; i++ ) {

        lfArray[i] = (float)(i + 1) *1.123967;

    }

    printf("\n\n");
    printf("Elements Of The Double Array\n\n");

    for(i = 0; i < 10; i++ ) {

        printf("lfArray[%d] = %lf\n", i , lfArray[i]);

    }

    printf("\n\n");
    printf("Elements Of The Double Array and Respective Addresses Of The Each Element\n\n");

    for(i = 0; i < 10; i++ ) {

        printf("lfArray[%d] = %lf \t\t Address : %p\n", i , lfArray[i], &lfArray[i]);
        
    }
    
    printf("\n\n");

    return (0);

}