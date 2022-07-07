#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void) {

    int rsk_iArray[NUM_ELEMENTS];
    int rsk_num, rsk_sum = 0, rsk_count;

    printf("\n\n");

    printf("Enter Integer Elememts For Array : \n\n");
    for(int i = 0; i < NUM_ELEMENTS; i++) {

        scanf("%d",&rsk_num);
        
        if(rsk_num < 0) {
            rsk_num = -1 * rsk_num;
        }

        rsk_iArray[i] = rsk_num;

    }

    printf("\n\n");

    printf("Prime Numbers :\n");

    for(int i=0;i<NUM_ELEMENTS;i++) {
        for(int j = 1; j<=rsk_iArray[i];j++) {
            if((rsk_iArray[i] % j)==0) {
                rsk_count++;
            }
        }
        if(rsk_count == 2) {

            printf("%d\n", rsk_iArray[i]);
        }
        rsk_count = 0;
    }

    printf("\n\n");
    
    return(0);
}