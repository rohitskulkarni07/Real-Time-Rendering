#include <stdio.h> // header file

int main(void) {

	//variable Declearation and assignment at the same time

	int rsk_i = 5;				//int type
	float rsk_f = 3.99f;			//float type
	double rsk_d = 8.0411997;	//double type
	char rsk_c = 'A';			//character type

	//code
	printf("\n\n"); //print 2 new lines

	printf("i = %d\n", rsk_i);	// %d decimal
	printf("f = %f\n", rsk_f);	// %f  06 Number with fractional part value { float value does not round off the orignal value }
	printf("d = %lf\n", rsk_d); // %lf 10 significant digit with fractional part value { double round off the orignal value }
	printf("c = %c\n", rsk_c);  // %c character literal
	
	printf("\n\n");

	//assignment
	rsk_i = 43;
	rsk_f = 6.54f;
	rsk_d = 26.1294;
	rsk_c = 'p';

	printf("i = %d\n", rsk_i);
	printf("f = %f\n", rsk_f);
	printf("d = %lf\n", rsk_d);
	printf("c = %c\n", rsk_c);

	printf("\n\n");

	//statement (keyword)
	return(0);
}
