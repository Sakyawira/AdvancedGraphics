#include "Camera.h"
#include <freeglut.h>

void Camera::something()
{

}

void Camera::UseCamera(GLuint program)
{
	//	view = glm::mat4(glm::mat3(view));
		// Put view matrix into '*program'
	GLuint viewLoc = glGetUniformLocation(program, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// Put the Projection (Orthographic/Perspective) Matrix into '*program'
	GLuint projLoc = glGetUniformLocation(program, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	GLuint camLoc = glGetUniformLocation(program, "camPos");
	glUniform3fv(camLoc, 1, glm::value_ptr(camPos));
}

glm::mat4 Camera::GetVP()
{
	return proj * view;
}

glm::vec3 Camera::GetPosition()
{
	return camPos;
}

void Camera::CameraUpdate(bool isGameStarted, float deltaTime, glm::vec3 _center)
{
	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camLookDir = glm::normalize(direction);

	if (isGameStarted)
	{
		//Move(_center);
		//center should be player position
		view = glm::lookAt(camPos, camPos + camLookDir, camUpDir);
		cameraRight = glm::normalize(glm::cross(camUpDir, camLookDir));
	}
	else
	{
		timeElapsed += deltaTime;
		GLfloat radius = 26.0f;

		camPos.x = sin(timeElapsed) * radius;
		camPos.y = 1.5f;
		camPos.z = cos(timeElapsed) * radius;
		// View
		view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), camUpDir);
	}
}

void Camera::MovePosX(float i_magnitude, float deltaTime)
{
	//camPos.x += (1.0f * i_magnitude);
	camPos += i_magnitude * cameraRight * deltaTime;
}

void Camera::MovePosY(float i_magnitude, float deltaTime)
{
	//camPos.y += (1.0f * i_magnitude);
	camPos -= i_magnitude * cameraRight * deltaTime;
}

void Camera::MovePosZ(float i_magnitude, float deltaTime)
{
	//camPos.z += (1.0f * i_magnitude);
	camPos += i_magnitude * camLookDir * deltaTime;
}



void Camera::SetPosX(float i_magnitude)
{
	camPos.x = i_magnitude;
}

void Camera::SetPosY(float i_magnitude)
{
	camPos.y = i_magnitude;
}

void Camera::updateLookDir(int currentX, int currentY)
{
	float offsetX = currentX - lastX;
	float offsetY = currentY - lastY;

	lastX = 0;
	lastY = 0;

	const float sensitivity = 0.05f;

	offsetX *= sensitivity;
	offsetY *= sensitivity;

	yaw += offsetX;
	pitch += offsetY;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}	
	glutWarpPointer((int)400, (int)400);
}

float Camera::calculateHorizontalDistance()
{
	return static_cast<float>(m_distance_to_player * glm::cos(glm::radians(pitch)));
}

float Camera::calculateVerticalDistance()
{
	return static_cast<float>(m_distance_to_player * glm::sin(glm::radians(pitch)));
}

void Camera::Move(glm::vec3 _center)
{
	float h_distance = calculateHorizontalDistance();
	float v_distance = calculateVerticalDistance();
	calculateCameraPosition(h_distance, v_distance, _center);
}

void Camera::calculateCameraPosition(float h_distance, float v_distance, glm::vec3 _center)
{
	float theta = /*player.getRotY() +*/ m_angle_around_player;
	float offsetX = h_distance * glm::sin(glm::radians(theta));
	float offsetZ = h_distance * glm::cos(glm::radians(theta));
	camPos.x = _center.x - offsetX;
	camPos.z = _center.z - offsetZ;
	camPos.y = _center.y + v_distance;
}

glm::mat4 Camera::get_projection()
{
	return proj;
}

glm::mat4 Camera::get_view()
{
	return view;
}


