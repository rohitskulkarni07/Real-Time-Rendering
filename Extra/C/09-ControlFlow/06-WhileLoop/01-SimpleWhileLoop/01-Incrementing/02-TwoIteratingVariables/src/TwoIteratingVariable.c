#include <stdio.h>

int main(void) {

    int rsk_i;
    int rsk_j;

    printf("\n\n");

    printf("Prining Digigts 1 to 10 and 10 to 100: \n\n");

    rsk_i = 1;
    rsk_j = 10;

    while(rsk_i <=10, rsk_j <= 100){

        printf("\t %d \t %d\t\n",rsk_i,rsk_j);
        rsk_i++;
        rsk_j = rsk_j + 10;
    }

    printf("\n\n");
    
    return(0);
}