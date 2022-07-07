#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    void rsk_MyStrcpy(char[], char[]);

    char rsk_strArray[5][10];

    int rsk_charSize;
    int rsk_strArraySize;
    int rsk_strArrayNumElements, rsk_strArrayNumRows, rsk_strArrayNumCols;
    int rsk_strActualNumChars = 0;

    printf("\n\n");

    rsk_charSize = sizeof(char);
    rsk_strArraySize = sizeof(rsk_strArray);
    rsk_strArrayNumRows = rsk_strArraySize /sizeof(rsk_strArray[0]);
    rsk_strArrayNumCols = sizeof(rsk_strArray[0]) / rsk_charSize;
    rsk_strArrayNumElements = rsk_strArrayNumRows * rsk_strArrayNumCols;


    printf("Size   Of               Character-Type  : %d\n", rsk_charSize);
    printf("Size   Of            2D Character-Array : %d\n", rsk_strArraySize);
    printf("Number Of Rows    in 2D Character-Array : %d\n", rsk_strArrayNumRows);
    printf("Number Of Columns in 2D Character-Array : %d\n", rsk_strArrayNumCols);
    printf("Number Of Elemnts in 2D Character-Array : %d\n", rsk_strArrayNumElements);

    printf("\n\n");

    rsk_MyStrcpy(rsk_strArray[0], "My");
    rsk_MyStrcpy(rsk_strArray[1], "Name");
    rsk_MyStrcpy(rsk_strArray[2], "Is");
    rsk_MyStrcpy(rsk_strArray[3], "Rohit");
    rsk_MyStrcpy(rsk_strArray[4], "Kulkarni");

    printf("\n\n");

    printf("Strings In 2D Array : \n\n");

    for(int i = 0; i < rsk_strArrayNumRows; i++) {

        printf("%s ", rsk_strArray[i]);

    }
    printf("\n\n");

    return(0);

}

void rsk_MyStrcpy(char strDest[], char strSource[]) {

    int rsk_MyStrlen(char[]);
    
    int rsk_iStringLen = 0;
    int i;
    rsk_iStringLen = rsk_MyStrlen(strSource);


    for(i = 0; i < rsk_iStringLen; i++) {
    
        strDest[i] = strSource[i];

    }
    strDest[i] = '\0';
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