#include "shader.h"

void printShaderInfoLog(GLint shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	if (infoLogLen > 0)
	{
		infoLog = (GLchar*)malloc(sizeof(GLchar)*infoLogLen);

		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		printf("InfoLog:\n%s\n", infoLog);
		free(infoLog);
	}
}

int shaderInitialize(Shader* shader, const char* vertexPath, const char* fragmentPath, const char* geometryPath)
{
	assert(shader);

	int compileGeom = strlen(geometryPath);
	FILE* vertexFile = fopen(vertexPath, "rb"), // "b" is necessary because windows
		*fragmentFile = fopen(fragmentPath, "rb"),
		*geometryFile = compileGeom ? fopen(geometryPath, "r") : 0;

	fseek(vertexFile, 0L, SEEK_END);
	fseek(fragmentFile, 0L, SEEK_END);
	if(compileGeom) fseek(geometryFile, 0L, SEEK_END);

	GLint vertexSize = ftell(vertexFile), fragmentSize = ftell(fragmentFile), geometrySize = compileGeom ? ftell(geometryFile) : 0;

	fseek(vertexFile, 0L, SEEK_SET);
	fseek(fragmentFile, 0L, SEEK_SET);
	if(compileGeom) fseek(geometryFile, 0L, SEEK_SET);

	GLchar* vertexCode = (GLchar*)malloc(sizeof(GLchar)*vertexSize),
		*fragmentCode = (GLchar*)malloc(sizeof(GLchar)*fragmentSize),
		*geometryCode = compileGeom ? (GLchar*)malloc(sizeof(GLchar)*geometrySize) : 0;

	fread(vertexCode, sizeof(GLchar), vertexSize, vertexFile);
	fread(fragmentCode, sizeof(GLchar), fragmentSize, fragmentFile);
	if(compileGeom) fread(geometryCode, sizeof(GLchar), geometrySize, geometryFile);

	GLuint v,f,g;
	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);
	if(compileGeom) g = glCreateShader(GL_GEOMETRY_SHADER);

	const GLchar* vcode = vertexCode,
		*fcode = fragmentCode,
		*gcode = geometryCode;

	glShaderSource(v, 1, &vcode, &vertexSize);
	glShaderSource(f, 1, &fcode, &fragmentSize);
	if(compileGeom) glShaderSource(g, 1, &gcode, &geometrySize);

	GLint compiled;

	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("Vertex shader %s not compiled.\n", vertexPath);
		printShaderInfoLog(v);
		return -1;
	}
	printf("Vertex shader %s compiled succesfully.\n", vertexPath);

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		printf("Fragment shader %s not compiled.\n", fragmentPath);
		printShaderInfoLog(f);
		return -1;
	}
	printf("Fragment shader %s compiled succesfully.\n", fragmentPath);

	if(compileGeom)
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

	shader->id = glCreateProgram();
	glAttachShader(shader->id, v);
	glAttachShader(shader->id, f);
	if(compileGeom) glAttachShader(shader->id, g);

	glLinkProgram(shader->id);
	glUseProgram(shader->id);

	printShaderInfoLog(v);
	printShaderInfoLog(f);
	if(compileGeom) printShaderInfoLog(g);

	free(vertexCode); free(fragmentCode); free(geometryCode);
	fclose(vertexFile); fclose(fragmentFile); if(compileGeom) fclose(geometryFile);

	return 0;
}

