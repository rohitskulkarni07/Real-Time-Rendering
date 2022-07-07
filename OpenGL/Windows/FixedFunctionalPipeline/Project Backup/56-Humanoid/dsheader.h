#pragma once 
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/GL.h>
#include <GL/GLU.h>

typedef struct CAMERA {
	GLfloat EyeCoord[3];
	GLfloat CenterCoord[3];
	
	GLfloat xcoord[4];
	GLfloat ycoord[4];
	GLfloat zcoord[4];

	CAMERA* NextLink;
}CAMERA;

