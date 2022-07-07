#import <OpenGLES/ES3/gl.h>
#import <OpenGLES/ES3/glext.h>
#include "vmath.h"
#include "sphere.h"
#define _USE_MATH_DEFINES  1 // Include constants defined in math.h
#include <math.h>



void Sphere(GLfloat radius, GLuint slices, GLuint stacks, SphereData* sphere_data) {

	GLuint vertex_count = (slices + 1) * (stacks + 1);
	
	GLfloat* vertices_array		= (GLfloat*)malloc(3 * vertex_count * sizeof(GLfloat));
	GLfloat* normal_array		= (GLfloat*)malloc(3 * vertex_count * sizeof(GLfloat));
	GLfloat* texcoords_array	= (GLfloat*)malloc(2 * vertex_count * sizeof(GLfloat));
	GLuint*  indices_array		= (GLuint*)malloc(2 * slices * stacks * 3 * sizeof(GLuint));

	GLfloat du = 2 * M_PI / slices;
	GLfloat dv = M_PI / stacks;
	
	//x,y,z => vertex coords
	GLfloat u, v, x, y, z;

	GLuint indexV = 0;
	GLuint indexT = 0;
	GLuint k = 0;

	for (int i = 0; i <= stacks; i++) {

		v = -M_PI / 2 + i * dv;
		
		for (int j = 0; j <= slices; j++) {
			
			u = j * du;
			x = cos(u) * cos(v);
			y = sin(u) * cos(v);
			z = sin(v);

			vertices_array[indexV] = radius * x;
			normal_array[indexV++] = x;

			vertices_array[indexV] = radius * y;
			normal_array[indexV++] = y;

			vertices_array[indexV] = radius * z;
			normal_array[indexV++] = z;

			texcoords_array[indexT++] = (GLfloat)j / (GLfloat)slices;
			texcoords_array[indexT++] = (GLfloat)i / (GLfloat)stacks;


		}
	}

	//Indices
	for (int j = 0; j < stacks; j++) {
		GLuint row1 = j * (slices + 1);
		GLuint row2 = (j + 1) * (slices + 1);

		for (int i = 0; i < slices; i++) {
			indices_array[k++] = row1 + i;
			indices_array[k++] = row2 + i + 1;
			indices_array[k++] = row2 + i;
			indices_array[k++] = row1 + i;
			indices_array[k++] = row1 + i + 1;
			indices_array[k++] = row2 + i + 1;
		}
	}

	sphere_data->sphere_vertices	= vertices_array;
	sphere_data->sphere_normals		= normal_array;
	sphere_data->sphere_texcoords	= texcoords_array;
	sphere_data->sphere_indices		= indices_array;
	sphere_data->numberOfVertices	= vertex_count;
	sphere_data->numberOfIndices	= k;

}
