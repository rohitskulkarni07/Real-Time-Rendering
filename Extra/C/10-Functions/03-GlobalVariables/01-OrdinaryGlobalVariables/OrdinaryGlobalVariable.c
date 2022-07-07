#include <stdio.h>

//Global Scope
int rsk_global_variable = 0;

int main(void) {
    //local scope of main starts here

    void change_count_one(void);
    void change_count_two(void);
    void change_count_three(void);

    change_count_one();

    change_count_two();

    change_count_three();

    return(0);
    //local scope of main ends here

}

//Global Scope

void change_count_one(void) {
    //local scope starts
    rsk_global_variable = 100;

    printf("Function \"change_count_one\"   : Global Count %d\n", rsk_global_variable);
    //local scope ends.

}

void change_count_two(void) {
    //local scope starts

    rsk_global_variable = rsk_global_variable + 1;

    printf("Function \"change_count_two\"   : Global Count %d\n", rsk_global_variable);
    //local scope ends.

}

void change_count_three(void) {
    //local scope starts
    rsk_global_variable = rsk_global_variable + 10;

    printf("Function \"change_count_three\" : Global Count %d\n", rsk_global_variable);
    //local scope ends.

}
//Global Scope
