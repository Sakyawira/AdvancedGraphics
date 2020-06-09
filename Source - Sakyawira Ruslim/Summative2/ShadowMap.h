#pragma once
#include <glew.h>
#include <SOIL.h>
#include <vector>
#include <gtc/type_ptr.hpp>
#include "Shader.h"
#include "Mesh.h"
class ShadowMap
{
	public:
		ShadowMap(Shader* _shadowmapShader);
		~ShadowMap() = default;
		void init();
		void start();
		void end();
		void ShadowMapPass(glm::mat4 modelMatrix, Camera* camera, GLuint indices_size, GLuint vao);
		GLuint GetTexture() { return depthMapTexture; }		
		GLuint GetProgram() { return shadowMapProgram; }
		glm::mat4 GetLightViewMatrix() { return lightViewMatrix; }
		glm::vec3 GetLightPos() { return lightPosition; }

	private:
		// depth texture 
		GLuint depthMapTexture;
		GLuint depthMapFBO; // global variable 
		GLuint shadowMapProgram;

		glm::mat4 lightViewMatrix = {};
		glm::vec3 lightPosition = { 0.0f, 0.0f, 260.0f };
};