#include <stdio.h>

#define MAX_STRING_LENGTH 512

int main() {

    int  MyStrlen(char[]);
    void MyStrcpy(char[],char[]);

    char rsk_chArray[MAX_STRING_LENGTH];
    int  rsk_iStringLength;
    int  count_A = 0, count_E = 0, count_I = 0, count_O = 0, count_U = 0;
    int  i;

    printf("\n\n");

    printf("ENter String: \n\n");
    gets_s(rsk_chArray, MAX_STRING_LENGTH);

    printf("\n\n");

    printf("Enter A String By You Is : \n\n");
    printf("%s\n", rsk_chArray);

    rsk_iStringLength = MyStrlen(rsk_chArray);

    for(i = 0; i < rsk_iStringLength; i++) {

        switch (rsk_chArray[i]) {
            
            case 'A':
            case 'a':
                count_A++;
                break;
            
            case 'E':
            case 'e':
                count_E++;
                break;
            
            case 'I':
            case 'i':
                count_I++;
                break;
            
            case 'O':
            case 'o':
                count_O++;
                break;
            
            case 'U':
            case 'u':
                count_U++;
                break;

            default:
                break;
        }
    }

    printf("\n\n");
    
    printf("Vowels And Their Number Of Occurences are As Follows : \n\n");
    
    printf("'A' Has Occured = %d Times !!!\n\n", count_A);
    printf("'E' Has Occured = %d Times !!!\n\n", count_E);
    printf("'I' Has Occured = %d Times !!!\n\n", count_I);
    printf("'O' Has Occured = %d Times !!!\n\n", count_O);
    printf("'U' Has Occured = %d Times !!!\n\n", count_U);

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
