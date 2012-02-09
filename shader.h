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
	int initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath);
};

void printShaderInfoLog(GLint shader);













#endif
