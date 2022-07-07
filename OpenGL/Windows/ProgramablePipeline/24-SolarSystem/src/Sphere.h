#pragma once
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <GL/GL.h>
#define _USE_MATH_DEFINES  1 // Include constants defined in math.h
#include <math.h>

struct SphereData {
	GLfloat*	sphere_vertices;
	GLfloat*	sphere_normals;
	GLfloat*	sphere_texcoords;
	GLuint*		sphere_indices;
	GLuint		numberOfVertices;
	GLuint		numberOfIndices;
};

void Sphere(GLdouble radius, GLuint slices, GLuint stacks, SphereData* sphere_data);
