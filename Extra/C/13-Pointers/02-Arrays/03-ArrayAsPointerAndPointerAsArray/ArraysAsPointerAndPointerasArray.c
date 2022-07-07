#include <stdio.h>


int main() {

    int iArray[] =   {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    printf("\n\n");
    printf("USING ARRAY NAME AS A POINTER i.e VALUE xth Element of iArray :*(iArray + x)  AND Address Of Xth Element Of iArray : (iArray + x) \n\n");

    printf("iArray[0]  = %d \t At Address : %p\n", *(iArray + 0), (iArray + 0));
    printf("iArray[1]  = %d \t At Address : %p\n", *(iArray + 1), (iArray + 1));
    printf("iArray[2]  = %d \t At Address : %p\n", *(iArray + 2), (iArray + 2));
    printf("iArray[3]  = %d \t At Address : %p\n", *(iArray + 3), (iArray + 3));
    printf("iArray[4]  = %d \t At Address : %p\n", *(iArray + 4), (iArray + 4));
    printf("iArray[5]  = %d \t At Address : %p\n", *(iArray + 5), (iArray + 5));
    printf("iArray[6]  = %d \t At Address : %p\n", *(iArray + 6), (iArray + 6));
    printf("iArray[7]  = %d \t At Address : %p\n", *(iArray + 7), (iArray + 7));
    printf("iArray[8]  = %d \t At Address : %p\n", *(iArray + 8), (iArray + 8));
    printf("iArray[9]  = %d \t At Address : %p\n", *(iArray + 9), (iArray + 9));

    printf("\n\n");
    printf("USING ARRAY NAME AS A ARRAY i.e VALUE xth Element of iArray :iArray[x] AND Address Of Xth Element Of iArray : &iArray[x] \n\n");

    printf("iArray[0]  = %d \t  At Address &iArray[0] : %p\n", iArray[0], &iArray[0]);
    printf("iArray[1]  = %d \t  At Address &iArray[1] : %p\n", iArray[1], &iArray[1]);
    printf("iArray[2]  = %d \t  At Address &iArray[2] : %p\n", iArray[2], &iArray[2]);
    printf("iArray[3]  = %d \t  At Address &iArray[3] : %p\n", iArray[3], &iArray[3]);
    printf("iArray[4]  = %d \t  At Address &iArray[4] : %p\n", iArray[4], &iArray[4]);
    printf("iArray[5]  = %d \t  At Address &iArray[5] : %p\n", iArray[5], &iArray[5]);
    printf("iArray[6]  = %d \t  At Address &iArray[6] : %p\n", iArray[6], &iArray[6]);
    printf("iArray[7]  = %d \t  At Address &iArray[7] : %p\n", iArray[7], &iArray[7]);
    printf("iArray[8]  = %d \t  At Address &iArray[8] : %p\n", iArray[8], &iArray[8]);
    printf("iArray[9]  = %d  \t At Address &iArray[9] : %p\n", iArray[9], &iArray[9]);

    printf("\n\n");
    return(0);
}