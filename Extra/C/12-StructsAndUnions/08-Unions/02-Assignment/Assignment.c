#include <stdio.h>

union MyUnion {
    int rsk_i;
    float rsk_f;
    double rsk_lf;
    char rsk_c;
};

int main(void) {

    union MyUnion u1,u2;

    printf("\n\n");
    printf("Size Of MyUnion u1 = %lu", sizeof(u1));
    printf("\n\n");
    printf("Size Of MyUnion u2 = %lu", sizeof(u2));
    printf("\n\n");

    printf("Members Of Union Are : \n\n");
    
    u1.rsk_i = 1;
    u1.rsk_f = 6.66;
    u1.rsk_lf = 2.4736266;
    u1.rsk_c = 'A';
    
    printf("u1.i  = %d \n\n", u1.rsk_i);
    printf("u1.f  = %f \n\n", u1.rsk_f);
    printf("u1.lf = %lf\n\n", u1.rsk_lf);
    printf("u1.c  = %c \n\n", u1.rsk_c);

    printf("Address of The Members of the Union u1\n\n");

    printf("u1.i  = %p \n\n", &u1.rsk_i);
    printf("u1.f  = %p \n\n", &u1.rsk_f);
    printf("u1.lf = %p \n\n", &u1.rsk_lf);
    printf("u1.c  = %p \n\n", &u1.rsk_c);

    printf("MyUnion u1 = %p\n\n", &u1);

    printf("\n\n");
    printf("Members Of Union u2 Are : \n\n");

    u2.rsk_i = 2;
    printf("u2.i  = %d \n\n", u2.rsk_i);
    
    u2.rsk_f = 4.26;
    printf("u2.f  = %f \n\n", u2.rsk_f);
    
    u2.rsk_lf = 5.9136427;
    printf("u2.lf = %lf\n\n", u2.rsk_lf);
    
    u2.rsk_c = 'Z';
    printf("u2.c  = %c \n\n", u2.rsk_c);
    
    printf("Address of The Members of the Union u2\n\n");

    printf("u2.i  = %p \n\n", &u2.rsk_i);
    printf("u2.f  = %p \n\n", &u2.rsk_f);
    printf("u2.lf = %p \n\n", &u2.rsk_lf);
    printf("u2.c  = %p \n\n", &u2.rsk_c);

    printf("MyUnion u2 = %p\n\n", &u2);

    printf("\n\n");
    
    return(0);
}