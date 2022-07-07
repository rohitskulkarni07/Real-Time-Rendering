#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    int MyStrlen(char *);

    char *chArray = NULL;
    int iStringLength = 0;

    chArray = (char*)malloc(MAX_STRING_LENGTH * sizeof(char));

    printf("\n\n");
    if(chArray == NULL) {

        printf("Memory Location To Character Array Failed !!! Exiting Now...\n\n");
        exit(0);
    }

    printf("Enter A String\n\n");
    gets_s(chArray, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("Your String Is : %s\n",chArray);

    printf("\n\n");
    iStringLength = MyStrlen(chArray);

    printf("Length Of String Is = %d Characters !!!\n\n", iStringLength);

    if(chArray) {

        free(chArray);
        chArray = NULL;

    }

    return(0);

}

int MyStrlen(char *str) {

    int j = 0;
    int string_len = 0;

    while(*(str + j) != '\0') {
        string_len++;
        j++;
    }
    return string_len;
}