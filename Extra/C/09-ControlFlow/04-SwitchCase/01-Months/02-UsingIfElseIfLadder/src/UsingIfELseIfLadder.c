#include <stdio.h>

int main(void) {

    int rsk_iNum_Month;

    printf("\n\n");
    printf("Enter Month (1-12) : ");
    scanf("%d", &rsk_iNum_Month);
    printf("\n\n");


    if(rsk_iNum_Month == 1) {
        
        printf("Month Number %d Is JANUARY !!!\n\n",rsk_iNum_Month);
        
    } else if(rsk_iNum_Month == 2) {
        printf("Month Number %d Is FEBRUARY !!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 3) {
        printf("Month Number %d Is MARCH !!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 4) {

        printf("Month Number %d Is APRIL !!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 5) {
        printf("Month Number %d Is MAY !!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 6) {
        printf("Month Number %d Is JUNE !!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 7) {
        printf("Month Number %d Is JULY !!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 8) {
        printf("Month Number %d Is AUGUST!!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 9) {
        printf("Month Number %d Is SEPTEMBER!!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 10) {
        printf("Month Number %d Is OCTOBER!!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 11) {
        printf("Month Number %d Is NOVEMBER!!!\n\n",rsk_iNum_Month);

    } else if(rsk_iNum_Month == 12) {
        printf("Month Number %d Is DECEMBER!!!\n\n",rsk_iNum_Month);

    } else {
        printf("Invalid Month Number %d !!! Please Try Again\n\n",rsk_iNum_Month);

    }
    return(0);
}