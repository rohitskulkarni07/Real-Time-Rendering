#include <stdio.h>

struct MyData {

    int *ptr_i;
    int i;

    float *ptr_f;
    float f; 

    double *ptr_d;
    double d;
};

int main(void) {

    struct  MyData data;

    data.i = 30;
    data.ptr_i = &data.i;

    data.f = 11.45f;
    data.ptr_f = &data.f;
    
    data.d = 1.29995;
    data.ptr_d = &data.d;

    printf("\n\n");
    printf("i = %d\n", *(data.ptr_i));
    printf("Address Of 'i' = %p \n",data.ptr_i);

    printf("\n\n");
    printf("f = %f\n", *(data.ptr_f));
    printf("Address Of 'f' = %p \n",data.ptr_f);

    printf("\n\n");
    printf("d = %lf\n", *(data.ptr_d));
    printf("Address Of 'd' = %p \n",data.ptr_d);

    return(0);

}

