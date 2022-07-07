#include <stdio.h>

int main(void) {

    int rsk_age;

    printf("\n\n");

    printf("Enter Your Age( > 0): ");
    scanf("%d",&rsk_age);
    printf("\n\n");

    if(rsk_age >= 18) {
        printf("ENtering in If Block\n\n");
        printf("You Are Eligible For Voting!!!\n\n");

    } else {
        printf("Entring in Else Block\n\n");
        printf("You Are NOT Eligible For Voting!!!\n\n");

     }

     printf("Byee!!!\n\n");
    
    return (0);
}