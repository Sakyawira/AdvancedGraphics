/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   Camera.h
  Description :   declare and define the Camera class
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once
#include <glew.h>
#include <vector>
#include <mat4x2.hpp>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>

class Camera
{
	//Square 1
public:
	Camera() = default;
	~Camera() = default;


	void UseCamera(GLuint program/*, glm::mat4 view, glm::mat4 proj*/)
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

	glm::mat4 GetVP()
	{
		return proj * view;
	}

	glm::vec3 GetPosition()
	{
		return camPos;
	}

	void CameraUpdate(bool isGameStarted, float deltaTime, glm::vec3 _center)
	{
		if (isGameStarted)
		{
			Move(_center);
			//center should be player position
			view = glm::lookAt(camPos, _center, camUpDir);
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

	void MovePosX(float i_magnitude)
	{
		camPos.x += (1.0f * i_magnitude);
	}
	void MovePosY(float i_magnitude)
	{
		camPos.y += (1.0f * i_magnitude);
	}
	void MovePosZ(float i_magnitude)
	{
		camPos.z += (1.0f * i_magnitude);
	}

	void SetPosX(float i_magnitude)
	{
		camPos.x = i_magnitude;
	}

	void SetPosY(float i_magnitude)
	{
		camPos.y = i_magnitude;
	}

	void calculateZoom()
	{
		// float zoomLevel
	}

	void calculatePitch()
	{
		//
	}

	void calculateAngleAroundPlayer()
	{
		//
	}

	float calculateHorizontalDistance()
	{
		return static_cast<float>(m_distance_to_player * glm::cos(glm::radians(pitch)));	
	}

	float calculateVerticalDistance()
	{
		return static_cast<float>(m_distance_to_player * glm::sin(glm::radians(pitch)));
	}
	void Move(glm::vec3 _center)
	{
		float h_distance = calculateHorizontalDistance();
		float v_distance = calculateVerticalDistance();
		calculateCameraPosition(h_distance, v_distance, _center);
	}
	void calculateCameraPosition(float h_distance, float v_distance,  glm::vec3 _center)
	{
		float theta = /*player.getRotY() +*/ m_angle_around_player;
		float offsetX = h_distance * glm::sin(glm::radians(theta));
		float offsetZ = h_distance * glm::cos(glm::radians(theta));
		camPos.x = _center.x - offsetX;
		camPos.z = _center.z - offsetZ;
		camPos.y = _center.y + v_distance;
	}


private:

	// Distance from player
	float m_distance_to_player = -65.0f;

	// angle around player
	float m_angle_around_player = 0.0f;

	// Pitch
	float pitch = -35.0f;

	// Time Elapse
	float timeElapsed = 0.0f;
	
	// Camera
	 glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 6.0f);
	 glm::vec3 camLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	 glm::vec3 camUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

	// View Matrix 
	glm::mat4 view = glm::lookAt(camPos, glm::vec3(0.0f, 0.0f, 0.0f), camUpDir);

	// Screen Size
	 unsigned int SCR_WIDTH = 800;
	 unsigned int SCR_HEIGHT = 800;

	 float halfScreenWidth = static_cast<float>(SCR_WIDTH) * 0.5f;
	 float halfScreenHeight = static_cast<float>(SCR_HEIGHT) * 0.5f;

	// Projection Space

	// Perspective
	glm::mat4 proj = glm::perspective(45.0f, static_cast<float>(SCR_WIDTH) / static_cast<float>(SCR_HEIGHT), 0.1f, 40000.0f);

	// Orthographic
	// glm::mat4 proj = glm::ortho(-halfScreenWidth, halfScreenWidth, -halfScreenHeight, halfScreenHeight, 0.1f, 100.0f);
};
