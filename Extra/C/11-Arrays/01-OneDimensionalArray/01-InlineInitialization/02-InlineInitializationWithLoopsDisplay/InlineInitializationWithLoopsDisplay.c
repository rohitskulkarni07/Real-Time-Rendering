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
    printf("Initialization of Array and Loop display of elements of Integer Array\n\n");

    
    rsk_iSize = sizeof(int);
    rsk_iArraySize = sizeof(rsk_iArray);
    rsk_iNumOfElements = rsk_iArraySize / rsk_iSize;

    for(int i = 0; i<rsk_iNumOfElements; i++){
       printf("iArray[%d] (Element %d ) [%d] \n",i, (i+1), rsk_iArray[i]);
    }
 

    printf("Size Of 'int' Datatype %d bytes \n\n", rsk_iSize);
    printf("Number of Elements in 'int' Array 'iArray[]' = %d elemtns' \n\n",rsk_iNumOfElements);
    printf("Size Of 'iArrray[]' (%d Elements * %d Bytes) = %d Bytes \n\n",rsk_iNumOfElements, rsk_iSize, rsk_iArraySize);


//_________________________________________________________________________________________________________________________________

    printf("\n\n");
    printf("Initialization of Array and Piecemeal display of elements of Float Array\n\n");

    rsk_fSize = sizeof(float);
    rsk_fArraySize = sizeof(rsk_fArray);
    rsk_fNumOfElements = rsk_fArraySize / rsk_fSize;

    for(int i = 0; i<rsk_fNumOfElements; i++){
       printf("iArray[%d] (Element %d ) [%f] \n",i, (i+1), rsk_fArray[i]);
    }
    
    printf("Size Of 'float' Datatype %d bytes \n\n", rsk_fSize);
    printf("Number of Elements in 'float' Array 'fArray[]' = %d elemtns' \n\n",rsk_fNumOfElements);
    printf("Size Of 'fArrray[]' (%d Elements * %d Bytes) = %d Bytes \n\n",rsk_fNumOfElements, rsk_fSize, rsk_fArraySize);
//_________________________________________________________________________________________________________________________________

    printf("\n\n");
    printf("Initialization of Array and Piecemeal display of elements of Charachter Array\n\n");

    rsk_cSize = sizeof(char);
    rsk_cArraySize = sizeof(rsk_cArray);
    rsk_cNumOfElements = rsk_cArraySize / rsk_cSize;

    for(int i = 0; i<rsk_cNumOfElements; i++){
       printf("iArray[%d] (Element %d ) [%c] \n",i, (i+1), rsk_cArray[i]);
    }
    
    printf("Size Of 'char' Datatype %d bytes \n\n", rsk_cSize);
    printf("Number of Elements in 'char' Array 'cArray[]' = %d elemtns' \n\n",rsk_cNumOfElements);
    printf("Size Of 'cArrray[]' (%d Elements * %d Bytes) = %d Bytes \n\n",rsk_cNumOfElements, rsk_cSize, rsk_cArraySize);

    return (0);
}