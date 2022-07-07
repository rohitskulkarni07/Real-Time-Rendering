#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main(void) {

    int MyStrlen(char[]);

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
            rsk_chArrayCapitalizedLetterOfEveryWord[j] = toupper(rsk_chArray[i]);
            
        } else if(rsk_chArray[i] == ' ') {
            rsk_chArrayCapitalizedLetterOfEveryWord[j] = rsk_chArray[i];
            rsk_chArrayCapitalizedLetterOfEveryWord[j+1] = toupper(rsk_chArray[i+1]);
            i++;
            j++;
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
