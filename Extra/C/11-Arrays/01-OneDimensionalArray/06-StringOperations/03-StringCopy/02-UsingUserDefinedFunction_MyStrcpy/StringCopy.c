#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    void MyStrcpy(char[],char[]);

    char rsk_chArray_Orignal[MAX_STRING_LENGTH];
    char rsk_chArray_Copy[MAX_STRING_LENGTH];


    printf("\n\n");

    printf("Enter String : \n\n");
    gets_s(rsk_chArray_Orignal, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Orignally String Entered By You Is : \n\n");
    printf("%s\n",rsk_chArray_Orignal);

    printf("\n\n");

    MyStrcpy(rsk_chArray_Copy, rsk_chArray_Orignal);

    printf("The Copied String (i.e 'chArray_Copy[]') Is : \n\n");
    printf("%s\n",rsk_chArray_Copy);

    printf("\n\n");

    return(0);

}

void MyStrcpy(char dest[],char src[]) {

    int MyStrlen(char[]);

    int j,rsk_iStringLength = 0;

    rsk_iStringLength = MyStrlen(src);

    for(j = 0; j < rsk_iStringLength; j++) {

        dest[j] = src[j]; 
    }
    dest[j] = '\0';

}

int MyStrlen(char str[]) {

    int i=0,len=0;

    while(str[i] != '\0') {
        i++;
        len++;   
    }
    
    return len;

}
