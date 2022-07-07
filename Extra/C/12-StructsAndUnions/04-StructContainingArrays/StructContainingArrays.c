#include <stdio.h>

#define INT_ARRAY_SIZE 10
#define FLOAT_ARRAY_SIZE 5
#define CHAR_ARRAY_SIZE 26

#define NUM_STRINGS 10
#define MAX_CHARACHTERS_PER_STRING 20

#define ALPHABET_BEGINNING 65

struct MyDataOne {
    int rsk_iArray[INT_ARRAY_SIZE];
    float rsk_fArray[FLOAT_ARRAY_SIZE];
};


struct MyDataTwo {
    char rsk_cArray[CHAR_ARRAY_SIZE];
    char rsk_strArray[NUM_STRINGS][MAX_CHARACHTERS_PER_STRING];
};

int main(void ) {

    struct MyDataOne data_one;
    struct MyDataTwo data_two;

    int i;

    // piece meal assignment 

    data_one.rsk_fArray[0] = 0.1f;
    data_one.rsk_fArray[1] = 1.2f;
    data_one.rsk_fArray[2] = 2.3f;
    data_one.rsk_fArray[3] = 3.4f;
    data_one.rsk_fArray[4] = 4.5f;

    // loop assignment

    for ( i = 0; i < INT_ARRAY_SIZE; i++) {

        scanf("%d", &data_one.rsk_iArray[i]);        

    }

    for ( i = 0; i < CHAR_ARRAY_SIZE; i++) {

        data_two.rsk_cArray[i] = (char)(i+ALPHABET_BEGINNING);

    }

    strcpy(data_two.rsk_strArray[0],"Welcome !!!");
    strcpy(data_two.rsk_strArray[1],"This");
    strcpy(data_two.rsk_strArray[2],"Is");
    strcpy(data_two.rsk_strArray[3],"ASTROMEDICOMP'S");
    strcpy(data_two.rsk_strArray[4],"Real");
    strcpy(data_two.rsk_strArray[5],"Time");
    strcpy(data_two.rsk_strArray[6],"Rendering");
    strcpy(data_two.rsk_strArray[7],"Batch");
    strcpy(data_two.rsk_strArray[8],"Of");
    strcpy(data_two.rsk_strArray[9],"2020-2021");
    
    printf("\n\n");
    
    printf("Integer Array (data_one.iArray[]): \n\n");

    for(i = 0; i < INT_ARRAY_SIZE;i++) {
    
        printf("data_one.iArray[%d] = %d\n",i, data_one.rsk_iArray[i]);
    
    }
    
    printf("\n\n");
    
    printf("Floating-point Array (data_one.fArray[]): \n\n");
    
    for(i = 0; i < FLOAT_ARRAY_SIZE;i++) {
    
        printf("data_one.fArray[%d] = %f\n",i, data_one.rsk_fArray[i]);
    
    }

    printf("\n\n");

    printf("Charachter Array (data_two.cArray[]) : \n\n");

    for(i = 0; i < CHAR_ARRAY_SIZE;i++) {
    
        printf("data_two.cArray[%d] = %c\n",i, data_two.rsk_cArray[i]);
    
    }

    printf("\n\n");

    printf("Charachter Array (data_two.cArray[]) : \n\n");

    for(i = 0; i < NUM_STRINGS;i++) {
    
        printf("%s ", data_two.rsk_strArray[i]);
    
    }

    printf("\n\n");

    return(0);
}