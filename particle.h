#pragma once

#include <GL3\gl3w.h>
#include <glm\glm.hpp>

struct Particle
{
	glm::vec3 position, velocity, acceleration;

	void tick(GLfloat dt);
	Particle() {acceleration = glm::vec3(0,-9.81f, 0);}
	Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, glm::vec3 initialAcceleration = glm::vec3(0,-9.81f,0));
};