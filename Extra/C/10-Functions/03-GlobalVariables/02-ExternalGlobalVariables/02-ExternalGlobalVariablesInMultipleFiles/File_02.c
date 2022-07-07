#include <stdio.h>

void change_count_two() {

    extern int rsk_global_count;
    rsk_global_count = rsk_global_count + 1;
    printf("change_count_two() File_02.c : Global Count = %d\n", rsk_global_count);
}
