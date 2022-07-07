#include <stdio.h>

int main(int argc, char *argv[], char *envp) {
    
    int MyAddition(void);


    int rsk_sum = MyAddition();

    printf("\n\n");
    printf("Sum : %d \n\n",rsk_sum);

    return(0);
}

int MyAddition(void) {

    int rsk_a, rsk_b, rsk_sum;

    printf("\n\n");

    printf("ENter A Value For 'A' : ");
    scanf("%d",& rsk_a);

    printf("\n\n");

    printf("ENter A Value For 'B' : ");
    scanf("%d",& rsk_b);
    
    rsk_sum = rsk_a + rsk_b;

    return(rsk_sum);
}
