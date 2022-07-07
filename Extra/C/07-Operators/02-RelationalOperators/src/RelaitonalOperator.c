#include <stdio.h>

int main(void) {

    int rsk_a;
    int rsk_b;
    int rsk_result;

    printf("\n\n");
    printf("Enter A Number : ");
    scanf("%d",&rsk_a);

    printf("\n\n");
    printf("Enter Another Number : ");
    scanf("%d",&rsk_b);

    printf("\n\n");
    printf("If Answer = 0 ,Its 'FALSE' \n");
    printf("If Answer = 1 ,Its 'TRUE' \n\n");

    rsk_result = (rsk_a < rsk_b);
    printf("(a < b) A = %d is Less Than B = %d \t Answer = %d\n", rsk_a, rsk_b, rsk_result);

    rsk_result = (rsk_a > rsk_b);
    printf("(a > b) A = %d is Greater Than B = %d \t Answer = %d\n", rsk_a, rsk_b, rsk_result);

    rsk_result = (rsk_a <= rsk_b);
    printf("(a <= b) A = %d is Less Than Or Equal To B = %d \t Answer = %d\n", rsk_a, rsk_b, rsk_result);

    rsk_result = (rsk_a >= rsk_b);
    printf("(a >= b) A = %d is Greater Than Or Equal To B = %d \t Answer = %d\n", rsk_a, rsk_b, rsk_result);

    rsk_result = (rsk_a == rsk_b);
    printf("(a == b) A = %d is Equal To B = %d \t Answer = %d\n", rsk_a, rsk_b, rsk_result);

    rsk_result = (rsk_a != rsk_b);
    printf("(a != b) A = %d is NOT Equal ToB = %d \t Answer = %d\n", rsk_a, rsk_b, rsk_result);


    return(0);

}

