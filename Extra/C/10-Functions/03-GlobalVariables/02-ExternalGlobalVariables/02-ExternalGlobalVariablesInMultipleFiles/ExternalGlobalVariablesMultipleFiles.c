#include <stdio.h>

int rsk_global_count = 0;

int main(void) {

    void change_count(); // in current file
    void change_count_one(); // in File_01.c
    void change_count_two(); // in File_02.c

    printf("\n\n");

    change_count();
    change_count_one();
    change_count_two();

    return(0);
}

void change_count() {

    rsk_global_count = rsk_global_count + 1;
    printf("Global Count = %d\n", rsk_global_count);
}
