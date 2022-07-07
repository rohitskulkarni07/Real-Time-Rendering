#include <stdio.h>

#define INT_ARRAY_NUM_ELEMENTS 5
#define FLOAT_ARRAY_NUM_ELEMENTS 3
#define CHAR_ARRAY_NUM_ELEMENTS 15

int main(void) {

    int   rsk_iArray[INT_ARRAY_NUM_ELEMENTS];
    float rsk_fArray[FLOAT_ARRAY_NUM_ELEMENTS];
    char  rsk_cArray[CHAR_ARRAY_NUM_ELEMENTS];

    printf("\n\n");

    printf("Enter Elements For Integer Array : \n");
    for(int i = 0; i<INT_ARRAY_NUM_ELEMENTS; i++) {

        scanf("%d",&rsk_iArray[i]);
    }

    printf("Enter Elements For Float Array : \n");
    for(int i = 0; i<FLOAT_ARRAY_NUM_ELEMENTS; i++) {

        scanf("%f",&rsk_fArray[i]);
    }

    printf("Enter Elements For Char Array : \n");
    for(int i = 0; i<CHAR_ARRAY_NUM_ELEMENTS; i++) {

        scanf("%c",&rsk_cArray[i]);
    }

    //_________________________________________________________

    printf("\n\n");

    printf("Printing Elements For Integer Array : \n\n");
    for(int i = 0; i<INT_ARRAY_NUM_ELEMENTS; i++) {
        printf("iArray[%d] = [%d]\n",i+1,rsk_iArray[i]);
    }

    printf("Printing Elements For Float Array : \n\n");
    for(int i = 0; i<FLOAT_ARRAY_NUM_ELEMENTS; i++) {

        printf("fArray[%d] = [%f]\n",i+1,rsk_fArray[i]);        
    }

    printf("Printing Elements For Char Array : \n\n");
    for(int i = 0; i<CHAR_ARRAY_NUM_ELEMENTS; i++) {

        printf("cArray[%d] = [%c]\n",i+1,rsk_cArray[i]);

    }
    return(0);
}