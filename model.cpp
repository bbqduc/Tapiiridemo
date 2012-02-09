#include "model.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

// Private constructor used by Model_3ds_loader
Model::Model():
numVertices(0),
	numPolygons(0),
	vertices(NULL),
	normals(NULL),
	texcoords(NULL),
	polygons(NULL)
{}

Model::Model(const Model& model):
	numVertices(model.numVertices),
	numPolygons(model.numPolygons),
	vertices(new glm::vec3[numVertices]),
	normals(new glm::vec3[numVertices]),
	texcoords(NULL),
	polygons(new glm::uvec3[numPolygons]),
	drawMode(model.drawMode),
	texture(model.texture),
	VBO_vertices_id(model.VBO_vertices_id),
	VBO_normals_id(model.VBO_normals_id),
	VBO_indices_id(model.VBO_indices_id),
	VBO_color_id(model.VBO_color_id),
	VBO_texcoord_id(model.VBO_texcoord_id),
	VAO_id(model.VAO_id)
{
	if(model.texcoords)
		texcoords = new glm::vec2[numVertices];
	for(int i = 0; i < numPolygons; ++i)
		polygons[i] = model.polygons[i];
	for(int i = 0; i < numVertices; ++i)
	{
		vertices[i] = model.vertices[i];
		normals[i] = model.normals[i];
		if(texcoords)
			texcoords[i] = model.texcoords[i];
	}
}

Model::Model(int numVertices_, int numPolygons_, glm::vec3* vertices_, glm::uvec3* polygons_, GLint drawMode_) :
	numVertices(numVertices_),
	numPolygons(numPolygons_),
	vertices(new glm::vec3[numVertices_]),
	texcoords(NULL),
	polygons(new glm::uvec3[numPolygons]),
	drawMode(drawMode_),
	texture(0)
{
	for(int i = 0; i < numVertices; ++i)
		vertices[i] = vertices_[i];
	for(int i = 0; i < numPolygons; ++i)
		polygons[i] = polygons_[i];
	InitVBOs();
}

Model::Model(int numVertices_, int numPolygons_, glm::vec3* vertices_, glm::uvec3* polygons_, glm::vec2* texcoords_, GLint drawMode_, std::string texturepath) :
numVertices(numVertices_),
	numPolygons(numPolygons_),
	vertices(new glm::vec3[numVertices_]),
	texcoords(new glm::vec2[numVertices_]),
	polygons(new glm::uvec3[numPolygons_]),
	drawMode(drawMode_),
	texture(0)
{
	for(int i = 0; i < numVertices; ++i)
	{
		vertices[i] = vertices_[i];
		texcoords[i] = texcoords_[i];
	}

	for(int i = 0; i < numPolygons; ++i)
		polygons[i] = polygons_[i];

	InitVBOs();

	Init_Texture(texturepath);

}

Model& Model::operator=(const Model& rhs)
{
	if(this!=&rhs)
	{
		numVertices=rhs.numVertices;
		numPolygons=rhs.numPolygons;
		vertices=new glm::vec3[numVertices];
		polygons=new glm::uvec3[numPolygons];
		texcoords=NULL;
		normals=new glm::vec3[numVertices];
		drawMode=rhs.drawMode;
		texture=rhs.texture;
		VBO_vertices_id=rhs.VBO_vertices_id;
		VBO_normals_id=rhs.VBO_normals_id;
		VBO_indices_id=rhs.VBO_indices_id;
		VBO_color_id=rhs.VBO_color_id;
		VBO_texcoord_id=rhs.VBO_texcoord_id;
		VAO_id=rhs.VAO_id;
		if(rhs.texcoords)
			texcoords = new glm::vec2[numVertices];
		for(int i = 0; i < numPolygons; ++i)
			polygons[i] = rhs.polygons[i];
		for(int i = 0; i < numVertices; ++i)
		{
			vertices[i] = rhs.vertices[i];
			normals[i] = rhs.normals[i];
			if(texcoords)
				texcoords[i] = rhs.texcoords[i];
		}
	}
	return *this;
}

void Model::calculate_normals()
{
	normals = new glm::vec3[numVertices];	
	for(int i = 0; i < numVertices; ++i)
		normals[i] *= 0;

	glm::vec3 a_to_b, a_to_c;

	for(int i = 0; i < numPolygons; ++i)
	{
		a_to_b = vertices[polygons[i][1]] - vertices[polygons[i][0]];
		a_to_c = vertices[polygons[i][2]] - vertices[polygons[i][0]];

		glm::vec3 cross_product = glm::cross(a_to_b, a_to_c);
		cross_product = glm::normalize(cross_product);

		for(int j = 0; j < 3; ++j)
			normals[polygons[i][j]] += cross_product;
	}

	for(int i = 0; i < numVertices; ++i)
		normals[i] = glm::normalize(normals[i]);
}

void Model::Init_Texture(const std::string& texturepath)
{
/*	texture=loadDDSTexture(texturepath.c_str());

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
	glGenerateMipmap(GL_TEXTURE_2D);*/
}


void Model::InitVBOs()
{
	calculate_normals();
	// initialize VAO
	glGenVertexArrays(1, &VAO_id);
	glBindVertexArray(VAO_id);

	// initialize VBO for model vertices and polygon vertex indices
	glGenBuffers(1, &VBO_vertices_id);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices_id);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	// initialize VBO for normals of vertices
	glGenBuffers(1, &VBO_normals_id);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals_id);
	glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec3), glm::value_ptr(normals[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	if (texcoords != NULL)
	{
		glGenBuffers(1, &VBO_texcoord_id);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_texcoord_id);
		glBufferData(GL_ARRAY_BUFFER, numVertices*sizeof(glm::vec2), glm::value_ptr(texcoords[0]), GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), 0);
	}

	glGenBuffers(1, &VBO_indices_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO_indices_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numPolygons*sizeof(glm::uvec3), glm::value_ptr(polygons[0]), GL_STATIC_DRAW);

	glBindVertexArray(0);

}

GLuint Model::GetTexture() const
{
	return texture;
}

Model::~Model()
{
	delete[] vertices;
	delete[] normals;
	delete[] polygons;
}
