#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    int rsk_MyStrlen(char[]);

    char rsk_strArray[10][15] = {"Hello!", "Welcome", "To", "Real", "Time", "Rendering", "Batch", "(2020-21)", "Of", "ASTROMEDICOMP."};

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

    for(int i = 0; i < rsk_strArrayNumElements; i++) {
        rsk_strActualNumChars = rsk_strActualNumChars + rsk_MyStrlen(rsk_strArray[i]);
    }
    printf("Actual Number Of Characters in 2D Character-Array Is  : %d\n\n",rsk_strActualNumChars);

    printf("\n\n");

    printf("Strings In 2D Array : \n\n");

    printf("%s ", rsk_strArray[0]);
    printf("%s ", rsk_strArray[1]);
    printf("%s ", rsk_strArray[2]);
    printf("%s ", rsk_strArray[3]);
    printf("%s ", rsk_strArray[4]);
    printf("%s ", rsk_strArray[5]);
    printf("%s ", rsk_strArray[6]);
    printf("%s ", rsk_strArray[7]);
    printf("%s ", rsk_strArray[8]);
    printf("%s ", rsk_strArray[9]);

    printf("\n\n");


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