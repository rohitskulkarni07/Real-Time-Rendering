#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    int MyStrlen(char[]);

    char rsk_chArray[MAX_STRING_LENGTH];
    int  rsk_iStringLength;
    int  i, rsk_iSpaceCount = 0;

    printf("\n\n");

    printf("ENter String: \n\n");
    gets_s(rsk_chArray, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Entered String By You Is : \n\n");
    printf("%s\n", rsk_chArray);
    
    rsk_iStringLength = MyStrlen(rsk_chArray);

    for(i = 0; i < rsk_iStringLength; i++) {

        switch (rsk_chArray[i]) {
            
            case 32:
                rsk_iSpaceCount ++;
                break;
            default:
                break;
        }
    }

    int rsk_iWordCount = rsk_iSpaceCount + 1;

    printf("\n\n");
    printf("Number Of Spaces in Strng = %d\n\n", rsk_iSpaceCount);
    printf("Number Of Words in Strng = %d\n\n", rsk_iWordCount);
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
