
#include <OpenGL/gl3.h>
#include "vmath.h"

struct SphereData {
	GLfloat*	sphere_vertices;
	GLfloat*	sphere_normals;
	GLfloat*	sphere_texcoords;
	GLuint*		sphere_indices;
	GLuint		numberOfVertices;
	GLuint		numberOfIndices;
};

void Sphere(GLdouble radius, GLuint slices, GLuint stacks, SphereData* sphere_data);
