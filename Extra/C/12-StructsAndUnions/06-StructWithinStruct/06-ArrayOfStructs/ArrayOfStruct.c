#include <stdio.h>

struct MyNumber {
    int rsk_num;
    int rsk_num_table[10];

};

struct NumTables {
    struct MyNumber rsk_n;
};

int main(void) {

    struct NumTables table[10];
    int i,j;

    for(i = 0; i < 10; i++) {
        table[i].rsk_n.rsk_num = (i + 1);
    }

    for( i = 0; i < 10; i++) {
        printf("\n\n");
        printf("Table Of %d : \n\n", table[i].rsk_n.rsk_num);
        
        for ( j = 0; j < 10; j++) {
            
            table[i].rsk_n.rsk_num_table[j] = table[i].rsk_n.rsk_num * (j+1);
            printf("%d * %d = %d\n", table[i].rsk_n.rsk_num, (j+1), table[i].rsk_n.rsk_num_table[j]); 
        }
        
    }

    return(0);
}

