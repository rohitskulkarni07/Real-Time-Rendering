#include <stdio.h>

int main(void) {

    char rsk_chArray_01[]  = {'A','S','T','R','O','M','E','D','I','C','O','M','P','\0'};

    char rsk_chArray_02[9] = {'W','E','L','C','O','M','E','S','\0'}; 

    char rsk_chArray_03[]  = {'Y','O','U', '\0'}; 

    char rsk_chArray_04[]  = "To";
    
    char rsk_chArray_05[]  = "REAL TIME RENDERING BATCH OF 2020-21";

    char rsk_chArray_WithoutNullTerminator[]  = {'H','E','L','L','O'};
    
    printf("\n\n");

    printf("Size of chArray_01 : %lu\n\n", sizeof(rsk_chArray_01));
    printf("Size of chArray_02 : %lu\n\n", sizeof(rsk_chArray_02));
    printf("Size of chArray_03 : %lu\n\n", sizeof(rsk_chArray_03));
    printf("Size of chArray_04 : %lu\n\n", sizeof(rsk_chArray_04));
    printf("Size of chArray_05 : %lu\n\n", sizeof(rsk_chArray_05));

    printf("\n\n");

    printf("The Strings Are : \n\n");
    printf("chArray_01 : %s\n\n", rsk_chArray_01);
    printf("chArray_02 : %s\n\n", rsk_chArray_02);
    printf("chArray_03 : %s\n\n", rsk_chArray_03);
    printf("chArray_04 : %s\n\n", rsk_chArray_04);
    printf("chArray_05 : %s\n\n", rsk_chArray_05);

    printf("\n\n");
    printf("Size of chArray_WithoutNullTerminator : %lu\n\n",sizeof(rsk_chArray_WithoutNullTerminator));

    printf("chArray_WithoutNullTerminator : %s\n\n", rsk_chArray_WithoutNullTerminator);

    return(0);
}