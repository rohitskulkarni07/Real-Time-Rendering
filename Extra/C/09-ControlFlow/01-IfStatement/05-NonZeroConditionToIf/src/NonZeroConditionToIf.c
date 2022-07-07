#include <stdio.h>

int main(void) {

    int rsk_a;
    
    printf("\n\n");

    rsk_a = 5;
    if(rsk_a) {
        printf(" if-block 1 : 'A' Exists And Value = %d  !!! \n\n ",rsk_a);
    }

    rsk_a = -5;
    if(rsk_a) {
        printf("if-block 2 : 'A' Exists And Value = %d  !!! \n\n ",rsk_a);

    }

    rsk_a = 0;
    if(rsk_a) {
        printf("if-block 3 : 'A' Exists And Value = %d  !!! \n\n ",rsk_a);

    }

    printf("All Three If-Statements Are Done\n\n");

    return (0);
}