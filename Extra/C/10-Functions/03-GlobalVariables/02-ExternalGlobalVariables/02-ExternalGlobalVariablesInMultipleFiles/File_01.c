#include <stdio.h>

extern int rsk_global_count;

void change_count_one() {

    rsk_global_count = rsk_global_count + 1;
    printf("change_count_one() File_01.c : Global Count = %d\n", rsk_global_count);
}
