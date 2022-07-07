#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    char rsk_chArray_Orignal[MAX_STRING_LENGTH];
    char rsk_chArray_Copy[MAX_STRING_LENGTH];


    printf("\n\n");

    printf("Enter String : \n\n");
    gets_s(rsk_chArray_Orignal, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Orignally String Entered By You Is : \n\n");
    printf("%s\n",rsk_chArray_Orignal);

    printf("\n\n");

    strcpy(rsk_chArray_Copy, rsk_chArray_Orignal);

    printf("The Copied String (i.e 'chArray_Copy[]') Is : \n\n");
    printf("%s\n",rsk_chArray_Copy);

    printf("\n\n");

    return(0);

}
