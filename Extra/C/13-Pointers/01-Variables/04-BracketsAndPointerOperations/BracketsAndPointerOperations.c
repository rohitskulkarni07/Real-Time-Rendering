#include <stdio.h>

int main(void) {
    int rsk_iNum;
    int *iptr = NULL;
    int ans;

    rsk_iNum = 5;
    iptr = &rsk_iNum;

    printf("\n\n");
    printf(" ****** BEFORE copy_ptr = ptr\n\n");
    printf(" num     = %d\n", rsk_iNum);
    printf(" &num    = %p\n", &rsk_iNum);
    printf(" *(&num) = %d\n", *(&rsk_iNum));
    printf(" ptr     = %p\n", iptr);
    printf(" *ptr    = %d\n", *iptr);
    printf("\n\n");

    printf("Answer of (ptr + 10) = %p\n", (iptr + 10)); // 1address of (10 * (sizeof type) + address)

    printf("Answer of *(ptr + 10) = %d\n", *(iptr + 10)); //Value at this address (10 * (sizeof type) + address)

    printf("Answer of (*ptr + 10) = %d\n", (*iptr + 10));  // add 10 to the value, pointed by *iptr

    ++*iptr; //preincreament value by one (value which is pointed by *iptr)

    printf("Answer of ++*ptr = %d\n", *iptr);

    *iptr++;    // Incorrect method of post increament value by one (value which is pointed by *iptr)
                // assosiativity  right to left post increament address ptr (ptr++) then value at that address is displayed
    
    printf("Answer of *ptr++ = %d\n", *iptr);

    iptr = &rsk_iNum;

    (*iptr)++; // correct way of post increamnting value at the address *iptr 

    printf("Answer of (*ptr)++ %d\n\n", *iptr);

    return(0);
}
