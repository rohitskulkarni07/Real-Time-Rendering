#include <stdio.h>

int main(void) {

    int rsk_num;

    printf("\n\n");

    printf("Enter Value For Num : ");
    scanf("%d", &rsk_num);

    if(rsk_num < 0) {
        printf("Num = %d Is Less Than 0 (Negative) \n\n", rsk_num);

    }
    if((rsk_num > 0) && (rsk_num <= 100)) {
        printf("Num = %d Is Between 0 and 100 \n\n",rsk_num);
        
    }
    if((rsk_num > 100) && (rsk_num <= 200)) {
        printf("Num = %d Is Between 100 and 200 \n\n",rsk_num);
        
    }
    if((rsk_num > 200) && (rsk_num <= 300)) {
        printf("Num = %d Is Between 200 and 300 \n\n",rsk_num);
        
    }
     if((rsk_num > 300) && (rsk_num <= 400)) {
        printf("Num = %d Is Between 300 and 400 \n\n",rsk_num);
        
    }
     if((rsk_num > 400) && (rsk_num <= 500)) {
        printf("Num = %d Is Between 400 and 500 \n\n",rsk_num);
        
    }

    if(rsk_num > 500) {
        printf("Num = %d Is Greater Than 500 \n\n",rsk_num);
        
    }
        
    return(0);
}