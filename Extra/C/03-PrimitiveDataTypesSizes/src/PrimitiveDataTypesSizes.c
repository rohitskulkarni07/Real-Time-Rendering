#include <stdio.h>

int main() {

	//code

	printf("\n\n");

	printf("Size of 'int'               = %ld Bytes i.e => %ld Bits\n", sizeof(int), (sizeof(int) * 8));
	printf("Size of 'unsigned int'      = %ld Bytes i.e => %ld Bits\n", sizeof(unsigned int), (sizeof(unsigned int) * 8));
	printf("Size of 'long'              = %ld Bytes i.e => %ld Bits\n", sizeof(long), (sizeof(long) * 8));
	printf("Size of 'long long'         = %ld Bytes i.e => %ld Bits\n", sizeof(long long), (sizeof(long long) * 8));

	printf("Size of 'float'             = %ld Bytes i.e => %ld Bits\n", sizeof(float), (sizeof(float) * 8));
	printf("Size of 'double'            = %ld Bytes i.e => %ld Bits\n", sizeof(double), (sizeof(double) * 8));
	printf("Size of 'long double'       = %ld Bytes i.e => %ld Bits\n", sizeof(long double), (sizeof(long double) * 8));

	printf("\n\n");

	return(0);
}