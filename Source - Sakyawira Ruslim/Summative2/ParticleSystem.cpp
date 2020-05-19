#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(glm::vec3 origin, Camera* _camera, std::string texFileName)
{
	nParticles = 4000; 
	for (int i = 0; i < nParticles; i++) 
	{
		vPosition.push_back(glm::vec3(0.0));
		//initialize position vector 
		Particle p = Particle( 
			origin,																// pos 
			glm::vec3(0.25 * cos(i * .0167) + 0.25f * randomFloat() - 0.125f,  
					  2.0f + 0.25f * randomFloat() - 0.125f, 
					  0.25 * sin(i* .0167) + 0.25f * randomFloat() - 0.125f),	// vel
			randomFloat() + 0.125,												// elapsed time 
			1.0f,																// speed 
			i,																	// id 
			_camera);   
		particles.push_back(p);													// add 
	}
}
