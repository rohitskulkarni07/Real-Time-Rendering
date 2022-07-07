#pragma once
#include <iostream>		// Input-Output Stream
#include <stdio.h>		// For C Standard Input-Output Function
#include <stdlib.h>		// For C Standard library Functions ex. exit()
#include <memory.h> 	// memset
#include "vector"
#include "string"
#include <X11/Xlib.h>	// XLib Releted API
#include <X11/Xutil.h>	// XVisualInfo Releted Function
#include <X11/XKBlib.h> // Releted To Keyboard symbol conversion
#include <X11/keysym.h> // KeySym Releted API
#include <GL/glew.h>	// For GLSL Extensions This Library Must Be Before #include <gl\gl.h>
#include <GL/glx.h>		// For glx API ding API Library
#include <GL/gl.h>		// For OpenGL API	// Path : /usr/include/GL
#include "../vmath.h"	// This File Comes With RedBook 8th Edition Header for vector mathematics

struct Model_Data {

	GLuint vao_model;
	GLuint vbo_model_position;
	GLuint vbo_model_texture;
	GLuint vbo_model_normals;
	GLuint vbo_model_elements;
	GLint numberOfElements;
};

void loadOBJModel(const char*, Model_Data*, int, int, int);

