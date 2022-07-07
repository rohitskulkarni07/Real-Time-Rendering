#include <stdio.h>


int main(void) {

    double rsk_lfnum;
    double *ptr = NULL;

    rsk_lfnum = 93.101112221;
    printf("\n\n");

    printf("****** BEFORE ptr = &num ******\n\n");
    printf("Value Of Num\t\t\t=%lf\n\n",rsk_lfnum);
    printf("Address Of Num\t\t\t=%p\n\n",&rsk_lfnum);
    printf("Value At Address Of Num\t\t=%lf\n\n",*(&rsk_lfnum));

    ptr = &rsk_lfnum;

    printf("\n\n");

    printf("****** AFTER ptr = &num ******\n\n");
    printf("Value Of Num\t\t\t=%f\n\n",rsk_lfnum);
    printf("Address Of Num\t\t\t=%p\n\n",ptr);
    printf("Value At Address Of Num\t\t=%f\n\n",*ptr);

    return(0);

}

