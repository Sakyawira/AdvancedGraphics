#include "ShadowMap.h"

ShadowMap::ShadowMap(Shader* _shadowmapShader)
{
	shadowMapProgram = _shadowmapShader->GetProgram();
}

void ShadowMap::init()
{
	glGenTextures(1, &depthMapTexture); 
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, //mipmap level 
				 GL_DEPTH_COMPONENT, //internal format 
				 1280, //screen width 
				 720, //screen height 
				 0, //border
				 GL_DEPTH_COMPONENT, //color format 
				 GL_FLOAT, //data type 
				 NULL);

	glGenFramebuffers(1, &depthMapFBO); 
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO); // attach depth texture as FBO's depth buffer 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	//disable writes to color buffer 
	glDrawBuffer(GL_NONE); 
	glReadBuffer(GL_NONE); 
	glBindFramebuffer(GL_FRAMEBUFFER, 0); // unbind buffer

	//Check if framebuffer is built properly 
	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (Status != GL_FRAMEBUFFER_COMPLETE) { printf("FB error, status: 0x%x\n", Status); }
}

void ShadowMap::start()
{
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO); 
	glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::end()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void ShadowMap::ShadowMapPass(glm::mat4 modelMatrix, Camera* camera, GLuint indices_size, GLuint vao)
{
	glUseProgram(this->shadowMapProgram);
	glm::mat4 lightViewMatrix = glm::lookAt(lightPosition, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 lightVPMatrix = camera->get_projection() * lightViewMatrix;

	GLint vpLoc = glGetUniformLocation(shadowMapProgram, "lightVPMatrix");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(lightVPMatrix));
	// model matrix of the current object 
	GLint modelLoc = glGetUniformLocation(shadowMapProgram, "model"); 
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	// draw the current object 
	glBindVertexArray(vao); 
	glDrawElements(GL_TRIANGLES, indices_size, GL_UNSIGNED_INT, 0); glBindVertexArray(0);
}
