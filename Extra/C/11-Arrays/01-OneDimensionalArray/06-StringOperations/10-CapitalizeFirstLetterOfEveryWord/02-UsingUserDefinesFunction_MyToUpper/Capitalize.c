#include <stdio.h>

#define MAX_STRING_LENGTH 512
#define SPACE ' '
#define FULLSTOP '.'
#define COMMA ','
#define EXCLAMAITION '!'
#define QUESTION_MARK '?'

int main(void) {

    int MyStrlen(char[]);
    char MyToUpper(char);

    char rsk_chArray[MAX_STRING_LENGTH], rsk_chArrayCapitalizedLetterOfEveryWord[MAX_STRING_LENGTH];
    int  rsk_iStringLength;
    int  i, j;

    printf("\n\n");

    printf("Enter a String: \n\n");
    gets_s(rsk_chArray, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Entered String By You Is : \n\n");
    printf("%s\n", rsk_chArray);
    
    rsk_iStringLength = MyStrlen(rsk_chArray);

    j = 0;
    for(i = 0; i < rsk_iStringLength; i++) {

        if(i==0) {
            rsk_chArrayCapitalizedLetterOfEveryWord[j] = MyToUpper(rsk_chArray[i]);
            
        } else if(rsk_chArray[i] == ' ') {
            rsk_chArrayCapitalizedLetterOfEveryWord[j] = rsk_chArray[i];
            rsk_chArrayCapitalizedLetterOfEveryWord[j+1] = MyToUpper(rsk_chArray[i+1]);
            i++;
            j++;
        } else if((rsk_chArray[i] == FULLSTOP || rsk_chArray[i] ==COMMA || rsk_chArray[i] == EXCLAMAITION || rsk_chArray[i] ==QUESTION_MARK) && (rsk_chArray[i] != SPACE)) {
            rsk_chArrayCapitalizedLetterOfEveryWord[j] = rsk_chArray[i];
            rsk_chArrayCapitalizedLetterOfEveryWord[j+1] = SPACE;
            rsk_chArrayCapitalizedLetterOfEveryWord[j] = MyToUpper(rsk_chArray[i+1]);

            j = j + 2;
            i++;
        } else {
            rsk_chArrayCapitalizedLetterOfEveryWord[j] = rsk_chArray[i];

        }
        j++;
    }

    rsk_chArrayCapitalizedLetterOfEveryWord[j] = '\0';


    printf("\n\n");
    printf("String After Capitalizing First Letter Each Word = %s\n\n", rsk_chArrayCapitalizedLetterOfEveryWord);
    printf("\n\n");

    return(0);

}

int MyStrlen(char str[]) {

    int i = 0,len = 0;

    while(str[i] != '\0') {
        len++;
        i++;
    }

    return len;
}

char MyToUpper(char ch) {
    int num;
    int c;

    num = 'a' - 'A';

    if((int)ch >= 97 && (int)ch <=122) {

        c = (int) ch - num;
        return((char)c);

    } else {

        return ch;

    }
}
