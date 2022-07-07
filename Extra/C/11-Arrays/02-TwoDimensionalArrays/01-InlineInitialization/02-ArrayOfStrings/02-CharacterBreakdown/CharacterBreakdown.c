#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    int rsk_MyStrlen(char[]);

    char rsk_strArray[10][15] = {"Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2020-21)", "Of", "ASTROMEDICOMP."};

    int rsk_iStrLengths[10];

    int rsk_strArraySize;
    int rsk_strArrayNumElements, rsk_strArrayNumRows;
    int i,j; //iterator

    rsk_strArraySize = sizeof(rsk_strArray);
    rsk_strArrayNumRows = rsk_strArraySize /sizeof(rsk_strArray[0]);


    for(i=0;i<rsk_strArrayNumRows; i++) {

        rsk_iStrLengths[i] = rsk_MyStrlen(rsk_strArray[i]);

    }

    printf("\n\n");
    printf("The Eniter String Array : \n\n ");

    for(i=0; i<rsk_strArrayNumRows;i++) {
        printf("%s ", rsk_strArray[i]);

    }

    printf("\n\n");
    printf("String In The 2D Array : \n\n");

    for(i=0; i<rsk_strArrayNumRows;i++) {

        printf("Strings Number %d => %s\n\n",(i+1),rsk_strArray[i]);

        for(j=0; j<rsk_iStrLengths[i];j++) {
            
            printf("Charachter %d = %c\n",(j+1),rsk_strArray[i][j]);

        }
        printf("\n\n");
    }

    return(0);

}

int rsk_MyStrlen(char str[]) {

    int j;
    int rsk_string_length = 0;

    for(j=0; j < MAX_STRING_LENGTH; j++) {
            if(str[j] == '\0') {
                break;
            }
            else {
                rsk_string_length++;

            }

    }

    return(rsk_string_length);
}