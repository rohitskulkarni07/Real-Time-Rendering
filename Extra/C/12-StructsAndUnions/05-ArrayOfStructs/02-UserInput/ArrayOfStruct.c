#include <stdio.h>

#define NAME_LENGTH 100
#define TOTAL_EMP 5

struct Employee {

    char rsk_name[NAME_LENGTH];
    float rsk_salary;
    int rsk_age;
    char rsk_sex;
    char rsk_martial_status;
};


int main (void) {

    struct Employee EmployeeRecords[TOTAL_EMP];

    void MyGetString(char[], int);
    
    int i;


    for(i = 0; i < TOTAL_EMP ; i++) {
 
        printf("\n\n***** Data Entry For Empoyee Number %d *****\n\n", i+1);
        printf("\nEnter Name: ");
        MyGetString(EmployeeRecords[i].rsk_name, NAME_LENGTH);

        printf("\nEnter Age: ");
        scanf("%d",&EmployeeRecords[i].rsk_age);
 
        printf("\nEnter Employe's Sex: \tMALE-> M / FEMALE -> F: ");
        EmployeeRecords[i].rsk_sex = getch();
        printf("%c\n", EmployeeRecords[i].rsk_sex);

        printf("\nEnter Salary : ");
        scanf("%f",&EmployeeRecords[i].rsk_salary);

        printf("\nMartial Status :Married -> Y / Unmarried -> N: ");
        EmployeeRecords[i].rsk_martial_status = getch();

        printf("%c\n",EmployeeRecords[i].rsk_martial_status);
        EmployeeRecords[i].rsk_martial_status = toupper(EmployeeRecords[i].rsk_martial_status);
    }
//____________________________________________________________________________________
    printf("\n\n*** DISPLAY EMPLOYEE RECORD ***\n\n");

    for(i = 0; i < TOTAL_EMP ; i++) {

        printf("\n\n***** Empoyee Number %d *****\n\n", i+1);
        printf("Name :\t\t%s\n",EmployeeRecords[i].rsk_name);
        printf("Age  :\t\t%d\n",EmployeeRecords[i].rsk_age);
 
        if(EmployeeRecords[i].rsk_sex == 'M' || EmployeeRecords[i].rsk_sex == 'm') {
            printf("Sex  :\t\tMALE\n");

        } else if (EmployeeRecords[i].rsk_sex == 'F' ||EmployeeRecords[i].rsk_sex == 'f') {
            printf("Sex  :\t\tFEMALE\n");

        } else {
            printf("Invalid Data\n");
             
        }

        printf("Salary :\t\tRs.%f\n", EmployeeRecords[i].rsk_salary);

        if(EmployeeRecords[i].rsk_martial_status == 'Y'){
            printf("Martial Staus: MARRIED");
        
        } else if(EmployeeRecords[i].rsk_martial_status == 'N') {

            printf("Martial Staus :UNMARRIED\n");
        } else {
            printf("Ïnvalid Status\n");
        }
    }


    return(0);

}


void MyGetString(char str[], int str_size) {

    int i;

    char ch = '\0';

    i = 0;

    do{
        ch = getch();
        str[i] = ch;
        printf("%c", str[i]);
        i++;
    }while((ch != '\r') && (i < str_size));
    printf("\n");
    if(i == str_size) {

        str[i-1] = '\0';

    } else {

        str[i] = '\0';
    }
}