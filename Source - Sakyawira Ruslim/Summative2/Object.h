#pragma once
#include <glew.h>
#include <vector>
#include <gtc/type_ptr.hpp>

class Object
{
	//Square 1
public:
	Object() = default;
	~Object() = default;

	static void CreateObject(GLuint& VAO, GLuint& EBO, GLuint& VBO, const std::vector<GLuint>& indices, const std::vector<GLfloat>& vertices)
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
															// 3 is the size of each vertex
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * indices.size(), &indices.front(), GL_STATIC_DRAW);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
															// 3 is the size of each vertex
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * vertices.size()/3, &vertices.front(), GL_STATIC_DRAW);

		/// Position
		glVertexAttribPointer(
			0,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GLfloat),	// Stride of the single vertex (pos)
			(GLvoid*)0);			// Offset from beginning of Vertex

		glEnableVertexAttribArray(0);

		/// Color
		glVertexAttribPointer(
			1,
			3,
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GLfloat),	// Stride of the single vertex (pos)
			(GLvoid*)(3 * sizeof(GLfloat)));			// Offset from beginning of Vertex

		glEnableVertexAttribArray(1);


		/// Texture
		glVertexAttribPointer(
			2,
			2,									// 2 float components for coordinates 
			GL_FLOAT,
			GL_FALSE,
			8 * sizeof(GLfloat),				// Stride the single vertex (pos + color + tex)
			(GLvoid*)(6 * sizeof(GLfloat)));	// offset from beginning of Vertex
		glEnableVertexAttribArray(2);



		glBindVertexArray(0);
	}

	static void TransformObject(GLuint* program, glm::mat4 translationMatrix, glm::mat4 rotationZ, glm::mat4 scaleMatrix, glm::mat4 modelMatrix)
	{
	// Put translation uniform into '*program'
	GLuint translateLoc = glGetUniformLocation(*program, "translation");
	glUniformMatrix4fv(translateLoc, 1, GL_FALSE, glm::value_ptr(translationMatrix));

	// Put rotation uniform into '*program'
	GLuint rotationLoc = glGetUniformLocation(*program, "rotation");
	glUniformMatrix4fv(rotationLoc, 1, GL_FALSE, glm::value_ptr(rotationZ));

	// Put scaling uniform into '*program'
	GLuint scaleLoc = glGetUniformLocation(*program, "scale");
	glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, glm::value_ptr(scaleMatrix));

	
	// Put model uniform into '*program'
	GLuint modelLoc = glGetUniformLocation(*program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}

	static void PassTexture(GLuint* program, const std::vector<GLuint> v_texture)
	{
		// rendering texture "texture"
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, v_texture[0]);
		glUniform1i(glGetUniformLocation(*program, "tex"), 0);

		// Get texture to render
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, v_texture[1]);
		glUniform1i(glGetUniformLocation(*program, "tex1"), 1);
	}
};
