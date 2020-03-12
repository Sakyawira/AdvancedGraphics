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
public:
	Camera() = default;
	~Camera() = default;
	void something();

	void UseCamera(GLuint program/*, glm::mat4 view, glm::mat4 proj*/);

	glm::mat4 GetVP();

	glm::vec3 GetPosition();

	void CameraUpdate(bool isGameStarted, float deltaTime, glm::vec3 _center);

	void MovePosX(float i_magnitude);

	void MovePosY(float i_magnitude);

	void MovePosZ(float i_magnitude);

	void SetPosX(float i_magnitude);

	void SetPosY(float i_magnitude);

	void calculateZoom();

	void calculatePitch();

	void calculateAngleAroundPlayer();

	void updateLookDir(int currentX, int currentY);

	float calculateHorizontalDistance();

	float calculateVerticalDistance();

	void Move(glm::vec3 _center);
	
	void calculateCameraPosition(float h_distance, float v_distance, glm::vec3 _center);


private:

	// Distance from player
	float m_distance_to_player = -65.0f;

	// angle around player
	float m_angle_around_player = 0.0f;

	// Pitch
	float pitch = 0.0f;

	// Yaw
	float yaw = - 90.0f;

	// Time Elapse
	float timeElapsed = 0.0f;

	float lastX = 400;
	float lastY = 400;
	
	// Camera
	 glm::vec3 camPos = glm::vec3(0.0f, 0.0f, 6.0f);
	 glm::vec3 camLookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	 glm::vec3 camUpDir = glm::vec3(0.0f, 1.0f, 0.0f);
	 glm::vec3 cameraRight = glm::normalize(glm::cross(camUpDir, camLookDir));

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
