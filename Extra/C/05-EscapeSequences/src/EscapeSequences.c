#include <stdio.h>

int main() {

    printf("\n\n");

    printf("Going on to the next line ....using \\n Escape Sequence\n\n");
    printf("Demonstrating \t Horizontal \t Tab \t Usnig  \t \\t Escape sequence!!!\n\n");
    printf("\"This is a Double Quoted output\" done using \\\" \\\" Escape sequence!!!\n\n");
    printf("\'This is a SIngle Quoted output\' done using \\\' \\\' Escape sequence!!!\n\n");

    printf("\r Demonstrating Carriage Return Using \\r Escape sequence!!!\n");
    printf("Demonstrating \r Carriage Return Using \\r Escape sequence!!!\n");
    printf("Demonstrating Carriage \r Return Using \\r Escape sequence!!!\n\n");

    printf("Demonstrating \x41 Using \\x-- Escape sequence!!!\n\n"); //0x41 is hexadecimal number for 'A' | x-- is placeholder for x followed by 2 digit
    printf("Demonstrating \102 Using \\ooo Escape sequence!!!\n\n"); //102 is octal for 'B' | ooo  is place holeder for 3 digits

    return(0);
}