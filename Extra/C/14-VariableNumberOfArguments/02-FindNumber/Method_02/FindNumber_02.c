#include <stdio.h>
#include <stdarg.h>

#define NUM_TO_BE_FOUND 3
#define NUM_ELEMENTS 10

int main(void) {

    void FindNumber(int, int, ...);

    printf("\n\n");

    FindNumber(NUM_TO_BE_FOUND, NUM_ELEMENTS, 3, 5, 9, 2, 3, 6, 9, 3, 1, 3);

    return(0);
}



void FindNumber(int num_to_be_found, int num, ...) {

    int va_FindNumber(int, int, va_list);

    int count = 0;
    int n;
    va_list number_list;

    va_start(number_list, num);

    count = va_FindNumber(num_to_be_found, num, number_list);

    if(count == 0) {

        printf("NUmber %d Could Not Be Found !!!\n\n", num_to_be_found);
    } else {
        printf("Number %d Found %d Times !!!\n\n", num_to_be_found, count);

    }

    va_end(number_list);
}

int va_FindNumber(int num_to_be_found, int num, va_list list) {

    int count_of_num = 0;
    int n;

    while(num) {
        n = va_arg(list, int);
        if(n == num_to_be_found) {
            count_of_num++;
        }
        num--;
    }
    return(count_of_num);
}
