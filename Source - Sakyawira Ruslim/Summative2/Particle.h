#pragma once
// Dependency Includes
#include <glew.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <mat4x2.hpp>
// Library Includes
#include <random> 
static float randomFloat() { float r = (float)rand() / (double)RAND_MAX; return r; }

class Particle 
{
	public:
		Particle(glm::vec3 _position);
		void update(float dt);

	private:
		glm::vec3 position;
		glm::vec3 velocity;
		float elapsed_time;
		float speed;
};