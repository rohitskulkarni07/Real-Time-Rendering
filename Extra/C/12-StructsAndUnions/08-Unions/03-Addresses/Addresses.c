#include <stdio.h>

struct MyStruct {
    int rsk_i;
    float rsk_f;
    double rsk_lf;
    char rsk_c;
};

union MyUnion {
    int rsk_i;
    float rsk_f;
    double rsk_lf;
    char rsk_c;
};

int main(void) {

    struct MyStruct s;
    union  MyUnion  u;

    printf("\n\n");
    printf("Size Of MyUnion u = %lu", sizeof(u));
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

    printf("\n\n");
    printf("***** Members Of Union Are ***** \n\n");

    u.rsk_i = 2;
    printf("u.i  = %d \n\n", u.rsk_i);
    
    u.rsk_f = 4.26;
    printf("u.f  = %f \n\n", u.rsk_f);
    
    u.rsk_lf = 5.9136427;
    printf("u.lf = %lf\n\n", u.rsk_lf);
    
    u.rsk_c = 'Z';
    printf("u.c  = %c \n\n", u.rsk_c);
    
    printf("Address of The Members of the Union u2\n\n");

    printf("u.i  = %p \n\n", &u.rsk_i);
    printf("u.f  = %p \n\n", &u.rsk_f);
    printf("u.lf = %p \n\n", &u.rsk_lf);
    printf("u.c  = %p \n\n", &u.rsk_c);

    printf("MyUnion u = %p\n\n", &u);

    return(0);
}
