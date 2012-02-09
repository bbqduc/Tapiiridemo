#pragma once

#include <GL3/gl3w.h>
#include <glm/glm.hpp>

struct Particle
{
	glm::vec3 position, velocity, acceleration;
	GLfloat timeLeft;
	void tick(GLfloat dt);
	void randomize();

	Particle() {acceleration = glm::vec3(0,-9.81f, 0); timeLeft = 100.0f;}
	Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, glm::vec3 initialAcceleration = glm::vec3(0,-9.81f,0));
};
