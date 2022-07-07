#include <stdio.h>

int main(void) {

    void change_count(void);

    extern int rsk_global_count;

    printf("\n");
    printf("Value Of global_count before change_count() = %d\n", rsk_global_count);
    change_count();
    printf("Value Of global_count after change_count() = %d\n", rsk_global_count);
    printf("\n");

    return(0);

}

int rsk_global_count = 0;

void change_count() {

    rsk_global_count = 5;
    printf("Values of  global_count() in change_count() = %d\n", rsk_global_count);

}