#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void) {

    int rsk_iArray[NUM_ELEMENTS];
    int rsk_num, rsk_sum = 0;

    printf("\n\n");
    printf("Enter Integer Elements For Array : \n\n");

    for(int i = 0;i<NUM_ELEMENTS;i++) {

        scanf("%d", &rsk_num);
        rsk_iArray[i] = rsk_num;


    }

    for(int i = 0;i<NUM_ELEMENTS;i++) {

        rsk_sum = rsk_sum + rsk_iArray[i];

        
    }

    printf("Sum Of all the elemtents in Array = %d \n\n", rsk_sum);
    return(0);
}