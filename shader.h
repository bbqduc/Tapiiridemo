#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <GL3/gl3w.h>
#include <GL/glfw.h>
#include <assert.h>

struct Shader
{
	GLuint id;
	GLint readShaderSource(const char* path, char** target);
	int initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
};

struct ShaderWithTime : public Shader
{
	GLint timeLocation;
	int initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
};

struct ShaderWithMVP : public Shader
{
	GLint MVPLocation;
	GLint timelocation;
	GLint colorpos;
	int initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
};

struct Framebuffer
{
	bool initialized;
	GLuint fb;
	GLuint fbo_texture;
	GLuint rbo_depth;
	GLuint vertices;
	Framebuffer() : initialized(false), fb(0), fbo_texture(0), rbo_depth(0), vertices(0) {}
	Framebuffer(unsigned int w, unsigned int h);
	~Framebuffer()
	{
		if(initialized)
		{
			glDeleteRenderbuffers(1, &rbo_depth);
			glDeleteTextures(1, &fbo_texture);
			glDeleteFramebuffers(1, &fb);
			glDeleteBuffers(1, &vertices);
		}
	}
};

void printShaderInfoLog(GLint shader);

#endif
