#include <stdio.h>

struct MyStruct {
    int rsk_i;
    float rsk_f;
    double rsk_lf;
    char rsk_c;
};

int main(void) {

    struct MyStruct s;

    printf("\n\n");
    printf("Size Of MyStruct s = %lu", sizeof(s));
    printf("\n\n");

    s.rsk_i = 1;
    s.rsk_f = 6.66;
    s.rsk_lf = 2.4736266;
    s.rsk_c = 'A';

    printf("***** Members Of Struct Are *****\n\n");

    printf("s.i  = %d \n\n", s.rsk_i);
    printf("s.f  = %f \n\n", s.rsk_f);
    printf("s.lf = %lf\n\n", s.rsk_lf);
    printf("s.c  = %c \n\n", s.rsk_c);

    printf("Address of The Members of the Struct Are : \n\n");

    printf("s.i  = %p \n\n", &s.rsk_i);
    printf("s.f  = %p \n\n", &s.rsk_f);
    printf("s.lf = %p \n\n", &s.rsk_lf);
    printf("s.c  = %p \n\n", &s.rsk_c);

    printf("MyStruct s = %p\n\n", &s);

    return(0);
}
