#include <stdio.h>

int main(void) {

    int rsk_a;
    int rsk_b;
    int rsk_c;
    int rsk_result;

    printf("\n\n");
    printf("Enter First Number : ");
    scanf("%d",&rsk_a);

    printf("\n\n");
    printf("Enter Second Number : ");
    scanf("%d",&rsk_b);


    printf("\n\n");
    printf("Enter Third Number : ");
    scanf("%d",&rsk_c);

    printf("\n\n");
    printf("If Answer = 0 ,Its 'FALSE' \n");
    printf("If Answer = 1 ,Its 'TRUE' \n\n");

    rsk_result = (rsk_a <= rsk_b) && (rsk_b != rsk_c);
    printf("LOGICAL AND (&&) : Ansewr is TRUE (1) Iff BOTH Conditions are TRUE, ELSE Answer is FALSE (0)");
    printf("A = %d is Less Than Or Equal To B = %d AND B = %d is NOT Equal To C = %d \t Answer = %d\n\n", rsk_a, rsk_b, rsk_b, rsk_c, rsk_result);

    rsk_result = (rsk_b >= rsk_a) || (rsk_a == rsk_c);
    printf("LOGICAL OR (||) : Ansewr is FALSE (0) Iff BOTH Conditions are FALSE, ELSE Answer is TRUE (1)");
    printf("B = %d is Greater Than Or Equal To A = %d OR B = %d is Equal To C = %d \t Answer = %d\n\n", rsk_b, rsk_a, rsk_b, rsk_c, rsk_result);
  

    rsk_result = !rsk_a;
    printf("A = %d , and Using Logical NOT \tGives Answer = %d\n", rsk_a, rsk_result);

    rsk_result = !rsk_b;
    printf("B = %d , and Using Logical NOT \tGives Answer = %d\n", rsk_b, rsk_result);

    rsk_result = !rsk_c;
    printf("C = %d , and Using Logical NOT \tGives Answer = %d\n", rsk_c, rsk_result);

    rsk_result = (!(rsk_a <= rsk_b) && !(rsk_b != rsk_c));
    printf("Using Logical NOT ON LOGICAL Expression (a <= b) and On (b != c) And Then AND-ing Them \tGives Answer = %d\n", rsk_result);

    printf("\n\n");

    rsk_result = !((rsk_b >= rsk_a) || (rsk_a == rsk_c));
    printf("Using Logical NOT ON ENTIER LOGICAL Expression (b >= a) || (a == c)  \tGives Answer = %d\n", rsk_result);

    printf("\n\n");

    return(0);

}

