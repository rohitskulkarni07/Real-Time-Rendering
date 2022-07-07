#include <stdio.h>

enum {
    NEGATIVE = -1,
    ZERO,
    POSITIVE
};

int main(void) {

    int Difference(int, int, int *);

    int rsk_a;
    int rsk_b;
    int rsk_answer, rsk_ret;

    printf("\n\n");
    printf("Enter Value For 'A' : ");
    scanf("%d", &rsk_a);

    printf("\n\n");
    printf("Enter Value For 'B' : ");
    scanf("%d", &rsk_b);

    rsk_ret = Difference(rsk_a, rsk_b, &rsk_answer);

    printf("\n\n");

    printf("Difference Of %d And %d = %d\n\n", rsk_a, rsk_b, rsk_answer);

    if(rsk_ret == POSITIVE) {
        printf("The Difference Of %d And %d Is Positive !!!\n\n", rsk_a, rsk_b);
    } else if (rsk_ret == NEGATIVE) {
        printf("The Difference Of %d And %d Is Negative !!!\n\n", rsk_a, rsk_b);

    } else {
        printf("The Difference Of %d And %d Is Zero !!!\n\n", rsk_a, rsk_b);

    }

    return(0);
}

int Difference(int x, int y, int *difference) {

    *difference = x - y;

    if(*difference > 0) {
        return(POSITIVE);

    } else if (*difference < 0) {
        return(NEGATIVE);

    }else { 
        return(ZERO);

    }

    return(ZERO);
}