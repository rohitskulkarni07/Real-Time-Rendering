#include <stdio.h>

int main(void ) {

    double rsk_lfNum;
    double* lfptr = NULL;

    rsk_lfNum = 7.199709f;

    printf("\n\n");
    printf("****** BEFORE ptr = &num ******\n\n");

    printf("VAlues Of 'num'             = %f \n\n",rsk_lfNum);
    printf("Address Of 'num'            = %p \n\n",&rsk_lfNum);
    printf("Value At Address Of 'num'   = %f \n\n",*(&rsk_lfNum));

    lfptr = &rsk_lfNum;

    printf("\n\n");

    printf("****** AFTER ptr = &num ******\n\n");
    printf("VAlues Of 'num'             = %f \n\n",rsk_lfNum);
    printf("Address Of 'num'            = %p \n\n",lfptr);
    printf("Value At Address Of 'num'   = %f \n\n",*lfptr);

    return(0);
}