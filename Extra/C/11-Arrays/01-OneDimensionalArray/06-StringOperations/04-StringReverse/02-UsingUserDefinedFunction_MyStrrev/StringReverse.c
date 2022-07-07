#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    void MyStrrev(char[], char[]);

    char rsk_chArray_Orignal[MAX_STRING_LENGTH];
    char rsk_chArray_Reversed[MAX_STRING_LENGTH];

    printf("\n\n");

    printf("Enter String : \n\n");
    gets_s(rsk_chArray_Orignal, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Orignally String Entered By You Is : \n\n");
    printf("%s\n",rsk_chArray_Orignal);

    printf("\n\n");

    MyStrrev(rsk_chArray_Orignal,rsk_chArray_Reversed);

    printf("The Reversed String (i.e 'chArray_Reversed[]') Is : \n\n");
    printf("%s\n",rsk_chArray_Reversed);

    printf("\n\n");

    return(0);

}

void MyStrrev(char rsk_chArray_Orignal[], char rsk_chArray_Reversed[]) {
    
    int MyStrlen(char[]);
    int i,j,iStringLen=0,len=0;
    iStringLen = MyStrlen(rsk_chArray_Orignal);
    len = iStringLen - 1;

    for(i=0 , j = len ; i < iStringLen, j>=0; i++,j--) {
        rsk_chArray_Reversed[i] = rsk_chArray_Orignal[j];
    }
    rsk_chArray_Reversed[i] = '\0';

}

int MyStrlen(char str[]) {

    int i=0,len=0;

    while(str[i] != '\0') {
        i++;
        len++;   
    }
    
    return len;

}