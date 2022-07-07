#include <stdio.h>

int main() {

    int rsk_i;
    int rsk_j;
    int rsk_c;

    printf("\n\n");

    rsk_i = 0;
    do {

        rsk_j = 0;
        do {

            rsk_c = ((rsk_i & 0x8) == 0) ^ ((rsk_j & 0x8) ==0);

            if(rsk_c == 0) {
                printf(" ");
            } 

            if(rsk_c == 1) {
                printf("* ");
            }   

            rsk_j++;
        }while( rsk_j < 64);
        printf("\n\n");
        rsk_i++;
    }while( rsk_i < 64);
    
    return(0);
}