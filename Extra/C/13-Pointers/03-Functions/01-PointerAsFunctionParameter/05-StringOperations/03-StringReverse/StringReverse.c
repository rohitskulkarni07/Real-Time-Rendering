#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    void MyStrrev(char*, char*);
    int MyStrlen(char *);

    char *chArray_Orignal = NULL, *chArray_Reserved = NULL;
    
    int orignal_string_length;

    printf("\n\n");
    chArray_Orignal = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));

    if(chArray_Orignal == NULL) {
        printf("Memory Alocation For Orignal String Failed !!! Exiting Now...\n\n");
        exit(0);
    }

    printf("Enter A String : \n\n");
    gets_s(chArray_Orignal, MAX_STRING_LENGTH);

    orignal_string_length = MyStrlen(chArray_Orignal);
    chArray_Reserved = (char *)malloc(orignal_string_length * sizeof(char));

    if(chArray_Reserved == NULL) {
        printf("Memory Allocation For reserved String Failed !!! Exiting Now...\n\n");
        exit(0);
    }

    MyStrrev(chArray_Reserved, chArray_Orignal);

    printf("\n\n");
    printf("The Orignal String Entered By You (i.e : 'chArray_Orignal[]') Is : \n\n");
    printf("%s\n", chArray_Orignal);

    printf("\n\n");
    printf("The Resereved String (i.e : 'chArray_Reserved[]') Is: \n\n");
    printf("%s\n", chArray_Reserved);

    if(chArray_Reserved) {
        free(chArray_Reserved);
        chArray_Reserved = NULL;
        printf("\n\n");
        printf("Memory Allocated To Reserved String Has Been Successfully Freed !!! \n\n");
    }

    if(chArray_Orignal) {

        free(chArray_Orignal);
        chArray_Orignal = NULL;
        printf("\n\n");
        printf("Memory Allocated To Orignal String Has Been Successfully Freed !!! \n\n");
    }


    return(0);
}

void MyStrrev(char *str_dest, char *str_src) {

    int MyStrlen(char *);

    int iStringLength = 0;
    int i, j, len;

    iStringLength = MyStrlen(str_src);

    len = iStringLength -1;

    for(i = 0, j = len; i < iStringLength, j>=0 ; i++, j--) {

        *(str_dest + i) = *(str_src + j);
    }
    *(str_dest + i ) = '\0';

}

int MyStrlen(char *str) {

    int j;
    int string_length = 0;

    for(j = 0; j < MAX_STRING_LENGTH; j++) {

        if(str[j] == '\0') {
            break;

        } else {
            string_length ++;
        }
    }
    return(string_length);  
}
