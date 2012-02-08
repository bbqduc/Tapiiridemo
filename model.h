#ifndef MODEL_H
#define MODEL_H

#include <GL3/gl3w.h>
#include <string>
#include <stdint.h>
#include <glm/glm.hpp>

class Model
{
	public:	int numVertices, numPolygons;
	glm::vec3* vertices, *normals;
	glm::vec2* texcoords;
	glm::uvec3* polygons;

	GLint drawMode;
	GLuint texture;
	GLuint VBO_vertices_id, VBO_normals_id, VBO_indices_id, VBO_color_id, VBO_texcoord_id, VAO_id;

	void calculate_normals();
	void InitVBOs();
	void Init_Texture(const std::string& texturepath);

	public:
	Model();
	Model(const Model&);
	Model(int num_vertices, int num_polygons, glm::vec3 *vertices, glm::uvec3 *polygons, GLint drawMode = GL_TRIANGLES);
	Model(int num_vertices, int num_polygons, glm::vec3 *vertices, glm::uvec3 *polygons, glm::vec2 *texcoords, GLint drawMode, std::string texturepath);
	Model& operator=(const Model& rhs);
	~Model();
	GLuint GetTexture() const;
};



#endif
