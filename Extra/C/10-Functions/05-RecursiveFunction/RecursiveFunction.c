#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {

    unsigned int rsk_num;

    void recursive(unsigned int);

    printf("\n\n");
    printf("Enter Any Number : ");
    scanf("%d", &rsk_num);
    printf("\n\n");

    printf("Output Of Recursive Function : \n\n");
    
    recursive(rsk_num);

    printf("\n\n");

    return(0);
}

void recursive(unsigned int rsk_n) {

    printf("N = %d\n",rsk_n);

    if( rsk_n > 0) {
        recursive(rsk_n-1);

    }

}
