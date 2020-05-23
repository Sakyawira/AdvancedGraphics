#pragma once
#include "Camera.h"
#include "Particle.h"
#include "Texture.h"
#include "Shader.h"
#include <string>

#define NUM_PARTICLES 128 * 20000 

class ParticleSystem 
{
	public: 
		ParticleSystem(glm::vec3 origin, Camera* _camera, Texture* _texture, Shader* _shader); 
		 ~ParticleSystem();
		 void init();
		 void render(float dt);
		 std::vector<Particle> particles; 
		 std::vector<glm::vec3> vPosition;
	private: 
		Camera* camera;  
		GLuint vao, vbo, texture, program; 
		float nParticles;

		std::vector<glm::vec4> initialposition;
		std::vector<glm::vec4> initialvelocity;
		GLuint posVbo, velVbo, initVelVbo, particleVao;
		GLuint computeProgram, renderProgram;

};