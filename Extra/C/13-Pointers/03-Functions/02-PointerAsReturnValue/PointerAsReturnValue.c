#include <stdio.h>
#include <stdlib.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    char* ReplaceVowelsWithHashSymbol(char *);

    char string[MAX_STRING_LENGTH];
    char *replaced_string = NULL;

    printf("\n\n");
    printf("Enter String : ");
    gets_s(string, MAX_STRING_LENGTH);

    replaced_string = ReplaceVowelsWithHashSymbol(string);

    if(replaced_string == NULL) {
        printf("ReplaceVowelsWithHashSymbol() Function Has Failed !!! Exiting Now...\n\n");
        exit(0);
    }

    printf("\n\n");
    printf("Replaced String Is : \n\n");
    printf("%s\n\n", replaced_string);

    if (replaced_string) {
        free(replaced_string);
        replaced_string = NULL;
    }

    return(0);
    
}

char* ReplaceVowelsWithHashSymbol(char *s) {

    void MyStrcpy(char *, char *);
    int MyStrlen(char *);

    char *new_string = NULL;
    int i;

    new_string = (char*) malloc(MyStrlen(s) * sizeof(char));

    if(new_string == NULL) {
        printf("Could ot Allocate For New String !!! \\n\n");
        return(NULL);
    }

    MyStrcpy(new_string,s);
    for(i = 0; i < MyStrlen(new_string); i++) {
        switch (new_string[i]) {
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
            new_string[i] = '#';
            break;        
        default:
            break;
        }
    }
    return(new_string);

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

