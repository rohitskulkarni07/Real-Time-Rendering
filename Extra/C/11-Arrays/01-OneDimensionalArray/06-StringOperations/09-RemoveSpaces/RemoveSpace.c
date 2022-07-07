#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    int MyStrlen(char[]);

    char rsk_chArray[MAX_STRING_LENGTH], rsk_chArraySpaceRemoved[MAX_STRING_LENGTH];
    int  rsk_iStringLength;
    int  i, j, rsk_iSpaceCount = 0;

    printf("\n\n");

    printf("ENter String: \n\n");
    gets_s(rsk_chArray, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Entered String By You Is : \n\n");
    printf("%s\n", rsk_chArray);
    
    rsk_iStringLength = MyStrlen(rsk_chArray);

    j = 0;
    for(i = 0; i < rsk_iStringLength; i++) {

        if(rsk_chArray[i] == ' ') {
            continue;
        } else {
            rsk_chArraySpaceRemoved[j] = rsk_chArray[i];
            j++;
        }
    }

    rsk_chArraySpaceRemoved[j] = '\0';


    printf("\n\n");
    printf("String After Removal Of Spcae Is = %s\n\n", rsk_chArraySpaceRemoved);
    printf("\n\n");

    return(0);

}

int MyStrlen(char str[]) {

    int i = 0,len = 0;

    while(str[i] != '\0') {
        len++;
        i++;
    }

    return len;
}
