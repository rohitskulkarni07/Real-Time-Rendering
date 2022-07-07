#include <stdio.h>
#include <stdlib.h>

int main(void) {

    void MyAlloc(int**, unsigned int);

    int *piArray = NULL;
    unsigned int num_elements;
    int i;

    printf("\n\n");
    printf("How Many Elements You Want In Integer Array ? \n\n");
    scanf("%u", &num_elements);

    printf("\n\n");
    MyAlloc(&piArray, num_elements);

    printf("Enter %u Elements To Fill Up Your Integer Array : \n\n",num_elements);

    for(i = 0; i < num_elements; i++) {
        scanf("%d", &piArray[i]);
    }

    printf("\n\n");
    printf("The %u Elements Entered By You In The Integer Array : \n\n", num_elements);
    printf("[ ");
    for(i = 0; i < num_elements; i++) {
        printf("%u, ",*(piArray + i));
    }
    printf("\b\b ]\n\n");

    return(0);
}

void MyAlloc(int **ptr, unsigned int numberOfElements) {

    *ptr = (int *)malloc(numberOfElements * sizeof(int ));

    if(*ptr == NULL) {

        printf("Could Not Allocate Memory!!! Exiting Now...\n\n");
        exit(0);
    }

    printf("MyAlloc() Has Successfully Allocated %lu Bytes For Integer Array!!!\n\n", (numberOfElements * sizeof(int)));

}