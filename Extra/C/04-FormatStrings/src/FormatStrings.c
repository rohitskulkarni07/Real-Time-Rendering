#include <stdio.h>

int main() {

    //code 
    printf("\n\n");
    printf("**************************************************************************************************************\n");
    printf("\n\n");

    printf("Hello, World!\n\n");

    int rsk_a = 13;
    printf("Integer Decimal value of 'a' =  %d\n", rsk_a);
    printf("Integer Octal value of 'a' =  %o\n", rsk_a);
    printf("Integer Hexadecimal[lowercase] value of 'a' =  %x\n", rsk_a);
    printf("Integer Hexadecimal[uppercase] value of 'a' =  %X\n\n", rsk_a);

    char rsk_ch = 'A';
    printf("Character ch = %c\n", rsk_ch);
    
    char rsk_str[] =  "Astromedicomp's Real TIme Rendering Batch 3.0 (2020-2021)";
    printf("String str= %s\n\n", rsk_str);

    long rsk_num = 30121995L;
    printf("Long Integer 'num' = %ld\n\n", rsk_num);

    unsigned int rsk_b = 7;
    printf("Unsigned Integer 'b' = %u\n\n", rsk_b);

    float rsk_f_num = 3012.1295f;
    printf("Floating point number with just %%f 'f_num' = %f\n", rsk_f_num);
    printf("Floating point number with %%4.2f 'f_num' = %4.2f\n", rsk_f_num);
    printf("Floating point number with %%6.5f 'f_num' = %6.5f\n\n", rsk_f_num);

    double rsk_d_pi = 3.14159265358979323846;
    printf("Double precision floating point number without Exponential = %g\n", rsk_d_pi);
    printf("Double precision floating point number with Exponential(lower case) = %e\n", rsk_d_pi);
    printf("Double precision floating point number with Exponential(upper case) = %E\n", rsk_d_pi);
    printf("Double precision floating point number in Hexadecimal(lower case) = %a\n", rsk_d_pi);
    printf("Double precision floating point number in Hexadecimal(upper case) = %A\n\n", rsk_d_pi);

    printf("**************************************************************************************************************\n");  
    printf("\n\n");

    return(0);
}