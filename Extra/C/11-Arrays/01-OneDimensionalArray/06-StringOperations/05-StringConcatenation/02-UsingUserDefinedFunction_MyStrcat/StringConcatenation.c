#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main() {

    void MyStrcat(char[],char[]);

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

    MyStrcat(rsk_chArray_One,rsk_chArray_Two);

    printf("***** AFTER CONCATENATION *****\n\n");

    printf("\n\n");

    printf("String 1: %s\n",rsk_chArray_One);

    printf("\n\n");

    printf("String 2: %s\n",rsk_chArray_Two);

    printf("\n\n");

    return(0);
}

void MyStrcat(char dest[],char src[]) {
    
    int MyStrlen(char[]);

    int rsk_iStringLen_Src = 0;
    int rsk_iStringLen_Dest = 0;
    int i,j;
    

    rsk_iStringLen_Src = MyStrlen(src);
    rsk_iStringLen_Dest = MyStrlen(dest);

    for(i = rsk_iStringLen_Dest,j = 0; j < rsk_iStringLen_Src;i++,j++) {
        dest[i] = src[j];
    }
    dest[i] = '\0';

}

int MyStrlen(char str[]) {

    int i=0,len=0;

    while(str[i] != '\0') {
        i++;
        len++;   
    }
    
    return len;

}

