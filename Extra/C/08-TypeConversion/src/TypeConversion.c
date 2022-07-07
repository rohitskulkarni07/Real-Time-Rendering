#include <stdio.h>

int main(void) {

    int rsk_i, rsk_j;
    int rsk_ch_01, rsk_ch_02;

    int rsk_a, rsk_result_int;
    float rsk_f, rsk_result_float;

    int rsk_i_explicit;
    float rsk_f_expicit;

    printf("\n\n");

    rsk_i = 70;
    rsk_ch_01 = rsk_i; // int to char implicit type-conversion
    printf("I = %d\n", rsk_i);
    printf("Charachter 1 (after rsk_ch_01 = rsk_i) = %c\n\n", rsk_ch_01);

    rsk_ch_02 = 'Q';
    rsk_j = rsk_ch_02; // char to int implicit type-conversion
    printf("Character 2 = %c\n", rsk_ch_02);
    printf("J (after rsk_ j = rsk_ch_02) = %d\n\n",rsk_j);

    rsk_a = 5;
    rsk_f = 7.8;
    
    rsk_result_float = rsk_a + rsk_f;
    printf("Integer rsk_a = %d And Floating Point Number %f Added Gives Floating-Point Sum  %f \n\n", rsk_a, rsk_f, rsk_result_float);
    
    rsk_result_int = rsk_a + rsk_f;
    printf("Integer rsk_a = %d And Floating Point Number %f Added Gives Integer Sum  %d \n\n", rsk_a, rsk_f, rsk_result_int);

    //explicit type-conversion

    rsk_f_expicit = 30.12199f;
    rsk_i_explicit = (int)rsk_f_expicit;

    printf("Floating Point Number Which Will Be Tpye Casted Explicitly = %f\n",rsk_f_expicit);

    printf("Resultant Integer After Explicit Type Casting Of %f = %d \n\n", rsk_f_expicit, rsk_i_explicit);

    return (0);
}