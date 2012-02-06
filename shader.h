#ifndef SHADER_H
#define SHADER_H

#include <stdio.h>
#include <stdlib.h>
#include <GL3/gl3w.h>
#include <GL/glfw.h>
#include <assert.h>

typedef struct
{
	GLuint id;
} Shader;

void printShaderInfoLog(GLint shader);
int shaderInitialize(Shader* shader, const char* vertexPath, const char* fragmentPath, const char* geometryPath);













#endif
