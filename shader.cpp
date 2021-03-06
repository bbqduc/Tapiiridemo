#include "shader.h"
#include <string.h>
#include <iostream>
#include "glutils.h"
	
void printShaderInfoLog(GLint shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 1)
	{
		infoLog = (GLchar*)malloc(sizeof(GLchar)*infoLogLen);

		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		printf("InfoLog:\n%s\n", infoLog);
		free(infoLog);
	}
}

Framebuffer::Framebuffer(unsigned int w, unsigned int h)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);

	/*
	glGenRenderbuffers(1, &rbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h); // width&height
	*/

	glGenFramebuffers(1, &fb);
	glBindFramebuffer(GL_FRAMEBUFFER, fb);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo_depth);
	GLenum status;
	if((status=glCheckFramebufferStatus(GL_FRAMEBUFFER)) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer incomplete :<, status: " << status << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	initialized=true;
}

GLint Shader::readShaderSource(const char* path, GLchar** target)
{
	if(!path)
		return 0;
	FILE* file = fopen(path, "rb");
	if(!file)
		return 0;
	fseek(file, 0L, SEEK_END);
	GLint size = ftell(file);
	fseek(file, 0L, SEEK_SET);

	*target = (GLchar*)malloc(sizeof(GLchar)*size);
	fread(*target, sizeof(GLchar), size, file);

	fclose(file);

	return size;
}

int Shader::initialize()
{
	initialize(vPath.c_str(), fPath.c_str(), gPath.c_str());
	return 0;
}

int Shader::initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	if(vertexPath) vPath=vertexPath;
	if(fragmentPath) fPath=fragmentPath;
	if(geometryPath) gPath=geometryPath;
	GLchar* vertexSource = 0, *fragmentSource = 0, *geometrySource = 0;
	GLint vertexSize = 0, fragmentSize = 0, geometrySize = 0;
	vertexSize = readShaderSource(vertexPath, &vertexSource);
	fragmentSize = readShaderSource(fragmentPath, &fragmentSource);
	geometrySize = readShaderSource(geometryPath, &geometrySource);

	GLuint v,f,g;

	if(vertexSize) v = glCreateShader(GL_VERTEX_SHADER);
	if(fragmentSize) f = glCreateShader(GL_FRAGMENT_SHADER);
	if(geometrySize) g = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* vcode = vertexSource, // need consts for opengl
		*fcode = fragmentSource,
		*gcode = geometrySource;

	if(vertexSize) glShaderSource(v, 1, &vcode, &vertexSize);
	if(fragmentSize) glShaderSource(f, 1, &fcode, &fragmentSize);
	if(geometrySize) glShaderSource(g, 1, &gcode, &geometrySize);

	GLint compiled;

	if(vertexSize)
	{
		glCompileShader(v);
		glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			printf("Vertex shader %s not compiled.\n", vertexPath);
			printShaderInfoLog(v);
			return -1;
		}
		printf("Vertex shader %s compiled succesfully.\n", vertexPath);
	}

	if(fragmentSize)
	{
		glCompileShader(f);
		glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			printf("Fragment shader %s not compiled.\n", fragmentPath);
			printShaderInfoLog(f);
			return -1;
		}
		printf("Fragment shader %s compiled succesfully.\n", fragmentPath);
	}

	if(geometrySize)
	{
		glCompileShader(g);
		glGetShaderiv(g, GL_COMPILE_STATUS, &compiled);
		if (!compiled)
		{
			printf("Geometry shader %s not compiled.\n", geometryPath);
			printShaderInfoLog(g);
			return -1;
		}
		printf("Geometry shader %s compiled succesfully.\n", geometryPath);
	}

	id = glCreateProgram();
	if(vertexSize) glAttachShader(id, v);
	if(fragmentSize) glAttachShader(id, f);
	if(geometrySize)
	{
		glAttachShader(id, g);
		/*checkGLErrors("geometry shader parameter passing1");
		glProgramParameteri(id, GL_GEOMETRY_INPUT_TYPE, GL_POINTS);
		checkGLErrors("geometry shader parameter passing2");
		glProgramParameteri(id, GL_GEOMETRY_OUTPUT_TYPE, GL_POINTS);
		checkGLErrors("geometry shader parameter passing3");
		glProgramParameteri(id, GL_GEOMETRY_VERTICES_OUT, 1);
		checkGLErrors("geometry shader parameter passing4");*/
	}



	glLinkProgram(id);
	glUseProgram(id);

	if(vertexSize) printShaderInfoLog(v);
	if(fragmentSize) printShaderInfoLog(f);
	if(geometrySize) printShaderInfoLog(g);

	free(vertexSource); free(fragmentSource); free(geometrySource);

	return 0;
}

int ShaderWithTime::initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	Shader::initialize(vertexPath, fragmentPath, geometryPath);
	timeLocation = glGetUniformLocation(id, "time");
	return 0;
}

int ShaderWithMVP::initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	Shader::initialize(vertexPath, fragmentPath, geometryPath);
	MVPLocation = glGetUniformLocation(id, "MVP");
	timelocation = glGetUniformLocation(id, "time");
	colorpos = glGetUniformLocation(id, "pos");
	return 0;
}

int ShaderPostProcessing::initialize(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	Shader::initialize(vertexPath, fragmentPath, geometryPath);
	timelocation = glGetUniformLocation(id, "time");
	texturelocation = glGetUniformLocation(id, "texture");
	return 0;
}
