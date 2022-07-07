#include <stdio.h>
#include <conio.h>

int main(void) {

    int rsk_i;
    int rsk_j;

    printf("\n\n");

    for(rsk_i = 0; rsk_i<=20; rsk_i++ ) {
    
        for(rsk_j = 0; rsk_j<=20; rsk_j++ ) {

            if(rsk_j > rsk_i) {
                break;
            } else {
                printf("* ");
            }
        }
        printf("\n");
    }

    printf("\n\n");
    printf("Exitting Loop...");
    printf("\n\n");

    return(0);
}