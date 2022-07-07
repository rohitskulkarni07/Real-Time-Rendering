#include <stdio.h>

#define RSK_PI 3.1415926535897932

#define AMC_STRING "Astromedicomp RTR 3.0"

enum {
    SUNDAY,     //0
    MONDAY,     //1
    TUESDAY,    //2
    WEDNESDAY,  //3
    THURSDAY,   //4
    FRIDAY,     //5
    SATURDAY //no comma //6
};  //semicolon

enum {
    JANUARY = 1, // 1
    FEBRUARY,    // 2
    MARCH,       // 3
    APRIL,       // 4
    MAY,         // 5
    JUNE,        // 6
    JULY,        // 7
    AUGUST,      // 8
    SEPTEMBER,   // 9
    OCTOMBER,    // 10
    NOVEMBER,    // 11
    DECEMBER     // 12
};

enum rsk_Numbers {
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE =5 ,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN

};

enum rsk_boolean {
    FALSE, //or FALSE = 0;
    TRUE    // and TRUE = 1; Is also valid or vice versa 
};

int main() {

    //local constant decleraions
    const double rsk_epsilon = 0.000001;

    printf("\n\n");
    printf("Local Constant Epsilon = %lf\n\n", rsk_epsilon);

    printf("SUNDAY Is Day Number   : %d\n",SUNDAY);
    printf("MONDAY Is Day Number   : %d\n",MONDAY);
    printf("TUESDAY Is Day Number  : %d\n",TUESDAY);
    printf("WEDNESDAY Is Day Number: %d\n",WEDNESDAY);
    printf("THURSDAY Is Day Number : %d\n",THURSDAY);
    printf("FRIDAY Is Day Number   : %d\n",FRIDAY);
    printf("SATURDAY Is Day Number : %d\n\n",SATURDAY);


    printf("ONE Is Enum Number    : %d\n", ONE);
    printf("TWO Is Enum Number    : %d\n", TWO);
    printf("THREE Is Enum Number  : %d\n", THREE);
    printf("FOUR Is Enum Number   : %d\n", FOUR);
    printf("FIVE Is Enum Number   : %d\n", FIVE);
    printf("SIX Is Enum Number    : %d\n", SIX);
    printf("SEVEN Is Enum Number  : %d\n", SEVEN);
    printf("EIGHT Is Enum Number  : %d\n", EIGHT);
    printf("NINE Is Enum Number   : %d\n", NINE);
    printf("TEN Is Enum Number    : %d\n\n", TEN);


    printf("JANUARY Is Month Number   : %d\n", JANUARY);
    printf("FEBRUARY Is Month Number  : %d\n", FEBRUARY);
    printf("MARCH Is Month Number     : %d\n", MARCH);
    printf("APRIL Is Month Number     : %d\n", APRIL);
    printf("MAY Is Month Number       : %d\n", MAY);
    printf("JUNE Is Month Number      : %d\n", JUNE);
    printf("JULY Is Month Number      : %d\n", JULY);
    printf("AUGUST Is Month Number    : %d\n", AUGUST);
    printf("SEPTEMBER Is Month Number : %d\n", SEPTEMBER);
    printf("OCTOBER Is Month Number   : %d\n", OCTOMBER);
    printf("NOVEMBER Is Month Number  : %d\n", NOVEMBER);
    printf("DECEMBER Is Month Number  : %d\n\n", DECEMBER);


    printf("Value of TRUE is  : %ld\n",TRUE);
    printf("Value of FALSE is : %ld\n\n",FALSE);

    printf("MY_PI : %.101f\n\n", RSK_PI);
    printf("Area of Circle of Radius 2 units = %f\n\n", (RSK_PI * 2.0f * 2.0f)); //Area of circle PI * r * r

    printf("\n\n");

    printf(AMC_STRING);
    printf("\n\n");

    printf("AMC_STRING IS : %s", AMC_STRING);
    printf("\n\n");



    return(0);    
}