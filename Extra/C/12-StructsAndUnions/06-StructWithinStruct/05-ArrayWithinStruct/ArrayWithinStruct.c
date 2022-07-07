#include <stdio.h>

struct MyNumber {
    int rsk_num;
    int rsk_num_table[10];
};

struct NumTables {
    struct MyNumber rsk_a;
    struct MyNumber rsk_b;
    struct MyNumber rsk_c;
};

int main(void) {

    struct NumTables tables;

    int i;//iterator

//_______table of 2__________________________________________________
    tables.rsk_a.rsk_num = 2;
    for(i = 0; i < 10; i++) {
        tables.rsk_a.rsk_num_table[i] = tables.rsk_a.rsk_num * (i+1);
    }
    printf("\n\n");
    printf("Table Of %d : \n\n", tables.rsk_a.rsk_num);
    for (i = 0; i < 10; i++) {
        printf("%d * %d = %d\n",tables.rsk_a.rsk_num, i+1, tables.rsk_a.rsk_num_table[i]);
    }
    
//________table of 3_________________________________________________

    tables.rsk_b.rsk_num = 3;
    for(i = 0; i < 10; i++) {
        tables.rsk_b.rsk_num_table[i] = tables.rsk_b.rsk_num * (i+1);
    }
    printf("\n\n");
    printf("Table Of %d : \n\n", tables.rsk_b.rsk_num);
    for (i = 0; i < 10; i++) {
        printf("%d * %d = %d\n",tables.rsk_b.rsk_num, i+1, tables.rsk_b.rsk_num_table[i]);
    }   

//____________table of 4_____________________________________________


    tables.rsk_c.rsk_num = 4;
    for(i = 0; i < 10; i++) {
        tables.rsk_c.rsk_num_table[i] = tables.rsk_c.rsk_num * (i+1);
    }
    printf("\n\n");
    printf("Table Of %d : \n\n", tables.rsk_c.rsk_num);
    for (i = 0; i < 10; i++) {
        printf("%d * %d = %d\n",tables.rsk_c.rsk_num, i+1, tables.rsk_c.rsk_num_table[i]);
    }

    return(0);
}