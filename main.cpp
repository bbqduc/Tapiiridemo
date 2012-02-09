#include <GL3/gl3w.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>

#include "shader.h"
#include "model.h"
#include "snd.h"


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
	if(!glfwOpenWindow(1024,768,0,0,0,0,0,0, GLFW_WINDOW))
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

	glBindBuffer(GL_ARRAY_BUFFER, triangle.VBO_vertices_id);
	glDrawElements(triangle.drawMode, triangle.numPolygons*3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	checkGLErrors("drawTimedTriangle");
}

void drawTimedTriangle(const ShaderWithTime& shader, const Model& triangle, float time)
{
	glUseProgram(shader.id);
	glUniform1f(shader.timeLocation, time);
	glBindVertexArray(triangle.VAO_id);

	glBindBuffer(GL_ARRAY_BUFFER, triangle.VBO_vertices_id);
	glDrawElements(triangle.drawMode, triangle.numPolygons*3, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
	glUseProgram(0);

	checkGLErrors("drawTimedTriangle");
}

int main()
{
	Snd s;
	s.loadMOD("test.xm");
	s.play();
	int running = GL_TRUE;

	if(init())
		return -1;

	glm::vec3 vertices[3];
	vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
	vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::uvec3 polygons(0,1,2);

	glm::vec3 vertices2[4];
	vertices2[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	vertices2[1] = glm::vec3(-1.0f, 1.0f, 0.0f);
	vertices2[2] = glm::vec3(1.0f, 1.0f, 0.0f);
	vertices2[3] = glm::vec3(1.0f, -1.0f, 0.0f);

	glm::uvec3 polygons2[2];
	polygons2[0] = glm::uvec3(0,1,2);
	polygons2[1] = glm::uvec3(2,3,0);

	Model triangle(3, 1, &vertices[0], &polygons, GL_TRIANGLES);
	Model fullScreenQuad(4,2, &vertices2[0], &polygons2[0], GL_TRIANGLES);
	printf("OpenGL version %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	ShaderWithTime plain;
	plain.initialize("shaders/timemover.vert", "shaders/music.frag", 0);
	checkGLErrors("beforemainloop");
	float time = 0.0f;
	float beat = 1.0f;
	while(running)
	{
		int pos=s.getMODPosition().second;
		time += 0.1f;
		if((pos%16)==0) (beat<1.0f)?(beat+=0.05f):beat=1.0f;
		else (beat>0)?beat-=0.0005f:beat=0.0f;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		drawTimedTriangle(plain, triangle, beat);
		glDisable(GL_BLEND);
		//drawTimedTriangle(plain, fullScreenQuad, time);
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
