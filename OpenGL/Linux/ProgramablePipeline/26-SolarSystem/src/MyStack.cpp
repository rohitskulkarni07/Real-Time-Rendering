#include "MyStack.h"

struct CTM* pTop = NULL;

void pushMatrix(mat4 data) {

	CTM* temp = NULL;

	temp = (CTM*)malloc(sizeof(CTM));
	if (!temp) {
		printf("\nERROR IN STACK FAILED TO ALLOCATE MEMORY\n");
		exit(0);
	}

	temp->data = data;
	temp->link = pTop;
	pTop = temp;
}


void popMatrix(mat4 *data) {

	CTM* temp = NULL;

	if (pTop == NULL) {
		printf("\nERROR IN STACK FAILED TO ALLOCATE MEMORY\n");
		exit(0);
	}

	temp = pTop;
	*data = temp->data;
	pTop = pTop->link;
	temp->link = NULL;
	free(temp);
	temp = NULL;
}


void deleteStack() {
	
	CTM* temp = NULL;


	while (temp != NULL) {
		temp = pTop;
		
		pTop = pTop->link;
		temp->link = NULL;

		free(temp);
		temp = NULL;
	}

	free(pTop);
	pTop = NULL;
}