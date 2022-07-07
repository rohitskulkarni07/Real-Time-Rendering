#include <stdio.h>

int main(void) {

    int rsk_i_num, rsk_num, rsk_i;

    printf("\n\n");
    
    printf("ENter An Integer Value From Which Iteration Must Begin : ");
    scanf("%d",&rsk_i_num);

    printf("HOw Many Digits Do You Want Print From %d Onwords ? :", rsk_i_num);
    scanf("%d", &rsk_num);

    printf("Printing Digits from %d to %d :\n\n", rsk_i_num , (rsk_i_num + rsk_num));

    for(rsk_i = rsk_i_num; rsk_i <= (rsk_i_num + rsk_num); rsk_i++) {

        printf("\t %d \n", rsk_i);
    }

    printf("\n\n");

    return(0);
}