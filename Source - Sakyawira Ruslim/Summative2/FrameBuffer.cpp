#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(Shader* _shader, Mesh* _mesh)
{
	m_shader = _shader;
	m_mesh = _mesh;

	glGenTextures(1, &renderTexture);
	glBindTexture(GL_TEXTURE_2D, renderTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 800, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL); 

	glGenerateMipmap(GL_TEXTURE_2D);

	glGenFramebuffers(1, &frameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	// Attach texture to framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, // target buffer 
		GL_COLOR_ATTACHMENT0, // attachment, could be //GL_DEPTH_ATTACHMENT or //GL_STENCIL_ATTACHMENT 
		GL_TEXTURE_2D, // texture target type
		renderTexture, // texture 
		0); // level © 2005 - 2013 Media 

	glGenRenderbuffers(1, &rbo); 
	glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
	glRenderbufferStorage(GL_RENDERBUFFER, 
		// must be 
		GL_DEPTH24_STENCIL8, //use as depth - stencil buffer 
		1280, 
		720) //viewport width and height
		;
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, //target 
		GL_DEPTH_STENCIL_ATTACHMENT, //attachment 
		GL_RENDERBUFFER, //renderbufferTarget 
		rbo); // render buffer

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
	{ 
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl; 
	}


}

FrameBuffer::~FrameBuffer()
{

}

void FrameBuffer::PrepareRender()
{
	// Bind our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
	glEnable(GL_DEPTH_TEST); // Depth is enabled to capture it
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FrameBuffer::Render(const GLchar* s_currentTime, GLfloat f_currentTime)
{

	
	/*
	render objects here
	*/

	// Bind Default framebuffer 
	glBindFramebuffer(GL_FRAMEBUFFER, 0); 
	glDisable(GL_DEPTH_TEST); //screenspace quad so depth is not required
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); 
	glClear(GL_COLOR_BUFFER_BIT);

	// glUseProgram(program);
	m_shader->Activate();
	m_shader->PassUniform(s_currentTime, f_currentTime);
	glActiveTexture(GL_TEXTURE0); 
	glUniform1i(glGetUniformLocation(m_shader->GetProgram(), "renderTexture"), 0); 
	glBindTexture(GL_TEXTURE_2D, renderTexture);

	//glBindVertexArray(quadVAO); 
	m_mesh->Bind();
	glDrawArrays(GL_TRIANGLES, 0, 6); glBindVertexArray(0);

}
