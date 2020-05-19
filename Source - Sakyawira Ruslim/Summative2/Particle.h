#pragma once
// Dependency Includes
#include <glew.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <mat4x2.hpp>
// Library Includes
#include <random> 
// Local Includes
#include "Camera.h"
static float randomFloat() { float r = (float)rand() / (double)RAND_MAX; return r; }

class Particle 
{
	public:
		Particle(glm::vec3 _position, glm::vec3 _velocity, float _elapsed_time, float _speed, int _id, Camera* _camera);
		void update(float dt);

	private:
		Camera* camera;

		glm::vec3 origin;
		glm::vec3 position;
		glm::vec3 velocity;

		int id;

		float elapsed_time;
		float speed;
};