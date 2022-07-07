#include <stdio.h>

int main(void) {

    //prototype
    int SumOfTwoInteger(int ,int );

    //variables
    int iNum1, iNum2, iNum3;

    iNum1 = 25;
    iNum2 = 75;

    iNum3 =  SumOfTwoInteger(iNum1 ,iNum2);

    printf("Sum = %d\n",iNum3);     //prints 100
    
    printf("Sum = %d\n",SumOfTwoInteger(125,175)); //prints 300

    SumOfTwoInteger(175,225);    //prints nothing but definelty operation is done;

    return(0);
}

int SumOfTwoInteger(int iNum1 ,int iNum2) {
    return(iNum1 + iNum2);
}
