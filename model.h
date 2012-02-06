#ifndef MODEL_H
#define MODEL_H

#include <GL3\gl3w.h>

typedef struct 
{
	GLint drawMode;
	GLuint texture;
	GLuint VBO_vertices_id, VBO_normals_id, VBO_indices_id, VBO_color_id, VBO_texcoord_id, VAO_id;

	GLuint numVertices, numPolygons;
	GLfloat *vertices, *normals, *texcoords;
	GLuint *indices;
} Model;

void modelInit(Model* model);
void modelVertexInit(Model* model, GLuint numVertices, GLfloat* data);
void modelTexCoordInit(Model* model, GLuint numVertices, GLfloat* data);
void modelNormalsInit(Model* model);
void modelInitVBOs(Model* model);
void modelDestroy(Model* model);


















#endif