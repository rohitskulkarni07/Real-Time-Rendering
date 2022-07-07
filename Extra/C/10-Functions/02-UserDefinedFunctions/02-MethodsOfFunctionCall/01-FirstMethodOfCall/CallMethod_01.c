#include <stdio.h>

int main(int argc, char *argv[], char *envp[]) {

    void MyAddition(void);
    int MySubtraction(void);
    void MyMultiplicarion(int ,int );
    int MyDivision(int ,int);

    int rsk_result_subtraction;
    int rsk_a_multiplication, rsk_b_multiplication;
    int rsk_a_division, rsk_b_division, rsk_result_division;

    MyAddition();

    rsk_result_subtraction = MySubtraction();

    printf("\n\n");
    printf("Subtraction Yeilds Result = %d \n",rsk_result_subtraction);

    printf("\n\n");
    printf("ENter Integer For 'A' For Multiplication : ");
    scanf("%d",&rsk_a_multiplication);

    printf("\n\n");
    printf("ENter Integer For 'B' For Multiplication : ");
    scanf("%d",&rsk_b_multiplication);

    MyMultiplicarion(rsk_a_multiplication, rsk_b_multiplication);

    printf("Enter Integer Value For 'A' For Division : ");    
    scanf("%d",&rsk_a_division);

    printf("\n\n");
    printf("Enter Integer Value For 'B' For Division : ");    
    scanf("%d",&rsk_b_division);

    rsk_result_division = MyDivision(rsk_a_division, rsk_b_division);
    printf("\n\n");

    printf("Division Of %d and %d Gives %d (Quotient)\n", rsk_a_division, rsk_b_division, rsk_result_division);

    return(0);
}

void MyAddition(void) {

    int rsk_a, rsk_b;

    printf("\n\n");
    printf("ENter Integer For 'A' For Addition : ");
    scanf("%d",&rsk_a);

    printf("\n\n");
    printf("ENter Integer For 'B' For Addition : ");
    scanf("%d",&rsk_b);

    int rsk_sum = rsk_a + rsk_b;

    printf("\n\n");
    printf("Addition Of %d and %d Gives %d \n", rsk_a, rsk_b, rsk_sum);

}

int MySubtraction(void) {

    int rsk_a, rsk_b;

    printf("\n\n");
    printf("ENter Integer For 'A' For Subtraction : ");
    scanf("%d",&rsk_a);

    printf("\n\n");
    printf("ENter Integer For 'B' For Subtraction : ");
    scanf("%d",&rsk_b);

    int rsk_subtraction = rsk_a - rsk_b;

    return(rsk_subtraction);

}

void MyMultiplicarion(int rsk_a_multiplication, int rsk_b_multiplication){

    int rsk_result_multiplication = rsk_a_multiplication * rsk_b_multiplication;
    
    printf("\n\n");
    printf("Multiplication Of %d and %d Gives %d \n", rsk_a_multiplication, rsk_b_multiplication, rsk_result_multiplication);
    printf("\n\n");

}


int MyDivision(int rsk_a_division, int rsk_b_division) {

    return (rsk_a_division >rsk_b_division) ? rsk_a_division/rsk_b_division : rsk_b_division/rsk_a_division;

}
