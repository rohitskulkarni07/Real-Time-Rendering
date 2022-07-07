#include <stdio.h>

struct MyData {
    int rsk_i;
    float rsk_f;
    double rsk_lf;
    char rsk_c;
};

int main(void) {

    struct MyData AddStructMembers(struct MyData, struct MyData, struct MyData);

    struct MyData d1, d2, d3, answer_data;

    printf("\n\n\n\n");
    printf("********** DATA 1 **********\n\n");
    
    printf("Enter Integer Value Of i of Struct MyData d1: ");
    scanf("%d", &d1.rsk_i);
    printf("\n\n");
    
    printf("Enter Float Value Of f of Struct MyData d1: ");
    scanf("%f", &d1.rsk_f);
    printf("\n\n");

    printf("Enter Double Value Of lf of Struct MyData d1: ");
    scanf("%lf", &d1.rsk_lf);
    printf("\n\n");

    printf("Enter Character Value Of c of Struct MyData d1: ");
    d1.rsk_c = getch();
    printf("%c", d1.rsk_c);
    printf("\n\n");

//________________________________________________________________________
  
    printf("\n\n\n\n");
    printf("********** DATA 2 **********\n\n");
    
    printf("Enter Integer Value Of i of Struct MyData d2: ");
    scanf("%d", &d2.rsk_i);
    printf("\n\n");
    
    printf("Enter Float Value Of f of Struct MyData d2: ");
    scanf("%f", &d2.rsk_f);
    printf("\n\n");

    printf("Enter Double Value Of lf of Struct MyData d2: ");
    scanf("%lf", &d2.rsk_lf);
    printf("\n\n");

    printf("Enter Character Value Of c of Struct MyData d2: ");
    d2.rsk_c = getch();
    printf("%c", d2.rsk_c);
    printf("\n\n");

//________________________________________________________________________
  
    printf("\n\n\n\n");
    printf("********** DATA 3 **********\n\n");
    
    printf("Enter Integer Value Of i of Struct MyData d3: ");
    scanf("%d", &d3.rsk_i);
    printf("\n\n");
    
    printf("Enter Float Value Of f of Struct MyData d3: ");
    scanf("%f", &d3.rsk_f);
    printf("\n\n");

    printf("Enter Double Value Of lf of Struct MyData d3: ");
    scanf("%lf", &d3.rsk_lf);
    printf("\n\n");

    printf("Enter Character Value Of c of Struct MyData d2: ");
    d3.rsk_c = getch();
    printf("%c", d3.rsk_c);
    printf("\n\n");

//_________________________________________________________________________

    answer_data = AddStructMembers(d1, d2, d3);

//_________________________________________________________________________

    printf("\n\n\n\n");
    printf("********** ANSWER **********\n\n");
    printf("answer_data.i = %d\n", answer_data.rsk_i);
    printf("answer_data.f = %f\n", answer_data.rsk_f);
    printf("answer_data.lf = %lf\n", answer_data.rsk_lf);

    answer_data.rsk_c = d1.rsk_c;
    printf("answer_data.c from data one = %c\n", answer_data.rsk_c);

    answer_data.rsk_c = d2.rsk_c;
    printf("answer_data.c from data two = %c\n", answer_data.rsk_c);

    answer_data.rsk_c = d3.rsk_c;
    printf("answer_data.c from data three = %c\n\n", answer_data.rsk_c);

    return(0);
}

struct MyData AddStructMembers(struct MyData md1, struct MyData md2, struct MyData md3) {
    
    struct MyData answer;
    
    answer.rsk_i = md1.rsk_i + md2.rsk_i + md3.rsk_i;
    answer.rsk_f = md1.rsk_f + md2.rsk_f + md3.rsk_f;
    answer.rsk_lf = md1.rsk_lf + md2.rsk_lf + md3.rsk_lf;

    return(answer);
}
