#include <stdio.h>

int main(void) {

    struct MyData {
    
        int i;
        float f;
        double d;
        char c;
    
    } data = {30, 4.45f, 11.451995, 'A'};

    printf("\n\n");

    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");

    printf("i = %d\n", data.i);
    printf("f = %f\n", data.f);
    printf("d = %lf\n", data.d);
    printf("c = %c\n", data.c);

    printf("\n\n");


    return(0);    
}

