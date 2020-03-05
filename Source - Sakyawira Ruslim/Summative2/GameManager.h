/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   GameObject.h
  Description :   contains declaration of GameManager class which control the gameplay, rendering of all game objects
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#pragma once

// Library Includes
#include <vector>

// Dependency Includes
#include <glew.h>
#include <gtc/type_ptr.hpp>
#include <glm.hpp>
#include <mat4x2.hpp>
//#include <gtc/matrix_transform.inl>

// Local Includes
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "Enemy.h"
#include "TextLabel.h"
#include "clock.h"
#include "Audio.h"
#include "Sphere.h"
#include "CubeMap.h" 
#include "Model.h"
#include "Bullet.h"
// #include "ModelMesh.h"

const int WINDOW_WIDHT = 800;
const int WINDOW_HEIGHT = 800;

class GameManager
{
public:
	GameManager();
	~GameManager();

	void Initialize();
	
	void ProcessGame(Audio& audio);

	float g_player_size = 1;

	bool collision_check(float _top, float _bottom, float _left, float _right);

	void Enemy_Collision_Check();

	void Coin_Collision_Check();

	void Bullet_Collision_Check();

	void Render();

	bool IsStarted();

	bool IsEnded();

	void StartGame();

	CClock* GetClock();

	GameObject* player;
	Camera camera;

	GameObject* tank;

	Bullet* m_bullet;
	
private:

	// Utility functions
	void CreateEnemies(int _number_enemies, int _border);
	void CreateCoins(int _number_coins, int _border);
	void ResetEnemies(int _border);
	
	// Clock
	CClock* m_clock;

	// Check whether or not m_program has been initialized
	bool m_b_initialized = false;
	
	// Control whether the game has been started
	bool m_b_start = false;
	bool m_b_ended = false;
	
	// Declaring Strings
	int m_i_score = 0;
	int m_i_lives = 3;
	int m_i_level = 1;
	int m_i_level_threshold = 150;
	std::string m_string_score;
	TextLabel* m_text_score;
	std::string m_string_lives;
	TextLabel* m_text_lives;
	std::string m_string_level;
	TextLabel* m_text_level;

	// Menu text
	char m_c_bg = 65;
	std::string m_string_bg;
	TextLabel* m_text_bg;

	// Menu Text
	std::string m_string_menu = "Collect All Crystal Balls to Win: The Game";
	TextLabel* m_text_menu;

	// Instruction Text
	std::string m_string_instruction = "Press 'R' to start the game...";
	TextLabel* m_text_instruction;

	// Uniforms
	GLfloat currentTime;
	float frameCounts = 0;

	Shader* m_sh_alternating;
	Shader* m_sh_animate;
	Shader* m_sh_scroll;
	Shader* m_sh_phong_diffuse;
	Shader* m_sh_phong_specular;
	Shader* m_sh_phong_rim;
	Shader* m_sh_cube_map;
	Shader* m_sh_reflective;

	Texture* m_tr_stars;
	Texture* m_tr_background;
	Texture* m_tr_slimes;
	Texture* m_tr_water;
	//Texture* m_tr_menu;
	CubeMap* m_tr_cube_map;

	Mesh* m_mesh_animate;
	Mesh* m_mesh_animate2;
	Mesh* m_mesh_scroll;
	Mesh* m_mesh_player;
	Mesh* m_mesh_static;
	Mesh* m_mesh_pyramid;
	Mesh* m_mesh_cube;
	Mesh* m_mesh_cube_map;
	Sphere* m_mesh_sphere;

	Model* m_mdl_tank;
	Model* m_mdl_cat;

	Enemy* m_coin;
	Enemy* m_enemy;

	GameObject* wall;
	//GameObject *Menu;
	GameObject* background;
	GameObject* pyramid;
	GameObject* cube;
	GameObject* sphere;
	GameObject* sky_box;

	GameObject* stencilCube;
	GameObject* stencilCube2;

	GameObject* transparentCube;
	
	// Vectors
	std::vector<Enemy*> m_vector_enemies;
	std::vector<Enemy*> m_vector_coins;
	std::vector<GameObject*> m_vector_obstacle_walls;
	std::vector<GameObject*> m_vector_backgroundObjects;
	std::vector<GameObject*> m_vector_playerObjects;

	std::vector<GLfloat> static_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.0f,			// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	1.0f, 0.0f,			// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 1.0f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Bottom-Right
	};

	std::vector<GLfloat> player_animation_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.001f,		// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	0.125f, 0.001f,		// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.2f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.125f, 0.2f,		// Bottom-Right
	};

	std::vector<GLfloat> enemy2_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.2f,			// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	0.125f, 0.2f,		// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.4f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.125f, 0.4f,		// Bottom-Right
	};

	// Animation has 4 textures in one image
	std::vector<GLfloat> animation_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.4f,			// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	0.125f, 0.4f,		// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.6f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	0.125f, 0.6f,		// Bottom-Right
	};

	std::vector<GLfloat> water_scrolling_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.5f, 0.0f,		0.0f, 1.0f,	1.0f,	0.0f, 0.4f,			// Top-Left
		 0.5f,  0.5f, 0.0f,		0.0f, 0.0f,	1.0f,	1.0f, 0.4f,			// Top-Right
		-0.5f, -0.5f, 0.0f,		1.0f, 0.0f, 0.0f,	0.0f, 0.6f,			// Bottom-Left
		 0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,	1.0f, 0.6f,			// Bottom-Right
	};

	// Pyramid
	std::vector<GLfloat> pyramid_vertices =
	{
		// Position				// Color			// Texture Coords
		-0.5f,  0.0f, -0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 1.0f,			// Top-Left
		-0.5f,  0.0f,  0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 0.0f,			// Top-Right
		 0.5f,  0.0f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Bottom-Left
		 0.5f,  0.0f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Bottom-Right

		 // Side 1
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Middle-Top
		 0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		 // Side 2
		 0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f, 0.0f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		 // Side 3
		 0.5f, 0.0f,  0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.0f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		// Side 4
		-0.5f, 0.0f,  0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		-0.5f, 0.0f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,

		// Top
		0.0f, 0.5f,  0.0f,		0.0f, 1.0f, 1.0f,	0.5f, 0.0f,
	};

	// Cube Vertices
	std::vector <GLfloat> cube_vertices =
	{
		// Position				// Color			// Texture Coords
		// Front
		-0.5f,  0.5f,  0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 0.0f,			// Top-Left
		-0.5f, -0.5f,  0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 1.0f,			// Top-Right
		 0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Bottom-Left
		 0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Bottom-Right

		 // Side 1
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Middle-Top
		-0.5f,-0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,
		 0.5f,-0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,			// Middle-Top
		 0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,

		 // Left
		-0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			// Middle-Top
		-0.5f,-0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		-0.5f,-0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Middle-Top
		-0.5f, 0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		 // Right
		 0.5f, 0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			// Middle-Top
		 0.5f,-0.5f,  0.5f,		0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		 0.5f,-0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Middle-Top
		 0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	1.0f, 0.0f,

		 // Top
		 -0.5f,  0.5f, -0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 0.0f,			// Top-Left
		 -0.5f,  0.5f,  0.5f,	0.0f, 1.0f,	0.0f,	0.0f, 1.0f,			// Top-Right
		  0.5f,  0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Bottom-Left
		  0.5f,  0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,			// Bottom-Right

		  // Bottom
		 -0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 0.0f,			// Middle-Top
		 -0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	0.0f, 1.0f,
		  0.5f, -0.5f, -0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 1.0f,			// Middle-Top
		  0.5f, -0.5f,  0.5f,	0.0f, 1.0f, 0.0f,	1.0f, 0.0f,
	};

	// Cube Vertices
	std::vector <GLfloat> cube_map_vertices =
	{
		//// Position			
		// Front
		-1.0f,  1.0f,  1.0f,		// 00
		-1.0f, -1.0f,  1.0f,		// 01
		 1.0f, -1.0f,  1.0f,		// 02
		 1.0f,  1.0f,  1.0f,		// 03

		 // Side 1
		-1.0f, 1.0f, -1.0f,			// 04
		-1.0f,-1.0f, -1.0f,			// 05
		 1.0f,-1.0f, -1.0f,			// 06
		 1.0f, 1.0f, -1.0f,			// 07

		// // Left
		//-1.0f, 1.0f, -1.0f,			// 08
		//-1.0f,-1.0f, -1.0f,			// 09
		//-1.0f,-1.0f,  1.0f,			// 10
		//-1.0f, 1.0f,  1.0f,			// 11

		//// Right
		//1.0f, 1.0f,  1.0f,			// 12
		//1.0f,-1.0f,  1.0f,			// 13
		//1.0f,-1.0f, -1.0f,			// 14
		//1.0f, 1.0f, -1.0f,			// 15

		//// Top
		//-1.0f,  1.0f, -1.0f,		// 16
		//-1.0f,  1.0f,  1.0f,		// 17
		// 1.0f,  1.0f,  1.0f,		// 18
		// 1.0f,  1.0f, -1.0f,		// 19

		// // Bottom
		//-1.0f, -1.0f,  1.0f,		// 20
		//-1.0f, -1.0f, -1.0f,		// 21
		// 1.0f, -1.0f, -1.0f,		// 22
		// 1.0f, -1.0f,  1.0f,		// 23
	};

	// For a cube
	std::vector <GLuint> cube_indices =
	{
		0, 1, 2,
		0, 2, 3,

		7, 6, 5,
		7, 5, 4,

		8,  9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,
		
		20, 21, 22,
		20, 22, 23,
	};

	std::vector <GLuint> cube_map_indices =
	{	
		4, 5, 1,
		4, 1, 0,

		3, 2, 6,
		3, 6, 7,

		4, 0, 3,
		4, 3, 7,

		1, 5, 6,
		1, 6, 2,

		0, 1, 2, // works
		0, 2, 3, // works 1

		7, 6, 5, // works 2
		7, 5, 4,
	};

	// Pyramid Indices
	std::vector<GLuint> pyramid_indices =
	{
		1, 0, 3,	// Base Triangle 1
		1, 3, 2,	// Base Triangle 2
		
		4, 12, 5,	// Side 1
		6, 12, 7,	// Side 2

		8, 12, 9,	// Side 3
		10, 12, 11,	// Side 4
	};
	
	// indices reuses the four unique values to create shapes
	std::vector<GLuint> indicesVector =
	{
		0, 4, 3,
		0, 1, 2, // First Triangle
		2, 3, 0, // Second Triangle
		1, 5, 2,
	};

	std::vector<GLuint> animation_indices =
	{
		1, 2, 3, // First Triangle
		1, 0, 2, // Second Triangle
	};
};

