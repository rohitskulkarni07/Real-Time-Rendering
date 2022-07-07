#include <stdio.h>

int main(void) {

    int rsk_i, rsk_j;


    printf("\n\n");

    printf("Outer Loop Prints Odd Numbers Between 1 and 10. \n\n");
    printf("Inner Loop Prints Even Numbers Between 1 and 10 For Every Odd Number Printed By Outer Loop. \n\n");

    for(rsk_i = 1; rsk_i <=10; rsk_i++) {

        if(rsk_i%2 != 0){

            printf("i = %d\n",rsk_i);
            printf("____________\n");

            for(rsk_j=1; rsk_j<=10; rsk_j++) {
                
                if(rsk_j % 2==0)
                    printf("\tj = %d\n",rsk_j);
                else
                    continue;
            
            }
            printf("\n\n");
        } else {
            continue;
        }
    }
    printf("\n\n");

    return(0);

}