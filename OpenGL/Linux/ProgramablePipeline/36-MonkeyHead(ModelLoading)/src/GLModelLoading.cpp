#include "ModelHeader.h"

#define BUFFER_SIZE 512

void loadOBJModel(const char* filename, Model_Data* model_data, int vertexIndex, int textureIndex, int normalIndex) {

	// variable declaration

	std::vector <GLfloat> vf_vertices;
	std::vector <GLfloat> vf_textures;
	std::vector <GLfloat> vf_normals;
	std::vector <GLint>	  vf_indices;

	char line[BUFFER_SIZE];
	char* first_token, * token;

	FILE* fp	 = NULL;

	GLfloat v[3];
	GLfloat vt[2];
	GLfloat vn[3];

	GLfloat* text_array		= NULL;
	GLfloat* norm_array		= NULL;
	GLfloat* vert_array		= NULL;
	GLuint*  index_array	= NULL;

	GLint vert_size, text_size, norm_size, index_size;

	bool bIsAllocated = false;

	int vi, ti, ni;
	char* face_vertex[3];

	fp = fopen(filename, "r");
	if(!fp) {
		printf("\nFailed To Open File\n");
		return;
	}

	while (fgets(line, BUFFER_SIZE, fp) != NULL) {
		first_token = strtok(line, " ");

		if (strcmp(first_token, "v") == 0) {

			v[0] = atof(strtok(NULL, " "));
			v[1] = atof(strtok(NULL, " "));
			v[2] = atof(strtok(NULL, " "));

			vf_vertices.push_back(v[0]);
			vf_vertices.push_back(v[1]);
			vf_vertices.push_back(v[2]);
		}
		else if (strcmp(first_token, "vt") == 0) {

			vt[0] = atof(strtok(NULL, " "));
			vt[1] = atof(strtok(NULL, " "));

			vf_textures.push_back(vt[0]);
			vf_textures.push_back(vt[1]);


		}
		else if (strcmp(first_token, "vn") == 0) {

			vn[0] = atof(strtok(NULL, " "));
			vn[1] = atof(strtok(NULL, " "));
			vn[2] = atof(strtok(NULL, " "));

			vf_normals.push_back(vn[0]);
			vf_normals.push_back(vn[1]);
			vf_normals.push_back(vn[2]);


		}
		else if (strcmp(first_token, "f") == 0) {

			if (bIsAllocated == false) {

				text_array = (GLfloat*)malloc(2 * vf_vertices.size() / 3 * sizeof(GLfloat));
				norm_array = (GLfloat*)malloc(vf_vertices.size() * sizeof(GLfloat));

				bIsAllocated = true;
			}

			//f v1/t1/n1	v2/t2/n2	v3/t3/n3

			face_vertex[0] = strtok(NULL, " ");		//v1/t1/n1
			face_vertex[1] = strtok(NULL, " ");		//v2/t2/n2
			face_vertex[2] = strtok(NULL, " ");		//v3/t3/n3

			for (int k = 0; k < 3; k++) {
				
				first_token = strtok(face_vertex[k], "/");		//v
				vi = atoi(first_token) - 1;
				ti = atoi(strtok(NULL, "/")) - 1;
				ni = atoi(strtok(NULL, "/")) - 1;

				vf_indices.push_back(vi);

				text_array[2 * vi + 0] = vf_textures[2 * ti + 0];
				text_array[2 * vi + 1] = vf_textures[2 * ti + 1];

				norm_array[3 * vi + 0] = vf_normals[3 * ni + 0];
				norm_array[3 * vi + 1] = vf_normals[3 * ni + 1];
				norm_array[3 * vi + 2] = vf_normals[3 * ni + 2];
			}
		}
	}


	vert_array = (GLfloat*)malloc(vf_vertices.size() * sizeof(GLfloat));


	for (int i = 0; i < vf_vertices.size(); i++) {
		vert_array[i] = vf_vertices[i];
	}


	index_array = (GLuint*)malloc(vf_indices.size() * sizeof(GLuint));
	for (int i = 0; i < vf_indices.size(); i++) {
		index_array[i] = vf_indices[i];
	}


	vert_size	= vf_vertices.size();
	text_size	= vf_textures.size();
	norm_size	= vf_normals.size();
	index_size	= vf_indices.size();

	vf_vertices.clear();
	vf_textures.clear();
	vf_normals.clear();
	vf_indices.clear();


	//load vao and vbo with proper data

	glGenVertexArrays(1, &model_data->vao_model);
	glBindVertexArray(model_data->vao_model);

		//position
		glGenBuffers(1, &model_data->vbo_model_position);
		glBindBuffer(GL_ARRAY_BUFFER, model_data->vbo_model_position);
		
			glBufferData(GL_ARRAY_BUFFER, vert_size * sizeof(GLfloat), vert_array, GL_STATIC_DRAW);
			glVertexAttribPointer(vertexIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(vertexIndex);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//texture
		glGenBuffers(1, &model_data->vbo_model_texture);
		glBindBuffer(GL_ARRAY_BUFFER, model_data->vbo_model_texture);

			glBufferData(GL_ARRAY_BUFFER, text_size * sizeof(GLfloat), text_array, GL_STATIC_DRAW);
			glVertexAttribPointer(textureIndex, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(textureIndex);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//normals
		glGenBuffers(1, &model_data->vbo_model_texture);
		glBindBuffer(GL_ARRAY_BUFFER, model_data->vbo_model_texture);

			glBufferData(GL_ARRAY_BUFFER, norm_size * sizeof(GLfloat), norm_array, GL_STATIC_DRAW);
			glVertexAttribPointer(normalIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			glEnableVertexAttribArray(normalIndex);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//elements
		glGenBuffers(1, &model_data->vbo_model_elements);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model_data->vbo_model_elements);
		
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size * sizeof(GLuint), index_array, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	glBindVertexArray(0);

	model_data->numberOfElements = index_size;

	free(index_array);
	free(vert_array);
	free(text_array);
	free(norm_array);

	fclose(fp);
	fp = NULL;
}