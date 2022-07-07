#include <stdio.h>


int main(void) {
    int num;
    int *ptr = NULL;

    num = 10;
    printf("\n\n");

    printf("****** BEFORE ptr = &num ******\n\n");
    printf("Value Of Num\t\t\t=%d\n\n",num);
    printf("Address Of Num\t\t\t=%p\n\n",&num);
    printf("Value At Address Of Num\t\t=%d\n\n",*(&num));

    ptr = &num;

    printf("\n\n");

    printf("****** AFTER ptr = &num ******\n\n");
    printf("Value Of Num\t\t\t=%d\n\n",num);
    printf("Address Of Num\t\t\t=%p\n\n",ptr);
    printf("Value At Address Of Num\t\t=%d\n\n",*ptr);

    return(0);

}

