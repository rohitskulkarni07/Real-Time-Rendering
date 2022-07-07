#include <stdio.h>

struct Employee {

    char name[100];
    int  age;
    float salary;
    char sex;
    char martial_status;
};

int main(void) {

    printf("\n\n");
    printf("SIZES OF DATA TYPES AND POINTERS TO THOSE RESPECTIVE DATA TYPES ARE :\n\n");

    printf("Size Of (int)               %d\t\t\tSize Of Pointer to int (int*)                  %d\n\n",sizeof(int ), sizeof(int*));
    printf("Size Of (float)             %d\t\t\tSize Of Pointer to float (float*)              %d\n\n",sizeof(float ), sizeof(float*));
    printf("Size Of (double)            %d\t\t\tSize Of Pointer to double (double*)            %d\n\n",sizeof(double ), sizeof(double*));
    printf("Size Of (char)              %d\t\t\tSize Of Pointer to char (char*)                %d\n\n",sizeof(char ), sizeof(char*));
    printf("Size Of (struct Employee)   %d\t\t\tSize Of Pointer to struct (struct Employee*)   %d\n\n",sizeof(struct Employee ), sizeof(struct Employee*));

    return(0);

}
