#include <stdio.h>
#include <conio.h>

int main(void) {

    int rsk_option;
    int rsk_ch;

    printf("\n\n");
    printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quit The Infinite For Loop : \n\n");

    printf("ENter 'Y' or 'y' to Initiate User Cotrolled Infinite Loop : ");
    printf("\n\n");

    rsk_option = getch();

    printf("\n\n");

    if(rsk_option == 'Y' || rsk_option == 'y') {

        while(1) {

            printf("In Loop");
            rsk_ch = getch();

            if(rsk_ch == 'Q' || rsk_ch =='q'){
              
                printf("\n\n");
                printf("Exitting User Controlled Infinite Loop...");
                printf("\n\n");

                break;

            }
           
        }
    } else {
        printf("You Must Press 'Y or 'y' to Initiate the User Controlled Infinite Loop...Please Try Again...\n\n");
    }

    return(0);


}