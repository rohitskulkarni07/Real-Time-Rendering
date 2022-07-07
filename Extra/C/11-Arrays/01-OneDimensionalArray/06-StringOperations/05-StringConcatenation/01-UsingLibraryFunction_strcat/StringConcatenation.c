#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main() {

    char rsk_chArray_One[MAX_STRING_LENGTH];
    char rsk_chArray_Two[MAX_STRING_LENGTH];

    printf("Enter First String : \n\n");
    gets_s(rsk_chArray_One,MAX_STRING_LENGTH);
    
    printf("Enter Two String : \n\n");
    gets_s(rsk_chArray_Two,MAX_STRING_LENGTH);
    
    printf("\n\n");

    printf("***** BEFORE CONCATENATION *****\n\n");

    printf("\n\n");

    printf("String 1: %s\n",rsk_chArray_One);
    
    printf("\n\n");

    printf("String 2: %s\n",rsk_chArray_Two);

    printf("\n\n");

    strcat(rsk_chArray_One,rsk_chArray_Two);

    printf("***** AFTER CONCATENATION *****\n\n");

    printf("\n\n");

    printf("String 1: %s\n",rsk_chArray_One);

    printf("\n\n");

    printf("String 2: %s\n",rsk_chArray_Two);

    printf("\n\n");

    return(0);

}