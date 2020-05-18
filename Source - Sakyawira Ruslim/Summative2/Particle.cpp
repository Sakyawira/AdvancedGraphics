#include "Particle.h"

Particle::Particle(glm::vec3 _position)
{
	position = _position;
}

void Particle::update(float dt)
{
	this->velocity.y += -0.2 * .0167f; 
	this->position += velocity; 
	this->elapsed_time -= .000167;
	if (this->elapsed_time <= 0.0f) 
	{
		this->position = this->origin; this->velocity = glm::vec3(0.25 * cos(this->id * .0167) + 0.25f * randomFloat() - 0.125f, 1.5f + 0.25f * randomFloat() - 0.125f, 0.25 * sin(this->id * .0167) + 0.25f * randomFloat() - 0.125f); this->elapsedTime = randomFloat() + 0.125;
	}

}
