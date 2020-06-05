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
		glm::mat4 GetLightViewMatrix() { return lightViewMatrix; }

	private:
		// depth texture 
		GLuint depthMapTexture;
		GLuint depthMapFBO; // global variable 
		GLuint shadowMapProgram;

		glm::mat4 lightViewMatrix = {};
		glm::vec3 lightPosition = { -70.0f, 70.0f, 70.0f };
};