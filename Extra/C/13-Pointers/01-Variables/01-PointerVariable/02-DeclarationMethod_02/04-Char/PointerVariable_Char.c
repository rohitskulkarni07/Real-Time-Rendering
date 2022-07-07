#include <stdio.h>

int main(void ) {

    char rsk_ch;
    char* cptr = NULL;

    rsk_ch = 'A';

    printf("\n\n");
    printf("****** BEFORE ptr = &ch ******\n\n");

    printf("VAlues Of 'char'             = %c \n\n",rsk_ch);
    printf("Address Of 'char'            = %p \n\n",&rsk_ch);
    printf("Value At Address Of 'char'   = %c \n\n",*(&rsk_ch));

    cptr = &rsk_ch;

    printf("\n\n");

    printf("****** AFTER ptr = &ch ******\n\n");
    printf("VAlues Of 'char'             = %c \n\n",rsk_ch);
    printf("Address Of 'char'            = %p \n\n",cptr);
    printf("Value At Address Of 'char'   = %c \n\n",*cptr);

    return(0);
}