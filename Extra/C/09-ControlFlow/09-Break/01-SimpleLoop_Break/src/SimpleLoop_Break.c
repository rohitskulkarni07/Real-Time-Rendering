#include <stdio.h>
#include <conio.h>

int main(void) {

    int rsk_i;
    char rsk_ch;

    printf("\n\n");

    printf("Printing Even Nu,bers 1 To 100 For Every User Input. Exitting THe Loop When User Charachter 'Q' or 'q' : \n\n");
    printf("ENter Character 'Q' or 'q' to Exit The Loop");

    for(rsk_i = 0; rsk_i<=100; rsk_i++ ) {
        printf("\t%d\n",rsk_i);
        rsk_ch = getch();

        if(rsk_ch == 'Q' || rsk_ch == 'q') {
            break;
        }
    }

    printf("\n\n");
    printf("Exitting Loop...");
    printf("\n\n");

    return(0);
}