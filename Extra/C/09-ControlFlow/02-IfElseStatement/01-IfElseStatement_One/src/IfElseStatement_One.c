#include <stdio.h>

int main(void) {

    int rsk_a = 10, rsk_b = 3, rsk_p = 2;

    printf("\n\n");

    printf("\n\n");
    if(rsk_a < rsk_b){

        printf("ENtering First if-block..\n\n");
        printf("A is less than B\n\n");
    }
    else
    {
        printf("ENtering First else-block..\n\n");
        printf("A NOT less than B\n\n");
    }
    printf("First if-lese Pair Done");
    
    if(rsk_b != rsk_p){

        printf("ENtering Second if-block..\n\n");
        printf("B Is Not Equal To P\n\n");

    }
    else
    {
        printf("ENtering First else-block..\n\n");
        printf("B Is Equal To P\n\n");
    }
    printf("Second if-lese Pair Done");
    

    return(0);

}