#include <stdio.h>

int main(void) {

    float rsk_f;
    float rsk_f_num = 1.7f;

    printf("\n\n");

    printf("Printing Numbers from %f to %f: \n\n", rsk_f_num, (rsk_f_num * 10.0f));

    rsk_f = rsk_f_num;

   do {

        printf("\t%f\n",rsk_f);
        rsk_f = rsk_f + rsk_f_num;
        
    }while(rsk_f <= (rsk_f_num * 10.0f));

    return (0);
}