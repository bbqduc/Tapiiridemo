#include "particle.h"

Particle::Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, glm::vec3 initialAcceleration)
	:position(initialPosition),
	velocity(initialVelocity),
	acceleration(initialAcceleration)
{}


void Particle::tick(GLfloat dt)
{
	velocity += dt*acceleration;
	position += dt*velocity;
}