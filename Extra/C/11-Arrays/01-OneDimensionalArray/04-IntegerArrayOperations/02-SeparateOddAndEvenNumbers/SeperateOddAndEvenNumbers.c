#include <stdio.h>

#define NUM_ELEMENTS 10

int main(void) {

    int rsk_iArray[NUM_ELEMENTS];
    int rsk_num, rsk_sum = 0;

    printf("\n\n");

    printf("Enter Integer Elememts For Array : \n\n");
    for(int i = 0; i < NUM_ELEMENTS; i++) {

        scanf("%d",&rsk_num);
        rsk_iArray[i] = rsk_num;

    }


    printf("\n\n");

    printf("Even Numbers\n");
    for(int i = 0; i<NUM_ELEMENTS; i++) {

        if(rsk_iArray[i] % 2 ==0){
            printf("%d\n",rsk_iArray[i]);
        }
    }
   
    printf("\n\n");
   
    printf("Odd Numbers\n");
    for(int i = 0; i<NUM_ELEMENTS; i++) {
        
        if(rsk_iArray[i] % 2 !=0){
            printf("%d\n",rsk_iArray[i]);
        }

    }
    printf("\n\n");

    return(0);
}