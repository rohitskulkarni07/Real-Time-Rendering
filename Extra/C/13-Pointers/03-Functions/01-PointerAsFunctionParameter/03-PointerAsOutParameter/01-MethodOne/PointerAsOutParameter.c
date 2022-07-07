#include <stdio.h>

int main(void) {

    void MathematicalOperation(int, int, int *, int *,int *, int *, int *);

    int rsk_a;
    int rsk_b;
    int rsk_answer_sum;
    int rsk_answer_difference;
    int rsk_answer_product;
    int rsk_answer_quotient;
    int rsk_answer_remainder;

    printf("\n\n");
    printf("Enter Value For 'A' : ");
    scanf("%d", &rsk_a);

    printf("\n\n");
    printf("Enter Value For 'B' : ");
    scanf("%d", &rsk_b);    

    MathematicalOperation(rsk_a,
                        rsk_b,
                        &rsk_answer_sum,
                        &rsk_answer_difference,
                        &rsk_answer_product,
                        &rsk_answer_quotient,
                        &rsk_answer_remainder);

    printf("\n\n");
    printf("***** Results *****\n\n");
    printf("Sum        = %d\n\n",rsk_answer_sum);
    printf("Difference = %d\n\n",rsk_answer_difference);
    printf("Product    = %d\n\n",rsk_answer_product);
    printf("Quotient   = %d\n\n",rsk_answer_quotient);
    printf("Remainder  = %d\n\n",rsk_answer_remainder);

    return (0);
}

void MathematicalOperation(int x, int y, int *sum, int *difference,int *product, int *quotient, int *remainder) {

    *sum = x + y;
    *difference = x - y;
    *product = x * y;
    *quotient = x/y;
    *remainder = x % y;

}
