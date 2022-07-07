#include <stdio.h>

int main(int argc, char *argv[], char *envp) {
    
    int rsk_a;
    int rsk_b;

    void MyAddition(int, int);

    printf("\n\n");

    printf("ENter A Value For 'A' : ");
    scanf("%d",&rsk_a);

    printf("\n\n");

    printf("ENter A Value For 'B' : ");
    scanf("%d",&rsk_b);

    MyAddition(rsk_a,rsk_b);

 

    return(0);
}

void MyAddition(int rsk_a,int rsk_b) {

    int rsk_sum;

    rsk_sum = rsk_a + rsk_b;

    printf("\n\n");
    printf("Sum Of A = %d And B = %d is : %d \n\n", rsk_a, rsk_b, rsk_sum);

}
