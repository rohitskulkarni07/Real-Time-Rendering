#include <stdio.h>

int main() {

    void SwapNumbers(int, int );

    int rsk_a;
    int rsk_b;

    printf("\n\n");
    printf("Enter Value For 'A' : ");
    scanf("%d", &rsk_a);

    printf("\n\n");
    printf("Enter Value For 'B' : ");
    scanf("%d", &rsk_b);    

    printf("\n\n");
    printf("Before Swapping\n\n");
    printf("Value Of 'A' = %d\n\n", rsk_a);
    printf("Value Of 'B' = %d\n\n", rsk_b);

    SwapNumbers(rsk_a, rsk_b);

    printf("\n\n");
    printf("After Swapping\n\n");
    printf("Value Of 'A' = %d\n\n", rsk_a);
    printf("Value Of 'B' = %d\n\n", rsk_b);

    return(0);
} 

void SwapNumbers(int x, int y) {

    int temp;

    printf("\n\n");
    printf("Before Swapping\n\n");
    printf("Value Of 'X' = %d\n\n",x);
    printf("Value Of 'Y' = %d\n\n",y);

    temp = x;
    x = y;
    y = temp;

    printf("\n\n");
    printf("After Swapping\n\n");
    printf("Value Of 'X' = %d\n\n",x);
    printf("Value Of 'Y' = %d\n\n",y);
}
