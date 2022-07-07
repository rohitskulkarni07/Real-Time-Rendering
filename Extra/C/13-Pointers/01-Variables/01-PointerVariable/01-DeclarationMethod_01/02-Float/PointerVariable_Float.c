#include <stdio.h>


int main(void) {
    float rsk_num;
    float *ptr = NULL;

    rsk_num = 33.1011;
    printf("\n\n");

    printf("****** BEFORE ptr = &num ******\n\n");
    printf("Value Of Num\t\t\t=%f\n\n",rsk_num);
    printf("Address Of Num\t\t\t=%p\n\n",&rsk_num);
    printf("Value At Address Of Num\t\t=%f\n\n",*(&rsk_num));

    ptr = &rsk_num;

    printf("\n\n");

    printf("****** AFTER ptr = &num ******\n\n");
    printf("Value Of Num\t\t\t=%f\n\n",rsk_num);
    printf("Address Of Num\t\t\t=%p\n\n",ptr);
    printf("Value At Address Of Num\t\t=%f\n\n",*ptr);

    return(0);

}

