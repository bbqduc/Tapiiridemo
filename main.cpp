#include <GL3/gl3w.h>
#include <GL/glfw.h>
#include <GL/gl.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <ctime>
#include <cstdlib>

#include "glutils.h"
#include "snd.h"
#include "shader.h"
#include "model.h"
#include "particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




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
		printf("Cannot open window!\n");
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

void drawParticle(const ShaderWithMVP& shader, const Particle& particle, const Model& point, float time, int pos)
{
	glm::mat4 perspective = glm::perspective(45.0f, 1024.0f/768.0f, 1.0f, 1000.0f);
	glm::mat4 MVP = glm::translate(glm::mat4(), particle.position);
	glm::mat4 rotate = glm::rotate(glm::mat4(), time, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 cam = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -5.0f));

	glm::mat4 result = perspective * MVP * cam;

	glUseProgram(shader.id);
	glUniform1f(shader.timelocation, time);
	glUniform1i(shader.colorpos, pos);
	glUniformMatrix4fv(shader.MVPLocation, 1, GL_FALSE, glm::value_ptr(result));

	glBindVertexArray(point.VAO_id);
	glBindBuffer(GL_ARRAY_BUFFER, point.VBO_vertices_id);
	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

	glUseProgram(0);

	checkGLErrors("drawParticle");
}

void emitParticles(std::list<Particle>& particles, int amount)
{
	for(int i = 0; i < amount; ++i)
	{
		particles.push_back(Particle());
		particles.back().randomize();
	}
}

void tickParticles(std::list<Particle>& particles, GLfloat dt)
{
	for(auto i = particles.begin(); i != particles.end();)
	{
		if(i->position.y < -5.0f)
		{
			auto j = i; ++i; particles.erase(j, i);
		}
		i->tick(dt);
		++i;
	}
}

void drawParticles(const std::list<Particle>& particles, const ShaderWithMVP& shader, Model& point, float time, int pos)
{	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for(auto i = particles.begin(); i != particles.end(); ++i)
		drawParticle(shader, *i, point, time, pos);
	glDisable(GL_BLEND);
}

Model simpleTriangleModel()
{
	glm::vec3 vertices[3];
	vertices[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	vertices[1] = glm::vec3(1.0f, -1.0f, 0.0f);
	vertices[2] = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::uvec3 polygons(0,1,2);
	return Model(3, 1, &vertices[0], &polygons, GL_TRIANGLES);
}

Model pointModel()
{
	glm::vec3 point = glm::vec3(0,0,0);
	glm::uvec3 pointPolygon = glm::uvec3(0,0,0);

	return Model(1,1, &point, &pointPolygon, GL_POINTS);
}

Model fullScreenQuadModel()
{
	glm::vec3 vertices2[4];
	vertices2[0] = glm::vec3(-1.0f, -1.0f, 0.0f);
	vertices2[1] = glm::vec3(-1.0f, 1.0f, 0.0f);
	vertices2[2] = glm::vec3(1.0f, 1.0f, 0.0f);
	vertices2[3] = glm::vec3(1.0f, -1.0f, 0.0f);

	glm::uvec3 polygons2[2];
	polygons2[0] = glm::uvec3(0,1,2);
	polygons2[1] = glm::uvec3(2,3,0);

	return Model(4,2, &vertices2[0], &polygons2[0], GL_TRIANGLES);
}

void drawPulsingTriangle(const ShaderWithTime& shader, const Model& triangle, float beat)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawTimedTriangle(shader, triangle, beat);
	glDisable(GL_BLEND);
}

int main()
{
	srand(time(0));
	Snd s;
	s.loadMOD("test.xm");
	s.play();
	int running = GL_TRUE;

	if(init())
		return -1;
	printf("OpenGL version %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	Model triangle = simpleTriangleModel();
	Model fullScreenQuad = fullScreenQuadModel();
	Model point = pointModel();

	std::list<Particle> particles;

	ShaderWithTime plain;
	plain.initialize("shaders/timemover.vert", "shaders/music.frag", 0);
	ShaderWithMVP pointShader;
	pointShader.initialize("shaders/plainMVP.vert", "shaders/plain.frag", "shaders/pointToSquare.geom");
	checkGLErrors("beforemainloop");
	float time = 0.0f;
	unsigned int pos=0;
	bool herp=false;
	while(running)
	{
		if(s.get4th())
		{
			if(herp)
			{
				emitParticles(particles, 100);
				pos++;
				herp=false;
			}
		}
		else herp=true;

		tickParticles(particles, 0.01f);
		time += 0.1f;
			
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//drawTimedTriangle(plain, triangle, beat);
		drawParticles(particles, pointShader, point, s.get4th(), pos%3);
		//drawPulsingTriangle(plain, triangle, beat);
		//drawTimedTriangle(plain, fullScreenQuad, time);
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
		glfwSleep(0.01);
	}

	glfwTerminate();
	exit(EXIT_SUCCESS);
}
