#include "glutils.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#ifdef __APPLE__
	#include <OpenGL/glu.h>
#else
	#include <GL/glu.h>
#endif

void checkGLErrors(const char* functionName)
{
	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		fprintf(stderr, "%s -- ERROR: %s\n", functionName, gluErrorString(err));
		exit(-1);
	}
}

void renderToTexture(GLuint texID, GLint textarget, GLuint dimx, GLuint dimy, GLuint framebufferID)
{
	glBindTexture(textarget, texID);
	glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
	glFramebufferTexture1D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textarget, texID, 0);

	if(glCheckFramebufferStatus(framebufferID) != GL_FRAMEBUFFER_COMPLETE)
		std::cerr << "FRAMEBUFFER ERROR -- ABANDON SHIP!\n";

	glViewport(0,0,dimx,dimy);
	glClear(GL_COLOR_BUFFER_BIT); // Maybe not here?
}

void renderToScreen(GLint* viewPort)
{
	glBindBuffer(GL_FRAMEBUFFER, 0);
	glViewport(viewPort[0], viewPort[1], viewPort[2], viewPort[3]);
}
