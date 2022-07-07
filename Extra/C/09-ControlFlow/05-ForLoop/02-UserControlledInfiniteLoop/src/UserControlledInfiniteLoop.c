#include <stdio.h>
#include <conio.h>

int main(void) {

    char rsk_option, rsk_ch = '\0';

    printf("\n\n");

    printf("Once The Infinite Loop Begins, Enter 'Q' or 'q' To quit The Infinite Loop: \n\n");

    rsk_option = getch();
    if(rsk_option == 'Y' || rsk_option == 'y'){

        for(;;){
            printf("In Loop...\n");
            rsk_ch = getch();

            if(rsk_ch == 'Q' || rsk_ch == 'q') {
                break;
            }
        }
    }
    printf("\n\n");
    printf("Exitting User Controlled Infinite Loop...");
    printf("\n\n");

    return(0);

}
