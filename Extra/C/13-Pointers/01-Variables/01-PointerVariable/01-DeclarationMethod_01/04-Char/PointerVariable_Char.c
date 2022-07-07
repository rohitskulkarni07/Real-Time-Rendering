#include <stdio.h>


int main(void) {

    char rsk_ch;
    char *ptr = NULL;

    rsk_ch = 'A';

    printf("\n\n");

    printf("****** BEFORE ptr = &num ******\n\n");
    printf("Value Of Char\t\t\t=%c\n\n",rsk_ch);
    printf("Address Of Char\t\t\t=%p\n\n",&rsk_ch);
    printf("Value At Address Of Char\t=%c\n\n",*(&rsk_ch));

    ptr = &rsk_ch;

    printf("\n\n");

    printf("****** AFTER ptr = &num ******\n\n");
    printf("Value Of Char\t\t\t=%c\n\n",rsk_ch);
    printf("Address Of Char\t\t\t=%p\n\n",ptr);
    printf("Value At Address Of Char\t=%c\n\n",*ptr);

    return(0);

}

