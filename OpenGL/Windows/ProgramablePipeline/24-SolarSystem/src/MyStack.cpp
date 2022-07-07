#pragma once
#include <Windows.h>
#include "MyStack.h"
#include "OGL.h"

struct CTM* pTop = NULL;

void pushMatrix(mat4 data) {

	CTM* temp = NULL;

	temp = (CTM*)malloc(sizeof(CTM));
	if (!temp) {
		fprintf(gpFile, "\nERROR IN STACK FAILED TO ALLOCATE MEMORY\n");
		DestroyWindow(ghwnd);
	}

	temp->data = data;
	temp->link = pTop;
	pTop = temp;
}


void popMatrix(mat4 *data) {

	CTM* temp = NULL;

	if (pTop == NULL) {
		fprintf(gpFile, "\nERROR IN STACK FAILED TO ALLOCATE MEMORY\n");
		DestroyWindow(ghwnd);
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