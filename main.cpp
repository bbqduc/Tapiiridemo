#include <sstream>
#include <GL3/gl3w.h>
#include <GL/glfw.h>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <ctime>
#include <cmath>
#include <cstdlib>

#include "glutils.h"
#include "snd.h"
#include "shader.h"
#include "model.h"
#include "particle.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "concurrency.h"
#include "oclfile.h"


bool emitParticles = false;


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
	glm::mat4 rotate = glm::rotate(glm::mat4(), (float)sin(time/20)*(float)cos(time/20)*360.0f, glm::vec3(sin(time/20), cos(time/20), (sin(time/20)*cos(time/20))/2));
	glm::mat4 cam = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -((sin(time/10)+2.0f)*6.0f)));

	glm::mat4 result = perspective * cam * rotate * MVP;

	glUseProgram(shader.id);
	glUniform1f(shader.timelocation, time);
	glUniformMatrix4fv(shader.MVPLocation, 1, GL_FALSE, glm::value_ptr(result));

	glBindVertexArray(point.VAO_id);
	glBindBuffer(GL_ARRAY_BUFFER, point.VBO_vertices_id);
	glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

	glUseProgram(0);

	checkGLErrors("drawParticle");
}

void drawParticles(const std::list<Particle>& particles, const ShaderWithMVP& shader, float time, float* basecolor, OCLProg& prog)
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glm::mat4 perspective = glm::perspective(45.0f, 1024.0f/768.0f, 1.0f, 100.0f);
	glm::mat4 rotate = glm::rotate(glm::mat4(), (float)sin(time/20)*(float)cos(time/20)*360.0f, glm::vec3(sin(time/20), cos(time/20), (sin(time/20)*cos(time/20))/2));
	glm::mat4 cam = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.0f, -50.0f));

	glm::mat4 result = perspective * cam;// * rotate;

	glUseProgram(shader.id);
	glUniform1f(shader.timelocation, time);
	glUniform4fv(shader.basecolorpos, 1, basecolor);
	glUniformMatrix4fv(shader.MVPLocation, 1, GL_FALSE, glm::value_ptr(result));

	glBindVertexArray(prog.posVAOid);
	glDrawArrays(GL_POINTS, 0, prog.vecLen);

	glUseProgram(0);
	glDisable(GL_BLEND);
	checkGLErrors("drawParticles");
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
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	drawTimedTriangle(shader, triangle, beat);
	glDisable(GL_BLEND);
}

struct threaddata
{
	C_Mutex* mtx;
	int* running;
	int* effectid;
	OCLProg* prog;
};

void sync(void* args)
{
	C_CondVar* c=(C_CondVar*)args;
	c->M_Signal();
}

void listentomusic(void* args)
{
	threaddata* d=(threaddata*)args;
	int* running=d->running;
	C_Mutex* mtx=d->mtx;
	C_CondVar c;
	Snd s;
	s.loadMOD("test.xm");
	for(int i=0; i<64; i+=8) s.syncPosition(sync, -1, i, &c);
	s.play();
	while(*running)
	{
		c.M_Wait();
		mtx->M_Lock();
		emitParticles = true;
		mtx->M_Unlock();
	}
}

void drawFramebuffer(const Framebuffer& fb, const ShaderPostProcessing& shader, const Model& model, float time)
{
	glUseProgram(shader.id);
	glBindTexture(GL_TEXTURE_2D, fb.fbo_texture);
	glUniform1f(shader.timelocation, time);
	glUniform1i(shader.texturelocation, 0);
	glBindVertexArray(model.VAO_id);
	glBindBuffer(GL_ARRAY_BUFFER, model.VBO_vertices_id);
	glDrawElements(model.drawMode, model.numPolygons*3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glUseProgram(0);
}

void drawTunnel(const ShaderTunnel& tunnelShader, float time, float tunnelDistance, const Model& model)
{
	glUseProgram(tunnelShader.id);

	checkGLErrors("drawTunnel1");
	glUniform1f(tunnelShader.timeLocation, time);
	checkGLErrors("drawTunnel2");
	glUniform1f(tunnelShader.tunnelDistancelocation, tunnelDistance);
	checkGLErrors("drawTunnel3");
	glBindVertexArray(model.VAO_id);
	checkGLErrors("drawTunnel4");
	glBindBuffer(GL_ARRAY_BUFFER, model.VBO_vertices_id);
	checkGLErrors("drawTunnel5");
	glDrawElements(model.drawMode, model.numPolygons*3, GL_UNSIGNED_INT, 0);
	checkGLErrors("drawTunnel6");
	glBindVertexArray(0);
	checkGLErrors("drawTunnel7");
	glUseProgram(0);

	checkGLErrors("drawTunnel");

}

int main(int argc, char** argv)
{
	unsigned int WGSize = 256;
	if(argc > 1)
	{
		std::stringstream herp;
		herp << argv[1];
		herp >> WGSize;
	}

	srand(time(0));
	int running = GL_TRUE;

	if(init())
		return -1;
	printf("OpenGL version %s, GLSL %s\n", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	OCLProg prog("simulation.cl", WGSize);

	Model triangle = simpleTriangleModel();
	Model fullScreenQuad = fullScreenQuadModel();
	Model point = pointModel();

	std::list<Particle> particles;

	Shader realPlain;
	realPlain.initialize("shaders/plain.vert", "shaders/realplain.frag", 0);

	ShaderWithTime plain;
	plain.initialize("shaders/timemover.vert", "shaders/music.frag", 0);
	ShaderTunnel tunnel;
	tunnel.initialize("shaders/plain.vert", "shaders/tunnel.frag", 0);
	ShaderWithMVP pointShader;
	pointShader.initialize("shaders/plainMVP.vert", "shaders/plain.frag", "shaders/pointToSquare.geom");
	ShaderPostProcessing post;
	post.initialize("shaders/post.vert", "shaders/post.frag", "");
	checkGLErrors("beforemainloop");


	float time = 0.0f;

	C_Mutex mtx;
	int effectid=0;
	threaddata d={&mtx, &running, &effectid};
	C_Thread music(listentomusic, &d);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Framebuffer postprocessing(1024, 768);

	float basecolor[4] = {2.0f, 2.0f, 2.0f, 1.0f};

	float tunnelDistance = 0.00001f;
	int counter = 0;
	while(running)
	{
		if(tunnelDistance > 0.2f)
			prog.simulate(0.001f);
		mtx.M_Lock();
		if(emitParticles)
		{
			emitParticles=false;
			for(int i = 0; i < 3; ++i) basecolor[i] = 1.0f;
		}
		mtx.M_Unlock();
		time += 0.01f;
		
		for(int i = 0; i < 3; ++i) basecolor[i] /= 1.15f;

		glBindFramebuffer(GL_FRAMEBUFFER, postprocessing.fb);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		tunnelDistance *= 1.05;
		basecolor[3]=tunnelDistance/16;

		if(tunnelDistance < 1.0f)
			drawTunnel(tunnel, time, tunnelDistance, fullScreenQuad);
		if(tunnelDistance > 0.2f)
			drawParticles(particles, pointShader, time, basecolor, prog);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		drawFramebuffer(postprocessing, post, fullScreenQuad, time);
		glfwSwapBuffers();
		running = !glfwGetKey(GLFW_KEY_ESC) && glfwGetWindowParam(GLFW_OPENED);
		glfwSleep(0.01);
	}
	music.M_Join();
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
