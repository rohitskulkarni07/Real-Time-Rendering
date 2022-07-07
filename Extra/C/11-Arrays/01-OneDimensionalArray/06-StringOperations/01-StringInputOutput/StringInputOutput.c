#include <stdio.h>
#include <string.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    char rsk_chArray[MAX_STRING_LENGTH];

    printf("\n\n");
    printf("ENter A String :\n\n");

    // function char *__cdecl gets_s(char *_Buffer, rsize_t _Size)
    gets_s(rsk_chArray,MAX_STRING_LENGTH);

    printf("\n\n");

    printf("String You Entered Is : \n\n");
    printf("%s\n", rsk_chArray);
    
    return(0);

}