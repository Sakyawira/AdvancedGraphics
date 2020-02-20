#include "Bullet.h"

Bullet::Bullet(Shader * _shader, Mesh * _mesh, std::vector<Texture*>& _textures, float _initial_x, float _initial_y, float _initial_z)
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

	// Pick a start direction (positive / negative)
	int negate = rand() % 2;
	m_bool_dir = (negate == 0 ? false : true);

	// Pick a start direction (vetical / horizontal)
	negate = rand() % 2;
	m_bool_vertical = (negate == 0 ? false : true);
}

Bullet::Bullet(Model * _model, float _initial_x, float _initial_y, float _initial_z)
{
	m_xPos = _initial_x;
	m_yPos = _initial_y;
	m_zPos = _initial_z;

	m_model = _model;

	m_objPosition = glm::vec3(m_xPos, m_yPos, m_zPos);
	m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

void Bullet::process(glm::vec3 _playerPos)
{
	if (glm::length(m_objPosition - m_target) < 1.0f)
	{
		m_bool_enable = false;
		m_target = _playerPos;
	}

	if (!m_bool_enable)
	{
		m_objPosition = glm::vec3(_playerPos.x, _playerPos.y, _playerPos.z);
		m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
		m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
	}
}

void Bullet::limit(glm::vec3& _vector3, float _maxMagnitude)
{
	if (glm::length(_vector3) > _maxMagnitude)
	{
		_vector3 = glm::normalize(_vector3);
		_vector3 *= _maxMagnitude;
	}
}

void Bullet::apply_force(glm::vec3 _force)
{
	m_acceleration += _force;
}

void Bullet::seek(glm::vec3 _target)
{
	// A vector pointing from the location to the target
	glm::vec3 desired = _target - m_objPosition;

	// Normalize desired and scale to maximum speed
	if (glm::length(desired) > 0.0f)
	{
		desired = glm::normalize(desired);
	}

	desired *= m_max_speed;

	// Steering = Desired minus velocity
	glm::vec3 steer = desired - m_velocity;

	// Limit to maximum steering force
	limit(steer, m_max_force);

	apply_force(steer);

	// Update velocity
	m_velocity += m_acceleration;

	// Limit speed
	limit(m_velocity, m_max_speed);

	m_objPosition += m_velocity * 1.0f;

	// Reset acceleration to 0 each cycle
	m_acceleration *= 0;

	// Update Model Matrix
	m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

void Bullet::arrive(/*glm::vec3 _target, */float delta_time)
{
	glm::vec3 _target = m_target;

	// A vector pointing from the location to the target
	glm::vec3 desired = _target - m_objPosition;

	// Get the magnitude of desired
	const float d = glm::length(desired);

	// Normalize desired
	if (glm::length(desired) > 0.0f)
	{
		desired = glm::normalize(desired);
	}

	// Map the speed depends on its distance to the target
	if (d < m_max_speed * 10.0f)
	{
		const float m = 0 + (m_max_speed - 0) * ((d - 0) / (m_max_speed * 10.0f - 0)); //* delta_time;
		if (d <= m_max_speed * 15.0f)
		{
			desired *= /*(glm::floor(*/m/*)*/ /** 1.0001f*/;
		}
		/*else
		{
			desired *= (glm::ceil(m));
		}*/
	}

	else
	{
		desired *= m_max_speed;// * delta_time;
	}

	// Steering = Desired minus Velocity
	glm::vec3 steer = desired - m_velocity;
	// Limit to maximum steering force
	limit(steer, (m_max_force * m_max_speed));

	apply_force(steer);

	// Update velocity
	m_velocity += m_acceleration;

	// Limit speed
	limit(m_velocity, m_max_speed);

	m_objPosition += m_velocity * 1.0f;

	// Reset acceleration to 0 each cycle
	m_acceleration *= 0;

	// Update Model Matrix
	m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

void Bullet::SetTarget(glm::vec3 _target)
{
	m_target = _target;
}
