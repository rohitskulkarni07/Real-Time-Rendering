#include <stdio.h>

struct MyData {
    int i;
    float f;
    double d;
    char c;
} data = {30, 4.45f, 11.451995, 'A'};


int main(void) {


    struct MyData data1 = {30, 4.45f, 11.451996, 'A'};
    struct MyData data2 = {31, 5.56f, 22.561997, 'B'};
    struct MyData data3 = {32, 6.78f, 33.671998, 'C'};
    struct MyData data4 = {33, 7.89f, 44.781999, 'D'};
  
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data1' ARE : \n\n");

    printf("i = %d\n", data1.i);
    printf("f = %f\n", data1.f);
    printf("d = %lf\n", data1.d);
    printf("c = %c\n", data1.c);

    printf("\n\n");
        
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data2' ARE : \n\n");

    printf("i = %d\n", data2.i);
    printf("f = %f\n", data2.f);
    printf("d = %lf\n", data2.d);
    printf("c = %c\n", data2.c);

    printf("\n\n");

    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data3' ARE : \n\n");

    printf("i = %d\n", data3.i);
    printf("f = %f\n", data3.f);
    printf("d = %lf\n", data3.d);
    printf("c = %c\n", data3.c);

    printf("\n\n");
    
    printf("\n\n");
    printf("DATA MEMBERS OF 'struct MyData data4' ARE : \n\n");

    printf("i = %d\n", data4.i);
    printf("f = %f\n", data4.f);
    printf("d = %lf\n", data4.d);
    printf("c = %c\n", data4.c);

    printf("\n\n");

    return(0);    
}

