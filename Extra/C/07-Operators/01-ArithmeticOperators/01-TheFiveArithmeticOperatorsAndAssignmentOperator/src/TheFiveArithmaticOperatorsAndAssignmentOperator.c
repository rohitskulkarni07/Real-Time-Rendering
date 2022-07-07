#include <stdio.h>

int main() {

    int rsk_a;
    int rsk_b;
    int rsk_result;

    printf("\n\n");
    
    printf("Enter A Number : ");
    scanf("%d", &rsk_a);

    printf("\n\n");

    printf("Enter Another NUmber : ");
    scanf("%d", &rsk_b);

    printf("\n\n");


    rsk_result = rsk_a + rsk_b; //add a and b, store result in variable name 'result'
    printf("Addition of A = %d And B = %d Gives %d.\n", rsk_a, rsk_b, rsk_result);

    rsk_result = rsk_a - rsk_b; //subtract a and b, store result in variable name 'result'
    printf("Subtraction of A = %d And B = %d Gives %d.\n", rsk_a, rsk_b, rsk_result);

    rsk_result = rsk_a * rsk_b; //multiply a into b, store result in variable name 'result'
    printf("Multiplication of A = %d And B = %d Gives %d.\n", rsk_a, rsk_b, rsk_result);

    rsk_result = rsk_a / rsk_b; //divide a and b store quotient in variable name 'result'
    printf("division of A = %d And B = %d Gives Quotient %d.\n", rsk_a, rsk_b, rsk_result);

    rsk_result = rsk_a % rsk_b; //divide a and b store remainder in variable name 'result'
    printf("division of A = %d And B = %d Gives Remainder %d.\n", rsk_a, rsk_b, rsk_result);

    printf("\n\n");

    return(0);

}