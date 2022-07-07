#include <stdio.h>

int main() {

    int rsk_num;
    int *ptr = NULL;
    int **pptr = NULL;

    rsk_num = 10;

    printf("\n\n");

    printf(" ****** BEFORE ptr = &num ******\n\n");
    printf("Value Of 'Num' = %d\n\n", rsk_num);
    printf("Address Of 'Num' = %p\n\n", &rsk_num);
    printf("Value Of 'Num' = %d\n\n", *(&rsk_num));

    printf("\n\n");

    ptr = &rsk_num;
    
    printf(" ****** AFTER ptr = &num ******\n\n");
    printf("Value Of 'Num' = %d\n\n", rsk_num);
    printf("Address Of 'Num' = %p\n\n", ptr);
    printf("Value At Address Of 'Num' = %d\n\n", *ptr);

    printf("\n\n");

    pptr = &ptr;

    printf(" ****** AFTER pptr = &ptr ******\n\n");
    printf("Value Of 'Num' = %d\n\n", rsk_num);
    printf("Address Of 'Num' = %p\n\n", ptr);
    printf("Address Of 'ptr' = %p\n\n", pptr);
    printf("Value At Address Of 'ptr' = %p\n\n", *pptr);
    printf("Value At Address Of 'Num' = %d\n\n", **pptr);
    
    return(0);

}
