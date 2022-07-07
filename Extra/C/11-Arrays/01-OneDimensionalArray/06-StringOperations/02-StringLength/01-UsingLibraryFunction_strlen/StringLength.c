#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    char rsk_chArray[MAX_STRING_LENGTH];
    int  rsk_iStringLength = 0;

    printf("\n\n");
    printf("Enter String : \n\n");
    gets_s(rsk_chArray, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("String Entered By You Is : \n\n");
    printf("%s\n",rsk_chArray);

    printf("\n\n");
    rsk_iStringLength = strlen(rsk_chArray);
    printf("Length Of String Is = %d Characters !!!\n\n", rsk_iStringLength);

    return(0);

}
