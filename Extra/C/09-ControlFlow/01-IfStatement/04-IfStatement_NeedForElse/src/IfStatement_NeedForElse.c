#include <stdio.h>

int main(void) {

    int rsk_age;

    printf("\n\n");

    printf("Enter Your Age( > 0): ");
    scanf("%d",&rsk_age);

    if(rsk_age >= 18) {
        
        printf("You Are Eligible For Voting!!!\n\n");
    }

    printf("You Are NOT Eligible For Voting!!!\n\n");

    return (0);
}