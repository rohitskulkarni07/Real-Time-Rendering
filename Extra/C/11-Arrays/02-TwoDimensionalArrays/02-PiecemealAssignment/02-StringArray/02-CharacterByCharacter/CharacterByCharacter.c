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

    rsk_strArray[0][0] = 'M';
    rsk_strArray[0][1] = 'y';
    rsk_strArray[0][2] = '\0';

    rsk_strArray[1][0] = 'N';
    rsk_strArray[1][1] = 'a';
    rsk_strArray[1][2] = 'm';
    rsk_strArray[1][3] = 'e';
    rsk_strArray[1][4] = '\0';

    rsk_strArray[2][0] = 'I';
    rsk_strArray[2][1] = 's';
    rsk_strArray[2][3] = '\0';

    rsk_strArray[3][0] = 'R';
    rsk_strArray[3][1] = 'o';
    rsk_strArray[3][2] = 'h';
    rsk_strArray[3][3] = 'i';
    rsk_strArray[3][4] = 't';
    rsk_strArray[3][5] = '\0';
    
    rsk_strArray[4][0] = 'K';
    rsk_strArray[4][1] = 'u';
    rsk_strArray[4][2] = 'l';
    rsk_strArray[4][3] = 'k';
    rsk_strArray[4][4] = 'a';
    rsk_strArray[4][5] = 'r';
    rsk_strArray[4][6] = 'n';
    rsk_strArray[4][7] = 'i';
    rsk_strArray[4][8] = '\0';
    


    printf("\n\n");

    printf("Strings In 2D Array : \n\n");

    for(int i = 0; i < rsk_strArrayNumRows; i++) {

        printf("%s ", rsk_strArray[i]);

    }
    printf("\n\n");

    return(0);

}
