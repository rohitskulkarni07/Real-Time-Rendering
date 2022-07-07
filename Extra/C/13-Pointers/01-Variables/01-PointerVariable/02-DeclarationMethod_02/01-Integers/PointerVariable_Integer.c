#include <stdio.h>

int main(void ) {

    int rsk_iNum;
    int* ptr = NULL;

    rsk_iNum = 10;

    printf("\n\n");
    printf("****** BEFORE ptr = &num ******\n\n");

    printf("VAlues Of 'num'             = %d \n\n",rsk_iNum);
    printf("Address Of 'num'            = %p \n\n",&rsk_iNum);
    printf("Value At Address Of 'num'   = %d \n\n",*(&rsk_iNum));

    ptr = &rsk_iNum;

    printf("\n\n");

    printf("****** AFTER ptr = &num ******\n\n");
    printf("VAlues Of 'num'             = %d \n\n",rsk_iNum);
    printf("Address Of 'num'            = %p \n\n",ptr);
    printf("Value At Address Of 'num'   = %d \n\n",*ptr);

    return(0);
}