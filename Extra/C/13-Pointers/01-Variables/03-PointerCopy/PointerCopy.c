#include <stdio.h>

int main(void) {
    int rsk_iNum;
    int *iptr = NULL;
    int *copy_iptr = NULL;

    rsk_iNum = 5;
    iptr = &rsk_iNum;

    printf("\n\n");
    printf(" ****** BEFORE copy_ptr = ptr\n\n");
    printf(" num     = %d\n", rsk_iNum);
    printf(" &num    = %p\n", &rsk_iNum);
    printf(" *(&num) = %d\n", *(&rsk_iNum));
    printf(" ptr     = %p\n", iptr);
    printf(" *ptr    = %d\n", *iptr);

    copy_iptr = iptr;

    printf("\n\n");
    printf(" ****** AFTER copy_ptr = ptr\n\n");
    printf(" num       = %d\n", rsk_iNum);
    printf(" &num      = %p\n", &rsk_iNum);
    printf(" *(&num)   = %d\n", *(&rsk_iNum));
    printf(" ptr       = %p\n", iptr);
    printf(" *ptr      = %d\n", *iptr);
    printf(" copy_ptr  = %p\n", copy_iptr);
    printf(" *copy_ptr = %d\n", *copy_iptr);

    return(0);
}
