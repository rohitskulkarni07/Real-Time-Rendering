#include <stdio.h>

int main(void) {

    char chArray[10];
    char *ch_ptr = NULL;
    int i;

    for(i = 0; i < 10 ; i++ ) {

        chArray[i] = (char)(i + 65);

    }

    printf("\n\n");
    printf("Elements Of The Character Array\n\n");

    for(i = 0; i < 10; i++ ) {

        printf("chArray[%d] = %c\n", i , chArray[i]);

    }

    ch_ptr = chArray;

    printf("\n\n");
    printf("Elements Of The Character Array and Respective Addresses Of The Each Element\n\n");

    for(i = 0; i < 10; i++ ) {

        printf("chArray[%d] = %c \t\t Address : %p\n", i , *(ch_ptr + i), (ch_ptr + i));
        
    }
    
    printf("\n\n");

    return (0);

}