#include <stdio.h>

int main(void) {

    int rsk_iArray[] = { 9, 18, 12, 1, 44};
    int rsk_iSize;
    int rsk_iArraySize;
    int rsk_iNumOfElements;

    float rsk_fArray[] = { 9.4f, 18.2f, 12.1f, 1.9f, 44.5f};
    int rsk_fSize;
    int rsk_fArraySize;
    int rsk_fNumOfElements;

    char rsk_cArray[] = { 'R', 'O', 'H', 'I', 'T'};
    int rsk_cSize;
    int rsk_cArraySize;
    int rsk_cNumOfElements;

    printf("\n\n");
    printf("Initialization of Array and Piecemeal display of elements of Integer Array\n\n");

    printf("iArray[0] = 1st Element => [%d] \n",rsk_iArray[0]);
    printf("iArray[1] = 2nd Element => [%d] \n",rsk_iArray[1]);
    printf("iArray[2] = 3rd Element => [%d] \n",rsk_iArray[2]);
    printf("iArray[3] = 4th Element => [%d] \n",rsk_iArray[3]);
    printf("iArray[4] = 5th Element => [%d] \n",rsk_iArray[4]);

    rsk_iSize = sizeof(int);
    rsk_iArraySize = sizeof(rsk_iArray);
    rsk_iNumOfElements = rsk_iArraySize / rsk_iSize;

    printf("Size Of 'int' Datatype %d bytes \n\n", rsk_iSize);
    printf("Number of Elements in 'int' Array 'iArray[]' = %d elemtns' \n\n",rsk_iNumOfElements);
    printf("Size Of 'iArrray[]' (%d Elements * %d Bytes) = %d Bytes \n\n",rsk_iNumOfElements, rsk_iSize, rsk_iArraySize);


//_________________________________________________________________________________________________________________________________

    printf("\n\n");
    printf("Initialization of Array and Piecemeal display of elements of Float Array\n\n");

    printf("fArray[0] = 1st Element => [%f] \n",rsk_fArray[0]);
    printf("fArray[1] = 2nd Element => [%f] \n",rsk_fArray[1]);
    printf("fArray[2] = 3rd Element => [%f] \n",rsk_fArray[2]);
    printf("fArray[3] = 4th Element => [%f] \n",rsk_fArray[3]);
    printf("fArray[4] = 5th Element => [%f] \n",rsk_fArray[4]);

    rsk_fSize = sizeof(float);
    rsk_fArraySize = sizeof(rsk_fArray);
    rsk_fNumOfElements = rsk_fArraySize / rsk_fSize;

    printf("Size Of 'float' Datatype %d bytes \n\n", rsk_fSize);
    printf("Number of Elements in 'float' Array 'fArray[]' = %d elemtns' \n\n",rsk_fNumOfElements);
    printf("Size Of 'fArrray[]' (%d Elements * %d Bytes) = %d Bytes \n\n",rsk_fNumOfElements, rsk_fSize, rsk_fArraySize);
//_________________________________________________________________________________________________________________________________

    printf("\n\n");
    printf("Initialization of Array and Piecemeal display of elements of Charachter Array\n\n");

    printf("cArray[0] = 1st Element => [%c] \n",rsk_cArray[0]);
    printf("cArray[1] = 2nd Element => [%c] \n",rsk_cArray[1]);
    printf("cArray[2] = 3rd Element => [%c] \n",rsk_cArray[2]);
    printf("cArray[3] = 4th Element => [%c] \n",rsk_cArray[3]);
    printf("cArray[4] = 5th Element => [%c] \n",rsk_cArray[4]);

    rsk_cSize = sizeof(char);
    rsk_cArraySize = sizeof(rsk_cArray);
    rsk_cNumOfElements = rsk_cArraySize / rsk_cSize;
    
    printf("Size Of 'char' Datatype %d bytes \n\n", rsk_cSize);
    printf("Number of Elements in 'char' Array 'cArray[]' = %d elemtns' \n\n",rsk_cNumOfElements);
    printf("Size Of 'cArrray[]' (%d Elements * %d Bytes) = %d Bytes \n\n",rsk_cNumOfElements, rsk_cSize, rsk_cArraySize);

    return(0);
    
}