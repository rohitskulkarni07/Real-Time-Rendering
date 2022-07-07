#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    int MyStrlen(char[]);
    void MyStrcpy(char[],char[]);

    char rsk_chArray_Orignal[MAX_STRING_LENGTH];
    char rsk_chArray_Replaced[MAX_STRING_LENGTH];
    int  rsk_iStringLength;
    int  i;

    printf("\n\n");

    printf("ENter String: \n\n");
    gets_s(rsk_chArray_Orignal, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Enter A String By You Is : \n\n");
    printf("%s\n", rsk_chArray_Orignal);

    MyStrcpy(rsk_chArray_Replaced, rsk_chArray_Orignal);
    
    rsk_iStringLength = MyStrlen(rsk_chArray_Replaced);

    for(i = 0; i < rsk_iStringLength; i++) {

        switch (rsk_chArray_Replaced[i]) {
            
            case 'A':
            case 'a':            
            case 'E':
            case 'e':            
            case 'I':
            case 'i':
            case 'O':
            case 'o':
            case 'U':
            case 'u':
                rsk_chArray_Replaced[i] = '*';
                break;
            default:
                break;
        }
    }

    printf("\n\n");
    
    printf("String After Vowels Are Replaced By '*' Is  : \n\n");
    printf("%s\n",rsk_chArray_Replaced);
    
    return(0);

}   

void MyStrcpy(char dest[],char src[]) {

    int j,rsk_iStringLength = 0;

    rsk_iStringLength = MyStrlen(src);

    for(j = 0; j < rsk_iStringLength; j++) {

        dest[j] = src[j]; 
    }
    dest[j] = '\0';

}

int MyStrlen(char str[]) {

    int i = 0,len = 0;

    while(str[i] != '\0') {
        len++;
        i++;
    }

    return len;
}
