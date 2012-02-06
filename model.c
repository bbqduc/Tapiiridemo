#include "model.h"
#include <assert.h>
#include <stdlib.h>

void modelInit(Model* model)
{
	assert(model);
	model->numVertices = 0;
	model->normals = model->texcoords = 0;
	model->indices = 0;
}

void modelVertexInit(Model* model, GLuint numVertices, GLfloat* data)
{
	assert(model);
	model->numVertices = numVertices;
	model->vertices = (GLfloat*)malloc(3*sizeof(GLfloat)*numVertices); // TODO : what about non-3D models?
	memcpy(model->vertices, data, 3*sizeof(GLfloat)*numVertices);
}

void modelTexCoordInit(Model* model, GLuint numVertices, GLfloat* data)
{
	assert(model);
	model->numVertices = numVertices;
	model->texcoords = (GLfloat*)malloc(sizeof(GLfloat)*numVertices*2); // TODO : what about non-2D textures?
}

void modelNormalsInit(Model* model)
{
	assert(model); assert(model->numVertices); assert(model->vertices);
	// copy & paste here from sheikki
}

void modelInitVBOs(Model* model)
{
	assert(model); assert(model->numVertices); assert(model->vertices);

	glGenVertexArrays(1, &model->VAO_id);
	glBindVertexArray(model->VAO_id);

	// initialize VBO for model vertices and polygon vertex indices
	glGenBuffers(1, &model->VBO_vertices_id);
	glBindBuffer(GL_ARRAY_BUFFER, model->VBO_vertices_id);
	glBufferData(GL_ARRAY_BUFFER, model->numVertices*3*sizeof(GLfloat), model->vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	if(model->normals)
	{
		// initialize VBO for normals of vertices
		glGenBuffers(1, &model->VBO_normals_id);
		glBindBuffer(GL_ARRAY_BUFFER, model->VBO_normals_id);
		glBufferData(GL_ARRAY_BUFFER, model->numVertices*3*sizeof(GLfloat), model->normals, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	if (model->texcoords)
	{
		glGenBuffers(1, &model->VBO_texcoord_id);
		glBindBuffer(GL_ARRAY_BUFFER, model->VBO_texcoord_id);
		glBufferData(GL_ARRAY_BUFFER, model->numVertices*2*sizeof(GLfloat), model->texcoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	/*glGenBuffers(1, &model->VBO_indices_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numPolygons*3*sizeof(GLuint), model->polygons), GL_STATIC_DRAW);*/

	glBindVertexArray(0);

}

void modelDestroy(Model* model)
{
	free(model->vertices);
	free(model->texcoords);
	free(model->normals);
	free(model->indices);
}
