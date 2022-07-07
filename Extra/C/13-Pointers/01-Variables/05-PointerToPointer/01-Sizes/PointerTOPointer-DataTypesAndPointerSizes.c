#include <stdio.h>

struct Employee{
    char name[100];
    int age;
    float salary;
    char sex;
    char marital_status;
};


int main(void) {

    printf("\n\n");

    printf("SIZES OF DATA TYPES OF POINTER AND POINTERS TO THOSE RESPECTIVE DATA TYPES ARE : \n\n");

    printf("Size Of (int)    %d\tSize Of Pointer To int (int*)       %d\tSize Of Pointer To Pointer To int (int**)       %d\n\n", sizeof(int), sizeof(int*), sizeof(int**));

    printf("Size Of (float)  %d\tSize Of Pointer To float (float*)   %d\tSize Of Pointer To Pointer To float (float**)   %d\n\n", sizeof(float), sizeof(float*), sizeof(float**));

    printf("Size Of (double) %d\tSize Of Pointer To double (double*) %d\tSize Of Pointer To Pointer To double (double**) %d\n\n", sizeof(double), sizeof(double*), sizeof(double**));

    printf("Size Of (char )  %d\tSize Of Pointer To char (char*)     %d\tSize Of Pointer To Pointer To char (char**)     %d\n\n", sizeof(char), sizeof(char*), sizeof(char**));

    printf("Size Of (struct Employee ) %d\tSize Of Pointer To struct Employee (struct Employee*)  %d\tSize Of Pointer To Pointer To struct Employee (struct Employee**) %d\n\n", sizeof(struct Employee), sizeof(struct Employee*), sizeof(struct Employee**));


    return(0);
}