#include <stdio.h>

int main(void) {

    int rsk_i;
    int rsk_j;

    printf("\n\n");

    printf("Printing NUmbers From 10 to 1 and 100 to 10:\n");

    rsk_i =  10;
    rsk_j = 100;

    while(rsk_i >=1 ,rsk_j>=10){

        printf("\t%d\t%d\n",rsk_i, rsk_j);

        rsk_i--;
        rsk_j = rsk_j - 10;
    }

    return(0);
}