#include <stdio.h>

int main(void) {

    int AddIntegers(int, int);
    int SubtractIntegers(int, int);
    float AddFloats(float, float);

    typedef int (*AddIntsFnPtr) (int, int);
    AddIntsFnPtr ptrAddTwoInteger = NULL;
    AddIntsFnPtr ptrFunc = NULL;

    typedef float (*AddFloatFnPtr) (float, float);
    AddFloatFnPtr ptrAddTwoFloats = NULL;

    int iAnswer = 0;
    float fAnswer = 0.0f;

    ptrAddTwoInteger = AddIntegers;
    iAnswer = ptrAddTwoInteger(9, 30);

    printf("\n\n");
    printf("Sum Of Integers = %d\n\n",iAnswer);

    ptrFunc = SubtractIntegers;
    iAnswer = ptrFunc(9, 30);

    printf("\n\n");
    printf("Subtraction Of Integer = %d\n\n", iAnswer);

    ptrAddTwoFloats = AddFloats;
    fAnswer = ptrAddTwoFloats(11.45, 8.2f);
    printf("\n\n");
    printf("Sum Of Floating-Point Numbers = %f\n\n",fAnswer);

    return(0);
}

int AddIntegers(int a, int b) {

    int c;

    c = a + b;

    return(c);
}

int SubtractIntegers(int a, int b) {

    int c;

    if(a > b) {
        c = a - b;

    } else {
        c = a - b;
    }

    return(c);
}

float AddFloats(float fNum1, float fNum2) {

    float ans;

    ans = fNum1 + fNum2;

    return(ans);
}
