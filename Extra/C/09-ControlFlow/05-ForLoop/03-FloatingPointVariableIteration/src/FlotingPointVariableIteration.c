#include <stdio.h>

int main() {
    float rsk_f;
    float rsk_f_num = 1.7f;

    printf("\n\n");

    printf("printing NUmber %f to %f : \n",rsk_f_num, (rsk_f * 10.0f));

    for(rsk_f = rsk_f_num ; rsk_f<=(rsk_f_num*10.0f); rsk_f= rsk_f + rsk_f_num) {
        printf("\t %f \n",rsk_f);

    }
    printf("\n\n");

    return(0);
}