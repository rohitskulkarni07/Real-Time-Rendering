#include <stdio.h>

int main(void) {

    int rsk_num;

    printf("\n\n");
    printf("ENter Value For Num : ");
    scanf("%d", &rsk_num);

    if(rsk_num < 0) {
        printf("Num = %d Is Less Than 0 (NEGATIVE)\n\n", rsk_num);

    } else {

        if((rsk_num > 0) && (rsk_num <= 100)) {
                printf("Num = %d Is Between 0 and 100\n", rsk_num);  
        } else {

            if((rsk_num > 100) && (rsk_num <= 200)) {
                printf("Num = %d Is Between 100 and 200\n", rsk_num);  
            } else {

                if((rsk_num > 200) && (rsk_num <= 300)) {
                    printf("Num = %d Is Between 200 and 300\n", rsk_num);  
                } else {

                    if((rsk_num > 300) && (rsk_num <= 400)) {
                        printf("Num = %d Is Between 300 and 400\n", rsk_num);  
                    } else {
                        
                        if((rsk_num > 400) && (rsk_num <= 500)) {
                            printf("Num = %d Is Between 400 and 500\n", rsk_num);
                        } else {
                            printf("Num = %d Is Greater than 500\n", rsk_num);
                        }
                    }
                }
            }
        }
    }
    printf("\n\n");
    return(0);

}