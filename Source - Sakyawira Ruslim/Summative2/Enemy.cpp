#include "Enemy.h"

Enemy::Enemy(Shader * _shader, Mesh * _mesh, std::vector<Texture*>& _textures, float _initial_x, float _initial_y, float _initial_z)
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

Enemy::Enemy(Model * _model, float _initial_x, float _initial_y, float _initial_z)
{
	m_xPos = _initial_x;
	m_yPos = _initial_y;
	m_zPos = _initial_z;

	m_model = _model;

	m_objPosition = glm::vec3(m_xPos, m_yPos, m_zPos);
	m_translationMatrix = glm::translate(glm::mat4(), m_objPosition);
	m_modelMatrix = m_translationMatrix * m_rotationZ * m_scaleMatrix;
}

void Enemy::Process(int WINDOW_WIDTH, int WINDOW_HEIGHT, int player_size, float delta_time)
{
	if (m_bool_random_move == true)
	{
		int negate = rand() % 260;
		m_bool_vertical = (negate == 0 ? !m_bool_vertical : m_bool_vertical);

		negate = rand() % 260;
		m_bool_dir = (negate == 0 ? !m_bool_dir : m_bool_dir);
		
	}

	if (m_bool_vertical)
	{
		if (static_cast<int>(m_yPos) == WINDOW_HEIGHT - (static_cast<int>(m_scale.z) + 150) || static_cast<int>(m_yPos) == - WINDOW_HEIGHT + (static_cast<int>(m_scale.z) + 150))
		{
			m_bool_dir = !m_bool_dir;
		}
		if (m_bool_dir)
		{
			Move(MOVE_UP, 1.0f * delta_time);
		}
		else
		{
			Move(MOVE_DOWN, 1.0f * delta_time);
		}
	}
	else
	{
		if (static_cast<int>(m_xPos) == WINDOW_WIDTH - (static_cast<int>(m_scale.z) + 150) || static_cast<int>(m_xPos) == -WINDOW_WIDTH + (static_cast<int>(m_scale.z) + 150))
		{
			m_bool_dir = !m_bool_dir;
		}
		if (m_bool_dir)
		{
			Move(MOVE_RIGHT, 1.0f * delta_time);
		}
		else
		{
			Move(MOVE_LEFT, 1.0f * delta_time);
		}
	}
}

void Enemy::RandomOn()
{
	m_bool_random_move = true;
}

void Enemy::limit(glm::vec3& _vector3, float _maxMagnitude)
{
	if (glm::length(_vector3) > _maxMagnitude)
	{
		_vector3 = glm::normalize(_vector3);
		_vector3 *= _maxMagnitude;
	}
}

void Enemy::apply_force(glm::vec3 _force)
{
	m_acceleration += _force;
}

void Enemy::seek(glm::vec3 _target)
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