#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    char rsk_chArray[MAX_STRING_LENGTH];


    printf("\n\n");

    printf("Enter String : \n\n");
    gets_s(rsk_chArray, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Orignally String Entered By You Is : \n\n");
    printf("%s\n",rsk_chArray);

    printf("\n\n");

    printf("The Reversed String (i.e 'chArray_Copy[]') Is : \n\n");
    printf("%s\n",strrev(rsk_chArray));

    printf("\n\n");

    return(0);

}
