#include <stdio.h>

int main(void) {

    //prototype
    void rsk_PrintBinaryFormOfNumber(unsigned int);

    unsigned int rsk_a;
    unsigned int rsk_num_bits;
    unsigned int rsk_result;

    printf("\n\n");
    printf("Enter A Integer = ");
    scanf("%u", &rsk_a);

    printf("\n\n");
    printf("Enter How Many Bits You Want To Shift Integer A =  %d To The Right ? ", rsk_a);
    scanf("%u", &rsk_num_bits);

    printf("\n\n");
    rsk_result = rsk_a >> rsk_num_bits;
    printf("Bitwise RIGHT-SHIFTing of \nA = %d (Decimal) By = %d Bits Gives Result %d (Decimal). \n\n",rsk_a, rsk_num_bits, rsk_result);

    rsk_PrintBinaryFormOfNumber(rsk_a);
    rsk_PrintBinaryFormOfNumber(rsk_result);

    return(0);
}

void rsk_PrintBinaryFormOfNumber(unsigned int rsk_decimal_number) {

    unsigned int rsk_quotient, rsk_remainder;
    unsigned int rsk_num;
    unsigned int rsk_binary_array[8];
    int i; //iterator

    for(i = 0; i < 8; i++) {
        rsk_binary_array[i] = 0;
    }

    printf("The Binary Form Of The Decimal Integer %d Is \t = \t",rsk_decimal_number);
    rsk_num = rsk_decimal_number;
    i = 7;

    while (rsk_num != 0) {
        rsk_quotient  = rsk_num/2;
        rsk_remainder = rsk_num%2;      
        rsk_binary_array[i] = rsk_remainder;
        rsk_num = rsk_quotient;
        i--;
    }

    for(i = 0;i < 8; i++) {
        printf("%u",rsk_binary_array[i]);
    }
    printf("\n\n");

}

