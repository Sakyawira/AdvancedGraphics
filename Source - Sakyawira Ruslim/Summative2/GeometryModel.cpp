#include "GeometryModel.h"

GeometryModel::GeometryModel(GLuint program, Camera* camera) 
{
	this->program = program; 
	this->camera = camera;
	GLfloat points[] = 
	{ 
		0.0f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //passing in 1 point 
	};
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

GeometryModel::~GeometryModel()
{

}

void GeometryModel::render(glm::vec3 position) 
{
	glUseProgram(this->program);
	glm::mat4 model;
	model = glm::translate(model, position);
	glm::mat4 mvp = camera->get_projection() * camera->get_view() * model;
	GLint vpLoc = glGetUniformLocation(program, "mvp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, 1);
	glBindVertexArray(0);
}
