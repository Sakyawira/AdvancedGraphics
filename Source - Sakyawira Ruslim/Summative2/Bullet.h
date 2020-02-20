#pragma once
#include "GameObject.h"

class Bullet : public GameObject
{
public:
	Bullet(Shader* _shader, Mesh* _mesh, std::vector<Texture*>& _textures, float _initial_x, float _initial_y, float _initial_z);
	Bullet(Model * _model, float _initial_x, float _initial_y, float _initial_z);

	~Bullet() = default;
	void process(glm::vec3 _playerPos);
	void limit(glm::vec3& _vector3, float _maxMagnitude);
	void apply_force(glm::vec3 _force);
	void seek(glm::vec3 _target);
	void arrive(/*glm::vec3 _target,*/ float delta_time);
	void SetTarget(glm::vec3 _target);

private:

	glm::vec3 m_acceleration = glm::vec3(0.0f, 0.0f, 0.0f);;
	glm::vec3 m_velocity = glm::vec3(0.0f, 0.1f, 0.0f);

	bool m_bool_dir = false;
	bool m_bool_vertical = false;
	bool m_bool_random_move = false;

	float r = 6.0f;
	float m_max_force = 0.05f;    // Maximum steering force
	float m_max_speed = 3.0f;     // Maximum speed

	float m_wander_theta = 0.0f;

	int m_wander_counter = 10;

	glm::vec3 m_target = glm::vec3(0.0f, 0.0f, 0.0f);
};
