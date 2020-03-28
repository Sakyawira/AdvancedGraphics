/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   GameObject.h
  Description :   contains definition of GameObject class which control the updates and drawing pipeline of an object
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#include "CubeMap.h"


#include "GameObject.h"

GameObject::GameObject(Shader* _shader, Mesh* _mesh, std::vector<Texture*>& _textures, float _initial_x, float _initial_y, float _initial_z, std::vector<GameObject*>& objectVector)
{
	m_shader = _shader;
	m_mesh = _mesh;
	m_xPos = _initial_x;
	m_yPos = _initial_y;
	m_zPos = _initial_z;
	// This creates a copy (even though was passed a s a reference) and therefore did not work
	// m_camera = _camera;
	m_textures = _textures;

	m_objPosition = glm::vec3(m_xPos, m_yPos, m_zPos);
	m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;

	objectVector.push_back(this);
}

GameObject::GameObject(Model * _model, float _initial_x, float _initial_y, float _initial_z)
{
	m_xPos = _initial_x;
	m_yPos = _initial_y;
	m_zPos = _initial_z;
	
	m_model = _model;

	m_objPosition = glm::vec3(m_xPos, m_yPos, m_zPos);
	m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

void GameObject::Draw(Camera& _camera, const GLchar* s_currentTime, GLfloat f_currentTime, float deltaTime)
{
	if (m_bool_enable)
	{
		m_shader->Activate();

		m_shader->PassUniform(s_currentTime, f_currentTime);

		// Passing texture to 'program'
		m_shader->PassTexture(m_textures);

		// Transform the objects controlled by 'program'
		m_shader->Transform(m_translationMatrix, m_rotationZ, m_scaleMatrix, m_modelMatrix, _camera);

		// Using camera on the program
		_camera.UseCamera(m_shader->GetProgram());

		// Hexagon 1
		m_mesh->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh->GetSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void GameObject::Draw(Camera & _camera, const GLchar * s_currentTime, GLfloat f_currentTime, const GLchar * s_frameTime, GLint i_frameTime, float deltaTime)
{
	if (m_bool_enable)
	{
		m_shader->Activate();

		m_shader->PassUniform(s_currentTime, f_currentTime);

		m_shader->PassFrameUniform(s_frameTime, i_frameTime);

		// Passing texture to 'program'
		m_shader->PassTexture(m_textures);

		// Transform the objects controlled by 'program'
		m_shader->Transform(m_translationMatrix, m_rotationZ, m_scaleMatrix, m_modelMatrix, _camera);

		// Using camera on the program
		_camera.UseCamera(m_shader->GetProgram());

		// Hexagon 1
		m_mesh->Bind();
		glDrawElements(GL_TRIANGLES, m_mesh->GetSize(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}

void GameObject::Draw(Camera& _camera, float deltaTime)
{
	m_shader->Activate();
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_textures[0]->GetID());
	glUniform1i(glGetUniformLocation(m_shader->GetProgram(), "cubeSampler"), 0);

	m_shader->Transform(m_translationMatrix, m_rotationZ, m_scaleMatrix, m_modelMatrix, _camera);

	// So it uses the reference and not a copy
	// Using camera on the program
	_camera.UseCamera(m_shader->GetProgram());
	
	m_mesh->Bind();
	glDrawElements(GL_TRIANGLES, m_mesh->GetSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void GameObject::Draw(Camera & _camera, const GLchar * s_currentTime, GLfloat f_currentTime, const GLchar * s_frameTime, GLint i_frameTime, CubeMap * cube_map, float deltaTime)
{
	glFrontFace(GL_CW);
	m_shader->Activate();
	glActiveTexture(GL_TEXTURE1);
	glUniform1i(glGetUniformLocation(m_shader->GetProgram(), "skybox"), 1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map->GetID());

	m_shader->Transform(m_translationMatrix, m_rotationZ, m_scaleMatrix, m_modelMatrix, _camera);

	// So it uses the reference and not a copy
	// Using camera on the program
	_camera.UseCamera(m_shader->GetProgram());

	m_mesh->Bind();
	glDrawElements(GL_TRIANGLES, m_mesh->GetSize(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glFrontFace(GL_CCW);
}

void GameObject::draw_with_model(float deltaTime)
{
	if (m_bool_enable)
	{
		m_model->Render(deltaTime, m_modelMatrix);
	}
}

bool GameObject::sphere_collision_check(GameObject* a, GameObject* b)
{
		// We take the sum of the radius (scale) and compares
		// it to the distance between the spheres (object a location - object b location).
		// If distance is less than the
		// sum of the radius, they intersect and collides.
	if (a->m_bool_enable && b->m_bool_enable)
	{
		const glm::vec3 distance = a->GetLocation() - b->GetLocation();

		const float length = glm::length(distance);

		// Sum of the radiuses
		const float radius_sum = a->GetScale().x + b->GetScale().x;

		if (length <= radius_sum)
		{
			std::cout << "Colliding!" << std::endl;
			return true;
		}
	}
		return false;
}

float GameObject::GetPosition(int COORDINATE_ID)
{
	// float fi;
	if (COORDINATE_ID == TOP)
	{
		return (m_yPos + m_scale.y/2);
		//fi = m_mesh->GetVertices().at(1);
	}
	else if (COORDINATE_ID == BOTTOM)
	{
		return (m_yPos - m_scale.y / 2);
	}
	else if (COORDINATE_ID == LEFT)
	{
		return (m_xPos - m_scale.x /2);
	}
	else if (COORDINATE_ID == RIGHT)
	{
		return (m_xPos + m_scale.x / 2);
	}
	else
	{
		return 0.0f;
	}
	// std::cout << fi;
}

glm::vec3 GameObject::GetLocation()
{
	return m_objPosition;
}

void GameObject::Move(int MOVE_ID, float SPEED)
{
	if (MOVE_ID == MOVE_UP)
	{
		m_yPos += SPEED;
	}
	else if (MOVE_ID == MOVE_DOWN)
	{
		m_yPos -= SPEED;
	}
	else if (MOVE_ID == MOVE_RIGHT)
	{
		m_xPos += SPEED;
	}
	else if (MOVE_ID == MOVE_LEFT)
	{
		m_xPos -= SPEED;
	}
	else if (MOVE_ID == MOVE_FRONT)
	{
		m_zPos -= SPEED;
	}
	else if (MOVE_ID == MOVE_BACK)
	{
		m_zPos += SPEED;
	}

	m_objPosition = glm::vec3(m_xPos, m_yPos, m_zPos);
	m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

glm::vec3 GameObject::GetScale()
{
	return m_scale;
}

void GameObject::Scale(float _scaleX, float _scaleY, float _scaleZ)
{
	m_scale = glm::vec3(_scaleX, _scaleY, _scaleZ);
	m_objScale = glm::vec3(1.0f * m_scale.x, 1.0f * m_scale.y, 1.0f * m_scale.z);
	m_scaleMatrix = glm::scale(glm::mat4(), m_objScale);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

void GameObject::Scale(float _scale)
{
	m_scale = glm::vec3(_scale, _scale, _scale);
	m_objScale = glm::vec3(1.0f * m_scale.x, 1.0f * m_scale.y, 1.0f * m_scale.z);
	m_scaleMatrix = glm::scale(glm::mat4(), m_objScale);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

void GameObject::Enable()
{
	m_bool_enable = true;
}

void GameObject::Disable()
{
	m_bool_enable = false;
}

void GameObject::Rotate(float _angle)
{
	m_rotationAngle = _angle;
	m_rotationZ = glm::rotate(glm::mat4(), glm::radians(m_rotationAngle), m_rotationAxisZ);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

float GameObject::GetRotate()
{
	return m_rotationAngle;
}

void GameObject::SetPos(glm::vec3 _newPos)
{
	m_xPos = _newPos.x;
	m_yPos = _newPos.y;
	m_zPos = _newPos.z;
	m_objPosition = glm::vec3(m_xPos, m_yPos, m_zPos);
	m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

