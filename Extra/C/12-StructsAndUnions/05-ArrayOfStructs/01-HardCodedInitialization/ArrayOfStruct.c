#include <stdio.h>

#define NAME_LENGTH 100
#define MARITAL_STATUS 10
#define TOTAL_EMP 5

struct Employee {

    char rsk_name[NAME_LENGTH];
    float rsk_salary;
    int rsk_age;
    char rsk_sex;
    char rsk_martial_status[MARITAL_STATUS];
};



int main (void) {

    struct Employee EmployeeRecords[TOTAL_EMP];

    char employee_Rohit[] = "Rohit";
    char employee_Mohit[] = "Mohit";
    char employee_Animesh[] = "Animesh";
    char employee_Vijay[] = "Vijay";
    char employee_Pranav[] = "Pranav";

    int i;


    // EMPLOYEE 1
    strcpy(EmployeeRecords[0].rsk_name, employee_Rohit);
    EmployeeRecords[0].rsk_age = 23;
    EmployeeRecords[0].rsk_sex = 'M';
    EmployeeRecords[0].rsk_salary = 50000.0f;
    strcpy(EmployeeRecords[0].rsk_martial_status, "Married");

    // EMPLOYEE 2
    strcpy(EmployeeRecords[1].rsk_name, employee_Mohit);
    EmployeeRecords[1].rsk_age = 22;
    EmployeeRecords[1].rsk_sex = 'M';
    EmployeeRecords[1].rsk_salary = 45000.0f;
    strcpy(EmployeeRecords[1].rsk_martial_status, "Unmarried");

    // EMPLOYEE 3
    strcpy(EmployeeRecords[2].rsk_name, employee_Animesh);
    EmployeeRecords[2].rsk_age = 23;
    EmployeeRecords[2].rsk_sex = 'M';
    EmployeeRecords[2].rsk_salary = 55000.0f;
    strcpy(EmployeeRecords[2].rsk_martial_status, "Married");

    // EMPLOYEE 4
    strcpy(EmployeeRecords[3].rsk_name, employee_Vijay);
    EmployeeRecords[3].rsk_age = 22;
    EmployeeRecords[3].rsk_sex = 'M';
    EmployeeRecords[3].rsk_salary = 60000.0f;
    strcpy(EmployeeRecords[3].rsk_martial_status, "Married");

    // EMPLOYEE 5
    strcpy(EmployeeRecords[4].rsk_name, employee_Pranav);
    EmployeeRecords[4].rsk_age = 23;
    EmployeeRecords[4].rsk_sex = 'M';
    EmployeeRecords[4].rsk_salary = 35000.0f;
    strcpy(EmployeeRecords[4].rsk_martial_status, "Unmarried");


    for(i = 0; i < TOTAL_EMP ; i++) {
 
        printf("\n\n***** Empoyee Number %d *****\n\n", i+1);
        printf("Name :\t\t%s\n",EmployeeRecords[i].rsk_name);
        printf("Age  :\t\t%d\n",EmployeeRecords[i].rsk_age);
 
        if(EmployeeRecords[i].rsk_sex == 'M' || EmployeeRecords[i].rsk_sex == 'm') {
            printf("Sex  :\t\tMALE\n");
        } else {
            printf("Sex  :\t\tFEMALE\n");
             
        }

        printf("Salary :\t\tRs.%f\n", EmployeeRecords[i].rsk_salary);
        printf("Martial Staus :\t\t%s\n\n", EmployeeRecords[i].rsk_martial_status);

    }

    return(0);

}

