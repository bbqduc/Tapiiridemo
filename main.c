#include <GL3/gl3w.h>
#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>
#include "shader.h"


GLfloat vertices[] = {-1.0f, -1.0f, 0.0f,
					1.0f, -1.0f, 0.0f,
					0.0f, 1.0f, 0.0f};
GLuint vertexBuffer;

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
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);
	
	glGenBuffers(1, &vertexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices[0], GL_STATIC_DRAW);

	checkGLErrors("init");

	return 0;
}

void drawSimpleTriangle(const Shader* shader)
{
	glUseProgram(shader->id);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glUseProgram(0);
	checkGLErrors("drawSimpleTriangle");
}

int main()
{
	int running = GL_TRUE;

	if(init())
		return -1;

	printf("OpenGL version %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	Shader plain;
	shaderInitialize(&plain, "shaders/plain.vert", "shaders/plain.frag", "");
	checkGLErrors("beforemainloop");
	while(running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		drawSimpleTriangle(&plain);
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	glfwTerminate();

	exit(EXIT_SUCCESS);
}
