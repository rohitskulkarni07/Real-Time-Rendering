#include <stdio.h>
#include <conio.h>

#define CHAR_ALPHABET_UPPER_CASE_BEGINING 65 // asci  for 'A'
#define CHAR_ALPHABET_UPPER_CASE_ENDING 90   // asci for 'Z'


#define CHAR_ALPHABET_LOWER_CASE_BEGINING 91 // asci  for 'a'
#define CHAR_ALPHABET_LOWER_CASE_ENDING 122   // asci for 'z'

#define CHAR_DIGIT_BEGINING 48 //ascc for '0'
#define CHAR_DIGIT_ENDING 57 //ascc for '9'

int main(void) {

    char rsk_ch;
    int rsk_ch_value;

    printf("\n\n");

    printf("ENter Charachter : ");
    rsk_ch = getch();

    printf("\n\n");

    if((rsk_ch == 'A' || rsk_ch== 'a') || (rsk_ch == 'E' || rsk_ch== 'e') || (rsk_ch == 'I' || rsk_ch== 'i') || (rsk_ch == 'O' || rsk_ch== 'o') || (rsk_ch == 'U' || rsk_ch== 'u')) {
            printf("Charachter \'%c\' Entered By You Is A VOWEL CHARACTER from English Alphabet!!! \n\n",rsk_ch);
    } else {
        rsk_ch_value = (int )rsk_ch;
           
           if((rsk_ch_value > CHAR_ALPHABET_UPPER_CASE_BEGINING && rsk_ch_value <=CHAR_ALPHABET_UPPER_CASE_ENDING) || (rsk_ch_value >= CHAR_ALPHABET_LOWER_CASE_BEGINING && rsk_ch_value <= CHAR_ALPHABET_LOWER_CASE_ENDING)) {
           
                printf("Charachter \'%c\' Entered By You Is A CONSONANT CHARACHTER from English Alphabet!!! \n\n",rsk_ch);
                
            } else if(rsk_ch_value >= CHAR_DIGIT_BEGINING && rsk_ch_value <= CHAR_DIGIT_ENDING) {
            
                printf("Charachter \'%c\' Entered By You Is A DIGIT CHARACTER !!! \n\n",rsk_ch);

            } else {
            
                printf("Charachter \'%c\' Entered By You Is A SPECIAL CHARACTER !!! \n\n",rsk_ch);

            } 
    }

    return(0);

}