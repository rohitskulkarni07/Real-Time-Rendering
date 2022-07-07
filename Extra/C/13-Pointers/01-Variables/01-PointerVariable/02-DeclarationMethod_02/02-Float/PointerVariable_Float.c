#include <stdio.h>

int main(void ) {

    float rsk_fNum;
    float* fptr = NULL;

    rsk_fNum = 7.1997f;

    printf("\n\n");
    printf("****** BEFORE ptr = &num ******\n\n");

    printf("VAlues Of 'num'             = %f \n\n",rsk_fNum);
    printf("Address Of 'num'            = %p \n\n",&rsk_fNum);
    printf("Value At Address Of 'num'   = %f \n\n",*(&rsk_fNum));

    fptr = &rsk_fNum;

    printf("\n\n");

    printf("****** AFTER ptr = &num ******\n\n");
    printf("VAlues Of 'num'             = %f \n\n",rsk_fNum);
    printf("Address Of 'num'            = %p \n\n",fptr);
    printf("Value At Address Of 'num'   = %f \n\n",*fptr);

    return(0);
}