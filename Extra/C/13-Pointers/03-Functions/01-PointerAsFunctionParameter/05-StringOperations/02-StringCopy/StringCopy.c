#include <stdio.h>
#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    void MyStrcpy(char *, char *);
    int MyStrlen(char *);

    char *chArray_Orignal = NULL, *chArray_Copy = NULL;

    int Orignal_string_length;

    printf("\n\n");
    chArray_Orignal = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));

    if(chArray_Orignal == NULL) {

        printf("Memory Allocation for Orignal String Failed !!! Exiting Now...\n\n");
        exit(0);
    }

    printf("Enter A String: \n\n");
    gets_s(chArray_Orignal, MAX_STRING_LENGTH);

    Orignal_string_length = MyStrlen(chArray_Orignal);

    chArray_Copy = (char *) malloc (Orignal_string_length * sizeof(char));
    if(chArray_Copy == NULL) {
        printf("Memory Allocation For Copied String Failed !!! Exiting Now..\n\n");
        exit(0);

    }

    MyStrcpy(chArray_Copy, chArray_Orignal);

    printf("\n\n");
    printf("The Orignal String Entered By You (i.e : 'chArray_Orignal') Is :\n\n");
    printf("%s\n", chArray_Orignal);
    printf("\n\n");
    printf("The Copied String (i.e 'chArray_Copy') is  : \n\n");
    printf("%s\n", chArray_Copy);

    if(chArray_Copy) {

        free(chArray_Copy);
        chArray_Copy = NULL;
        printf("\n\n");
        printf("Memory Allocated ForCopied String Been Successfully Freed");

    }

    if(chArray_Orignal) {
        free(chArray_Orignal);
        chArray_Orignal = NULL;
        printf("\n\n");
        printf("Memory Allocated For Orignal String Has Been Suucessfully Freed !!! \n\n");
    }

    return(0);
}

void MyStrcpy(char *str_dest, char *str_src) {
    int MyStrlen(char *);

    int iStringLength = 0;
    int j;

    iStringLength = MyStrlen(str_src);

    for(j = 0; j < MAX_STRING_LENGTH; j++) {
        *(str_dest + j) = *(str_src + j);
    }
    *(str_dest + j) = '\0';

}

int MyStrlen(char *str) {
    
    int j;
    int string_length = 0;

    for(j = 0; j < MAX_STRING_LENGTH; j++) {

        if(str[j] == '\0') {
            break;
        } else {
            string_length++;
        }
    }
}

