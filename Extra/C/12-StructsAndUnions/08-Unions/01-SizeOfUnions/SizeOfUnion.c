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
    union MyUnion u;

    printf("\n\n");
    printf("Size Of MyStruct = %lu", sizeof(s));
    printf("\n\n");
    printf("Size Of MyUnion = %lu", sizeof(u));
    printf("\n\n");
    
    return(0);
}