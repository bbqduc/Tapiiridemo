#include <GL3/gl3w.h>
#include <GL/glfw.h>
#include <stdlib.h>
#include <stdio.h>


int main()
{
	int running = GL_TRUE;

	if( !glfwInit()) 	
	{
		printf("GLFWINIT FAILED!\n");
		exit( EXIT_FAILURE );
	}
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
	glfwOpenWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	if(!glfwOpenWindow(300,300,0,0,0,0,0,0, GLFW_WINDOW))
	{
		glfwTerminate();
		exit( EXIT_FAILURE );
	}
	if(gl3wInit()) 
	{
		printf("Gl3w init failed!\n");
		exit(-1);
	}
	if(!gl3wIsSupported(3,2))
	{
		printf("opengl 3.2 is not supported\n");
		exit(-1);
	}

	printf("OpenGL version %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));



	while(running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
	}

	glfwTerminate();

	exit(EXIT_SUCCESS);
}
