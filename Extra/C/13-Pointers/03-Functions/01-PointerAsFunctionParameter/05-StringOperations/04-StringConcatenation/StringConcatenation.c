#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    void MyStrcat(char*, char*);
    int MyStrlen(char *);

    char *chArray_One = NULL, *chArray_Two = NULL;

    chArray_One = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));

    printf("\n\n");

    if(chArray_One == NULL) {
        printf("Memory Alocation For Orignal String Failed !!! Exiting Now...\n\n");
        exit(0);
    }

    printf("Enter A String : \n\n");
    gets_s(chArray_One, MAX_STRING_LENGTH);

    chArray_Two = (char*) malloc(MAX_STRING_LENGTH * sizeof(char));

    printf("\n\n");

    if(chArray_Two == NULL) {
        printf("Memory Alocation For Orignal String Failed !!! Exiting Now...\n\n");
        exit(0);
    }

    printf("Enter Another String : \n\n");
    gets_s(chArray_Two, MAX_STRING_LENGTH);

    printf("\n\n");
    printf("*********** BEFORE CONCATENATION ***********");
    printf("\n\n");
    
    printf("THe Orignal First String Entered By You ('i.e : chArray_One[]') Is :\n\n");
    
    printf("%s\n",chArray_One);
    printf("\n\n");
    
    printf("THe Orignal Second String Entered By You ('i.e : chArray_Two[]') Is :\n\n");
    printf("%s\n",chArray_Two);

    MyStrcat(chArray_One, chArray_Two);

    printf("\n\n");
    printf("*********** AFTER CONCATENATION ***********");
    printf("\n\n");
    
    printf("chArray_One[] :\n\n");
    
    printf("%s\n",chArray_One);
    printf("\n\n");
    
    printf("chArray_Two[] :\n\n");
    printf("%s\n",chArray_Two);

    if(chArray_Two) {
        
        free(chArray_Two);
        chArray_Two = NULL;
        printf("\n\n");
        printf("Memory Allocated To Second String Has Been Successfully Freed !!!\n\n");

    }

    if(chArray_One) {
        
        free(chArray_One);
        chArray_One = NULL;
        printf("\n\n");
        printf("Memory Allocated To First String Has Been Successfully Freed !!!\n\n");

    }


    return(0);
}

void MyStrcat(char* str_dest, char* str_src) {

    int MyStrlen(char *);

    int iStringLength_Source = 0, iStringLength_Destination = 0;

    int i, j;

    iStringLength_Source = MyStrlen(str_src); 
    iStringLength_Destination = MyStrlen(str_dest);

    for(i = iStringLength_Destination, j = 0; j < iStringLength_Source; i++, j++) {

        *(str_dest + i) = *(str_src + j);
    }

    *(str_dest + i) = '\0';
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
