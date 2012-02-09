#include <GL3/gl3w.h>
#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>

#include "shader.h"
#include "model.h"


void checkGLErrors(const char* functionName){
	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		fprintf(stderr, "%s -- ERROR: %s\n", functionName, gluErrorString(err));
		exit(-1);
	}
}

int init()
{
	if( !glfwInit()) 	
	{
		printf("GLFWINIT FAILED!\n");
		return -1;
	}
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if(!glfwOpenWindow(300,300,0,0,0,0,0,0, GLFW_WINDOW))
	{
		glfwTerminate();
		return -1;
	}
	if(gl3wInit()) 
	{
		printf("Gl3w init failed!\n");
		return -1;
	}
	if(!gl3wIsSupported(3,2))
	{
		printf("opengl 3.2 is not supported\n");
		return -1;
	}

	checkGLErrors("init");

	return 0;
}

void drawSimpleTriangle(const Shader& shader, const Model& triangle)
{
	glUseProgram(shader.id);
	glBindVertexArray(triangle.VAO_id);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, triangle.VBO_vertices_id);
	glVertexAttribPointer(0, triangle.numVertices, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, triangle.numVertices);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glUseProgram(0);

	checkGLErrors("drawSimpleTriangle");
}

void drawTimedTriangle(const ShaderWithTime& shader, const Model& triangle, float time)
{
	glUseProgram(shader.id);
	glUniform1f(shader.timeLocation, time);
	glBindVertexArray(triangle.VAO_id);

	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, triangle.VBO_vertices_id);
	glVertexAttribPointer(0, triangle.numVertices, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, triangle.numVertices);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glUseProgram(0);

	checkGLErrors("drawSimpleTriangle");
}

int main()
{
	int running = GL_TRUE;

	if(init())
		return -1;

	glm::vec3 vertices[3];
	vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
	vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::uvec3 polygons(0,1,2);

	Model triangle(3, 1, &vertices[0], &polygons, GL_TRIANGLES);
	printf("OpenGL version %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	ShaderWithTime plain;
	plain.initialize(0, "shaders/disco.frag", 0);
	checkGLErrors("beforemainloop");
	float time = 0.0f;
	while(running)
	{
		time += 0.001f;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawTimedTriangle(plain, triangle, time);
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
