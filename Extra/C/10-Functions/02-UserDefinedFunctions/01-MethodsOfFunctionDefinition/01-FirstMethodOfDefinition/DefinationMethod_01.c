#include <stdio.h>

int main(int argc, char *argv[], char *envp) {
    
    void MyAddition(void);

    MyAddition();

    return(0);
}

void MyAddition(void) {

    int rsk_a, rsk_b, rsk_sum;

    printf("\n\n");

    printf("ENter A Value For 'A' : ");
    scanf("%d",& rsk_a);

    printf("\n\n");

    printf("ENter A Value For 'B' : ");
    scanf("%d",& rsk_b);

    printf("\n\n");

    rsk_sum = rsk_a + rsk_b;

    printf("\n\n");
    printf("The Sum Of %d And %d is %d \n\n", rsk_a, rsk_b, rsk_sum);
    
}
