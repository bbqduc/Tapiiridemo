#include "particle.h"

Particle::Particle(const glm::vec3& initialPosition, const glm::vec3& initialVelocity, glm::vec3 initialAcceleration)
	:position(initialPosition),
	velocity(initialVelocity),
	acceleration(initialAcceleration)
{
	timeLeft = 100.0f;
}


void Particle::tick(GLfloat dt)
{
	velocity += dt*acceleration;
	position += dt*velocity;
	timeLeft -= dt;
}

void Particle::randomize()
{
	position = glm::vec3(0.0f);
	velocity = glm::vec3((rand()%50-25)/10.0f, 4.0f+(rand()%30-15)/10.0f, (rand()%50-25)/10.0f);
}