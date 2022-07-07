#include <stdio.h>

#define MAX_NUMBER(a, b) ((a>b)? a : b)

int main(int argc, char *argv[],char *envp[]) {

    int rsk_iNum_01;
    int rsk_iNum_02;
    int rsk_iResult;

    float rsk_fNum_01;
    float rsk_fNum_02;
    float rsk_fResult;


    printf("\n\n");
    printf("ENter An Integer : \n\n");
    scanf("%d",&rsk_iNum_01);

    printf("\n\n");
    printf("ENter Another Integer : \n\n");
    scanf("%d",&rsk_iNum_02);


    rsk_iResult = MAX_NUMBER(rsk_iNum_01,rsk_iNum_02);

    printf("\n\n");
    printf("Result Of Macro Funtion MAX_NUMBER() = %d\n", rsk_iResult);
    printf("\n\n");

    printf("\n\n");
    printf("ENter An Floating Value : \n\n");
    scanf("%f",&rsk_fNum_01);

    printf("\n\n");
    printf("ENter Another Floating Value : \n\n");
    scanf("%f",&rsk_fNum_02);


    rsk_fResult = MAX_NUMBER(rsk_fNum_01,rsk_fNum_02);

    printf("\n\n");
    printf("Result Of Macro Funtion MAX_NUMBER() = %f \n", rsk_fResult);
    printf("\n\n");

    return(0);

}