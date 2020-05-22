#include "Particle.h"

Particle::Particle(glm::vec3 _position, glm::vec3 _velocity, float _elapsed_time, float _speed, int _id, Camera* _camera)
{
	position = _position;
	origin = _position;
	velocity = _velocity;
	elapsed_time = _elapsed_time;
	speed = _speed;
	id = _id;
	camera = _camera;
}

void Particle::update(float dt)
{
	this->velocity.y += -0.2 * .0167f; 
	this->position += velocity; 
	this->elapsed_time -= .000167;
	if (this->elapsed_time <= 0.0f) 
	{
		this->position = this->origin; 
		this->velocity = glm::vec3(0.25 * cos(this->id * .0167) + 0.25f * randomFloat() - 0.125f, 
								   1.5f + 0.25f * randomFloat() - 0.125f,
								   0.25 * sin(this->id * .0167) + 0.25f * randomFloat() - 0.125f); 
		this->elapsed_time = randomFloat() + 0.125;
	}
	this->position += velocity;
	this->elapsed_time -= .000167;
	this->camDistance = glm::distance(this->camera->get_position(), this->position); //add
}

glm::vec3 Particle::get_position()
{
	return position;
}
