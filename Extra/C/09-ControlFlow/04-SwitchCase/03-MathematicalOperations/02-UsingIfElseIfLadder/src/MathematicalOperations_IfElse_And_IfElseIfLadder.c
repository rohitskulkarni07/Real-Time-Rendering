#include <stdio.h>
#include <conio.h> //for  getch()

int main(void) {
    int rsk_a,rsk_b;
    int rsk_result;
    char rsk_option, rsk_option_division;

    printf("\n\n");

    printf("Enter Value For 'A' :");
    scanf("%d", &rsk_a);

    printf("Enter Value For 'B' :");
    scanf("%d", &rsk_b);

    printf("Enter OPtion in Charachter:\n\n");
    printf("'A' or 'a' for Addition\n");
    printf("'S' or  's' for Subtraction\n");
    printf("'M' or  'm' for Multiplication\n");
    printf("'D' or  'd' for Divison\n\n");

    printf("Enter OPtion: ");
    rsk_option = getch();

    printf("\n\n");

    if(rsk_option == 'A' || rsk_option == 'a')  {

        rsk_result = rsk_a + rsk_b;
        printf("Addition Of A = %d And B = %d Gives Result %d", rsk_a, rsk_b, rsk_result);

    } else if( rsk_option == 'S' || rsk_option == 's') {
        
        if(rsk_a >= rsk_b) {
        
            rsk_result = rsk_a - rsk_b;
            printf("Subtraction Of B = %d From A = %d Gives Result %d", rsk_b, rsk_a, rsk_result);
        } else {
        
            rsk_result = rsk_b - rsk_a;
            printf("Subtraction Of A = %d From B = %d Gives Result %d", rsk_a, rsk_b, rsk_result);
        }

    } else if( rsk_option == 'M' || rsk_option == 'm') {       
        
        rsk_result = rsk_a * rsk_b;
        printf("Multiplication Of A = %d And B = %d Gives Result %d", rsk_a, rsk_b, rsk_result);

    } else if( rsk_option == 'D' || rsk_option == 'd') {
        
        printf("ENter Option In Charachter\n\n");
        printf("'Q' or 'q' or '/' for Quotient Upon Division: \n");
        printf("'R' or 'r' or '%' for Remainder Upon Division: \n");
        printf("Enter Option : ");
            
        rsk_option_division = getch();

        if(rsk_option_division == 'Q'  || rsk_option_division == 'q' ||rsk_option_division == '/') {

            if(rsk_a >= rsk_b) {
        
                rsk_result = rsk_a / rsk_b;
                printf("Divison Of A %d By %d Gives Result %d\n", rsk_a, rsk_b, rsk_result);
            } else {
        
                rsk_result = rsk_b / rsk_a;
                printf("Divison Of A %d By %d Gives Result %d\n", rsk_a, rsk_b, rsk_result);
            }
        } else if(rsk_option_division == 'R'  || rsk_option_division == 'r' ||rsk_option_division == '%') {
         
            if(rsk_a >= rsk_b) {
         
                rsk_result = rsk_a % rsk_b;
                printf("Divison Of A %d By %d Gives Remainder %d\n", rsk_a, rsk_b, rsk_result);
            } else {
         
                rsk_result = rsk_b % rsk_a;
                printf("Divison Of A %d By %d Gives Remainder %d\n", rsk_a, rsk_b, rsk_result);
            }
        } else {
        
            printf("Invalid Character /'%c/' Entered For Division !!! Please Try Again...\n\n", rsk_option_division);
        }

    
    } else {
        
        printf("Invalid Character /'%c/' Entered!!! Please Try Again...\n\n", rsk_option);
    }

    printf("If-Else and If-Else-If Ladder Complete!!!\n\n");
    
    return(0);

}