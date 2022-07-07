#include <stdio.h>

//Global Scope

int main(void) {
    //local scope of main starts here

    int rsk_a = 5;

    void change_count(void);

    printf("\n");

    printf("A = %d\n\n",rsk_a);

    //local_count initialize
    change_count();

    // local variable is can hold or sustain its privous value when called by same fucntion
    change_count();

    // local variable is can hold or sustain its privous value when called by same fucntion
    
    change_count();

    return(0);
    //local scope of main ends here

}

//Global Scope

void change_count(void) {
    //local scope starts
    static int rsk_local_variable = 0;

    rsk_local_variable = rsk_local_variable + 1;

    printf("Local Count %d\n",  rsk_local_variable);
    //local scope ends.

}

//Global Scope
