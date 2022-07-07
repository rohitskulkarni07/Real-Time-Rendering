#include <stdio.h>

int main(int argc, char *argv[], char *envp) {
    
    int MyAddition(int ,int);

    int rsk_a, rsk_b;
    printf("\n\n");

    printf("ENter A Value For 'A' : ");
    scanf("%d",& rsk_a);

    printf("\n\n");

    printf("ENter A Value For 'B' : ");
    scanf("%d",& rsk_b);

    int rsk_sum = MyAddition(rsk_a, rsk_b);

    printf("\n\n");
    printf("Sum : %d \n\n",rsk_sum);

    return(0);
}

int MyAddition(int rsk_a, int rsk_b) {

    int rsk_sum;

    rsk_sum = rsk_a + rsk_b;

    return(rsk_sum);
}
