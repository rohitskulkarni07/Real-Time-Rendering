#include <stdio.h>
#include <conio.h>

int main(void) {

    int rsk_option;
    int rsk_ch;

    printf("\n\n");
    printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To Quit The Infinite For Loop : \n\n");

    printf("ENter 'Y' or 'y' to Initiate User Cotrolled Infinite Loop : ");
    printf("\n\n");


    do {

        do {

            printf("\n");
            printf("In Loop");
            rsk_ch = getch();

        }while(rsk_ch == 'Q' || rsk_ch =='q');
              
            printf("\n\n");
            printf("Exitting User Controlled Infinite Loop...");
            printf("\n\n");
            printf("Do You Want To Beigin User Controlled Infinite Loop Again, Enter 'Y' or 'y' to Initiate User Cotrolled Infinite Loop or 'N' to to Stop : ");

        rsk_option = getch();

    }while(rsk_option == 'Y' || rsk_option == 'y');

    return(0);


}