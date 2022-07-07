#include <stdio.h>

int main(void) {

    int rsk_iNum, rsk_num, rsk_i;

    printf("\n\n");
    
    printf("Enter AN Integer Value From Which Iteration Must Begin: ");
    scanf("%d",&rsk_iNum);

    printf("How Many Digits Do You Want To Print From %d Onwords",rsk_iNum);
    scanf("%d",&rsk_num);

    printf("Printing Digits From %d to %d \n\n",rsk_iNum,( rsk_num+rsk_iNum));

    rsk_i = rsk_iNum;

    while (rsk_i<= (rsk_iNum + rsk_num)) {
        printf("\t%d\n",rsk_i);
        rsk_i ++;

    }
    
    printf("\n\n");
    return(0);
}