#include <stdio.h>

int main(void) {

    int rsk_iNum_Month;

    printf("\n\n");

    printf("Enter NUmber Of Month(1-12) :");
    scanf("%d", &rsk_iNum_Month);

    printf("\n\n");

    switch(rsk_iNum_Month) {
        case 1:
            printf("Month Number %d Is JANUARY !!!\n\n",rsk_iNum_Month);
            break;
        case 2:
            printf("Month Number %d Is FEBRUARY !!!\n\n",rsk_iNum_Month);
            break;
        case 3:
            printf("Month Number %d Is MARCH !!!\n\n",rsk_iNum_Month);
            break;
        case 4:
            printf("Month Number %d Is APRIL !!!\n\n",rsk_iNum_Month);
            break;
        case 5:
            printf("Month Number %d Is MAY !!!\n\n",rsk_iNum_Month);
            break;
        case 6:
            printf("Month Number %d Is JUNE !!!\n\n",rsk_iNum_Month);
            break;
        case 7:
            printf("Month Number %d Is JULY !!!\n\n",rsk_iNum_Month);
            break;
        case 8:
            printf("Month Number %d Is AUGUST!!!\n\n",rsk_iNum_Month);
            break;
        case 9:
            printf("Month Number %d Is SEPTEMBER!!!\n\n",rsk_iNum_Month);
            break;
        case 10:
            printf("Month Number %d Is OCTOBER!!!\n\n",rsk_iNum_Month);
            break;
        case 11:
            printf("Month Number %d Is NOVEMBER!!!\n\n",rsk_iNum_Month);
            break;
        case 12:
            printf("Month Number %d Is DECEMBER!!!\n\n",rsk_iNum_Month);
            break;
        default:
            printf("Invalid Month Number %d !!! Please Try Again\n\n",rsk_iNum_Month);
            break;
    }

    printf("Completed Switc-case\n\n");

    
    return(0);

}