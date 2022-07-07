#include <stdio.h>

int main() {

    int rsk_i;
    int rsk_j;
    int rsk_c;

    printf("\n\n");

    for(rsk_i = 0; rsk_i < 64; rsk_i++) {
        
        for(rsk_j = 0; rsk_j < 64; rsk_j++) {

            rsk_c = ((rsk_i & 0x8) == 0) ^ ((rsk_j & 0x8) ==0);

            if(rsk_c == 0) {
                printf(" ");
            } 

            if(rsk_c == 1) {
                printf("* ");
            }   
        
        }
        printf("\n\n");
    }
    
    return(0);
}