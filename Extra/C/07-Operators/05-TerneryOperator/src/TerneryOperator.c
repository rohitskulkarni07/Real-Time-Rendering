#include <stdio.h>

int main(void) {

    int rsk_a, rsk_b;
    int rsk_p, rsk_q;
    char rsk_ch_result_01, rsk_ch_result_02;
    int rsk_i_result_01, rsk_i_result_02;

    printf("\n\n");

    rsk_a = 3;
    rsk_b = 5;

    rsk_ch_result_01 = (rsk_a > rsk_b) ? 'A'   :   'B';
    rsk_i_result_01  = (rsk_a > rsk_b) ? rsk_a : rsk_b;

    printf("Ternery Operator Anser 1 ----- %c and %d.\n\n",rsk_ch_result_01,rsk_i_result_01);

    rsk_p = 10;
    rsk_q = 10;

    rsk_ch_result_02 = (rsk_p != rsk_q) ? 'P'   :   'Q';
    rsk_i_result_02  = (rsk_p != rsk_q) ? rsk_p : rsk_q;

    printf("Ternery Operator Anser 2 ----- %c and %d.\n\n",rsk_ch_result_02,rsk_i_result_02);

    printf("\n\n");

    return (0);
}   