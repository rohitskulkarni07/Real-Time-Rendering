#pragma once
#include <Windows.h>
#include <stdio.h>
#include "OGL.h"
#include "vector"
#include "string"
#include <gl/glew.h> // graphics library extension wrangler //For GLSL Extensions This Library Must Be Before #include <gl\gl.h>
#include <gl/gl.h>	// graphic library
#include "vmath.h"
#include <vector>

struct Model_Data {

	GLuint vao_model;
	GLuint vbo_model_position;
	GLuint vbo_model_texture;
	GLuint vbo_model_normals;
	GLuint vbo_model_elements;
	GLint numberOfElements;
};

void loadOBJModel(const char*, Model_Data*, int, int, int);

