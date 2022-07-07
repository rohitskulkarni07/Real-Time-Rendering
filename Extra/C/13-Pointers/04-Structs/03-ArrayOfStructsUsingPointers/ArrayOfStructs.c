#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define NAME_LENGHTH 100
#define MARTIAL_STATUS 10

struct Employee {

    char rsk_name[NAME_LENGHTH];
    int rsk_age;
    char rsk_sex;
    float rsk_salary;
    char martial_ststus;

};

int main(void) {

    void MyGetString(char[], int);

    struct Employee *pEmployeeRecord = NULL;

    int num_employees, i;

    printf("\n\n");
    printf("Enter Number Of Employees Whose Details You Want To Record : ");
    scanf("%d", &num_employees);

    printf("\n\n");
    pEmployeeRecord = (struct Employee *)malloc(sizeof(struct Employee) * num_employees);

    if(pEmployeeRecord == NULL) {

        printf("Failed To Allocate Memory For %d Employees !!! Exiting Now...\n\n", num_employees);
        exit(0);
    } else {
        printf("Successfully Allocated Memory For %d Employees !!!\n\n", num_employees);

    }

    for(i = 0; i < num_employees; i++) {

        printf("\n\n\n\n");
        printf("******** DATA ENTRY FOR EMPLOYEE NUMBER %d ********\n", (i + 1));
        
        printf("\n\n");
        printf("Enter Employee Name : ");
        MyGetString(pEmployeeRecord[i].rsk_name, NAME_LENGHTH);

        printf("\n\n\n");
        printf("Enter Employee's Age (in Years) : ");
        scanf("%d", &pEmployeeRecord[i].rsk_age);

        printf("\n\n");
        printf("Enter Employee's Sex (M/m For Male, F/f For Female) : ");
        pEmployeeRecord[i].rsk_sex = getch();
        printf("%c", pEmployeeRecord[i].rsk_sex);
        pEmployeeRecord[i].rsk_sex = toupper(pEmployeeRecord[i].rsk_sex);

        printf("\n\n\n");
        printf("Enter Employee's Salary (in Indian Ruppes) :");
        scanf("%f", &pEmployeeRecord[i].rsk_salary);

        printf("\n\n");
        printf("Is The Employee Married? (Y/y For Yes, N/n For No) :");
        pEmployeeRecord[i].martial_ststus = getch();
        printf("%c", pEmployeeRecord[i].martial_ststus);
        pEmployeeRecord[i].martial_ststus = toupper(pEmployeeRecord[i].martial_ststus);
        
    }

    printf("\n\n");
    printf("********** DISPLAYING EMPLOYEE RECORDS **********\n\n");

    for(i = 0; i < num_employees; i++) {

        printf("********* EMPLOYEE NUMBER %d **********\n\n", (i + 1));
        printf("Name            : %s\n", pEmployeeRecord[i].rsk_name);
        printf("Age             : %d Years\n", pEmployeeRecord[i].rsk_age);
        if(pEmployeeRecord[i].rsk_sex == 'M') {
            printf("Sex             : Male\n");
        } else if(pEmployeeRecord[i].rsk_sex == 'F') {
            printf("Sex             : Female\n");
        } else {
            printf("Sex             :Invalid Data Entered\n");
        }

        if(pEmployeeRecord[i].martial_ststus == 'Y') {
            printf("Martial Status : Married\n");
        } else if(pEmployeeRecord[i].martial_ststus == 'N') {
            printf("Martial Status : Unmarried\n");
        } else {
            printf("Martial Status : Invalid Data Entered\n");
        }

        printf("\n\n");
        
    }

    if(pEmployeeRecord) {
        free(pEmployeeRecord);
        pEmployeeRecord = NULL;
        printf("Memory Allocated %d Employees Has Been Successfully Freed !!!\n\n", num_employees);
    }

    return(0);

}


void MyGetString(char str[],int str_size) {

    int i;
    char ch = '\0';

    i = 0;

    do {
        ch = getch();
        str[i] = ch;
        printf("%c", str[i]);
        i++;
    } while((ch != '\r') && (i < str_size));

    if(i == str_size) {
        str[i - 1] = '\0';

    } else {
        str[i] = '\0';
    }

}

