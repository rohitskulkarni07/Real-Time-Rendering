#include <stdio.h>

struct MyData {
    int i;
    float f;
    double d;
    char c;
};


int main(void) {

    struct MyData data;

    printf("\n\n");

    printf("Enter Integer i DATA MEMBERS OF 'struct MyData data' \n\n");
    scanf("%d", &data.i);
    
    printf("Enter Float f DATA MEMBERS OF 'struct MyData data' \n\n");
    scanf("%f", &data.f);
    
    printf("Enter Double d DATA MEMBERS OF 'struct MyData data' \n\n");
    scanf("%lf", &data.d);
    
    printf("Enter Character c DATA MEMBERS OF 'struct MyData data' \n\n");
    data.c = getch();
    
    printf("DATA MEMBERS OF 'struct MyData' ARE : \n\n");

    printf("i = %d\n", data.i);
    printf("f = %f\n", data.f);
    printf("d = %lf\n", data.d);
    printf("c = %c\n", data.c);
    
    printf("\n\n");
    return(0);    
}

