#include <stdio.h>

int main() {

    int rsk_a;
    int rsk_b;
    int rsk_x;

    printf("\n\n");

    printf("Enter A Number : ");
    scanf("%d", &rsk_a);

    printf("\n\n");

    printf("Enter Another Number : ");
    scanf("%d", &rsk_b);

    printf("\n\n");

    rsk_x = rsk_a;
    rsk_a += rsk_b; // a = a + b
    printf("Addition of A = %d and B = %d gives %d.\n", rsk_a, rsk_b, rsk_x);
    
    rsk_x = rsk_a;
    rsk_a -= rsk_b; // a = a - b
    printf("Subtraction of A = %d and B = %d gives %d.\n", rsk_a, rsk_b, rsk_x);

    
    rsk_x = rsk_a;
    rsk_a *= rsk_b; // a = a * b
    printf("Multiplication of A = %d and B = %d gives %d.\n", rsk_a,rsk_b, rsk_x);

    
    rsk_x = rsk_a;
    rsk_a /= rsk_b; // a = a / b
    printf("Division of A = %d and B = %d gives Quotient %d.\n", rsk_a, rsk_b, rsk_x);

    
    rsk_x = rsk_a;
    rsk_a %= rsk_b; // a = a % b
    printf("Division of A = %d and B = %d gives Remainder %d.\n", rsk_a, rsk_b, rsk_x);

    printf("\n\n");
    
    return(0);
}