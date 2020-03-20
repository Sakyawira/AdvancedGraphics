/***********************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand

  (c) 2018 Media Design School

  File Name   :   GameObject.h
  Description :   contains definition of GameManager class which control the gameplay, rendering of all game objects
  Author      :   Sakyawira Nanda Ruslim
  Mail        :   Sakyawira.Rus8080@mediadesign.school.nz
********************/
#include "GameManager.h"
#include <random>

GameManager::GameManager()
{
	m_b_initialized = false;

	// Create Clock
	m_clock = new CClock();
	// Create Shader
	m_sh_alternating = new Shader("Resources/Shaders/AlternatingVertex.txt", "Resources/Shaders/AlternatingFragment.txt", m_v_sh);
	m_sh_animate = new Shader("Resources/Shaders/AnimationVertex.txt", "Resources/Shaders/AnimationFragment.txt", m_v_sh);
	m_sh_scroll = new Shader("Resources/Shaders/ScrollingVertex.txt", "Resources/Shaders/AnimationFragment.txt", m_v_sh);
	m_sh_phong_diffuse = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongDiffuse.fs", m_v_sh);
	m_sh_phong_specular = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongSpecular.fs", m_v_sh);
	m_sh_phong_rim = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongRim.fs", m_v_sh);
	m_sh_cube_map = new Shader("Resources/Shaders/CubeMapVS.txt", "Resources/Shaders/CubeMapFS.txt", m_v_sh);
	m_sh_reflective = new Shader("Resources/Shaders/FogVS.txt", "Resources/Shaders/FogFS.txt", m_v_sh);

	// Create Mesh
	m_mesh_static = new Mesh(animation_indices, static_vertices, m_v_mesh);
	m_mesh_animate = new Mesh(animation_indices, animation_vertices, m_v_mesh);
	m_mesh_animate2 = new Mesh(animation_indices, enemy2_vertices, m_v_mesh);
	m_mesh_scroll = new Mesh(animation_indices, animation_vertices, m_v_mesh);
	m_mesh_player = new Mesh(animation_indices, player_animation_vertices, m_v_mesh);
	m_mesh_pyramid = new Mesh(pyramid_indices, pyramid_vertices, m_v_mesh);
	m_mesh_cube = new Mesh(cube_indices, cube_vertices, m_v_mesh);
	m_mesh_sphere = new Sphere();
	m_mesh_cube_map = new Mesh(cube_map_indices, cube_map_vertices, m_v_mesh);

	// Model
	m_mdl_tank = new Model("Resources/Models/Tank/Tank.obj", &camera);
	m_mdl_cat = new Model("Resources/Models/pug/Dog 1.obj", &camera);

	// Text
	std::string m_string_menu = "Sakyawira's Burnt Out";
	std::string m_string_instruction = "Press 'R' to start the game...";
	
	m_text_score = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_score, "Resources/Fonts/arial.ttf", glm::vec2(-390.0f, 350.0f), m_v_text);
	m_text_lives = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_lives, "Resources/Fonts/arial.ttf", glm::vec2(-390.0f, 300.0f), m_v_text);
	m_text_level = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_level, "Resources/Fonts/arial.ttf", glm::vec2( 290.0f, 350.0f), m_v_text);
	m_text_menu = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_menu, "Resources/Fonts/arial.ttf", glm::vec2(-108, 250.0f), m_v_text);
	m_text_instruction = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_instruction, "Resources/Fonts/arial.ttf", glm::vec2(-108, -250.0f), m_v_text);
	m_string_bg = "L" + std::to_string(m_c_bg);
	m_text_bg = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_bg, "Resources/Fonts/waltographUI.ttf", glm::vec2(-1300.0f, -260.0f), m_v_text);

	// Texture
	m_tr_stars = new Texture("Resources/Textures/stars.png");
	m_tr_background = new Texture("Resources/Textures/bullet.png");
	m_tr_slimes = new Texture("Resources/Textures/Slimes.png");
	m_tr_water = new Texture("Resources/Textures/green.png");
	//m_tr_menu = new Texture("Resources/Textures/Menu.png");
	std::vector<const char*> textureDirs = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg"};
	m_tr_cube_map = new CubeMap(textureDirs);
	
	// Texture Vectors to be passed in
	std::vector<Texture*> v_water_texture = { m_tr_water, m_tr_water };
	//std::vector<Texture*> v_menu_texture = { m_tr_menu, m_tr_menu };
	std::vector<Texture*> v_texture2 = { m_tr_slimes, m_tr_slimes };
	std::vector<Texture*> v_texture = { m_tr_stars, m_tr_background };
	std::vector<Texture*> bg_texture = { m_tr_background, m_tr_background };
	std::vector<Texture*> v_cubeMap = { m_tr_cube_map };

	// Stencil Cube
	stencilCube = new GameObject(m_sh_phong_diffuse, m_mesh_cube, v_texture, 0.0f, 0.0f, 0.0f, m_v_geometry);
	stencilCube->Scale(5.0f);
	stencilCube2 = new GameObject(m_sh_phong_diffuse, m_mesh_cube, bg_texture, 0.0f, 0.0f, 0.0f, m_v_geometry);
	stencilCube2->Scale(6.0f);
	transparentCube = new GameObject(m_sh_phong_diffuse, m_mesh_cube, v_water_texture, 0.0f, 0.0f, 0.0f, m_v_geometry);
	transparentCube->Scale(8.0f);

	// Sky-box / Cube-Map
	sky_box = new GameObject(m_sh_cube_map, m_mesh_cube_map, v_cubeMap, 0.0f, 0.0f, 0.0f, m_v_geometry);
	sky_box->Scale(2000.0f);

	// Pyramid
	pyramid = new GameObject(m_sh_alternating, m_mesh_pyramid, bg_texture, 0.0f, 0.0f, 0.0f, m_v_geometry);

	// Cube
	cube = new GameObject(m_sh_phong_specular, m_mesh_sphere, bg_texture, 0.0f, -10.0f, 0.0f, m_v_geometry);
	cube->Scale(5.0f);

	// Sphere
	sphere = new GameObject(m_sh_reflective, m_mesh_sphere, bg_texture, 10.0f, 0.0f, 0.0f, m_v_geometry);
	sphere->Scale(5.0f);

	// Create Menu Object
	/*Menu = new GameObject(m_sh_alternating, m_mesh_static, v_menu_texture, 0.0f, 0.0f, 0.0f);*/

	// Tank
	tank = new GameObject(m_mdl_tank, 0.0f, 0.0f, 0.0f);

	// Create bullet
	m_bullet = new Bullet(m_sh_phong_rim, m_mesh_sphere, bg_texture, -10.0f, 0.0f, 0.0f);
	m_bullet->Scale(3.0f);

	srand(static_cast<unsigned>(std::random_device()()));
	int border = 350 - static_cast<int>(tank->GetScale()) * 2;

	CreateCoins(10, border - 100);
	
	this->Initialize();
}


void GameManager::Initialize()
{
	m_clock->Initialise();
	m_clock->Process();
	
	m_text_menu->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_text_menu->SetScale(0.5f);
	
	m_text_instruction->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_text_instruction->SetScale(0.39f);

	m_text_score->SetScale(0.5f);
	m_text_score->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	m_text_level->SetScale(0.5f);
	m_text_level->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	m_text_lives->SetScale(0.5f);
	m_text_lives->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	m_text_bg->SetScale(130.0f);
	m_text_bg->SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
	
	//Menu->Scale(800.0f);

	// Reset Camera's Position
	camera.SetPosX(0.0f);
	camera.SetPosY(0.0f);

	// Reset score
	m_i_level = 1;
	m_i_level_threshold = 150;
	m_i_score = 0;
	m_i_lives = 3;

	for (auto& enemy : m_vector_enemies)
	{
		enemy->Enable();
	}

	for (auto& coin : m_vector_coins)
	{
		coin->Enable();
	}
	
	m_b_initialized = true;
	m_b_ended = false;
}

void GameManager::ProcessGame(Audio& audio)
{
	if (m_b_initialized == 1)
	{
		float f_deltaT = m_clock->GetDeltaTick() * 120.0f;
		camera.CameraUpdate(m_b_start, m_clock->GetDeltaTick() * 1.0f, tank->GetLocation());

		if (m_b_start)
		{
			m_bullet->process(tank->GetLocation());

			m_bullet->arrive(f_deltaT);

			for (auto& enemy : m_vector_enemies)
			{
				enemy->seek(tank->GetLocation());
			}
			
		//	Enemy_Collision_Check();

			// Update Texts
			m_string_score = "Score = " + std::to_string(m_i_score) + " / " + std::to_string(m_i_level_threshold);
			m_text_score->SetText(m_string_score);

			m_string_lives = "Lives = " + std::to_string(m_i_lives);
			m_text_lives->SetText(m_string_lives);

			m_string_level = "Level = " + std::to_string(m_i_level);
			m_text_level->SetText(m_string_level);

			m_string_bg = "L"+ std::to_string(m_c_bg);
			m_text_bg->SetText(m_string_bg);

			m_text_instruction->SetText("Press 'Space' to shoot...");

			tank->sphere_collision_check(tank, cube);

			currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // Get current time.
			currentTime = currentTime * 0.001f;
			
			if (m_i_lives <= 0)
			{
				while (m_vector_enemies.size() > 3)
				{
					Enemy* tempEnemy = m_vector_enemies.back();
					delete tempEnemy;
					tempEnemy = nullptr;
					m_vector_enemies.pop_back();
				}
				m_string_menu = "You're Burnt Out!";
				m_string_instruction = "Press 'R' to play again...";
				m_text_menu->SetText(m_string_menu);
				m_text_instruction->SetText(m_string_instruction);
				tank->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
				camera.SetPosX(0.0f);
				camera.SetPosY(0.0f);
				m_b_start = false;
				m_b_ended = true;
			}
		}
	}
	
	else
	{
		this->Initialize();
	}
	
	m_clock->Process();
}

void GameManager::Render()
{
	if (m_b_initialized == 1)
	{
		//// Drawing background
		//for (auto& backgroundObjects : m_vector_backgroundObjects)
		//{
		//	backgroundObjects->Draw(camera, "currentTime", currentTime, m_clock->GetDeltaTick());
		//}

		//// Drawing all players
		//for (auto& playerObjects : m_vector_playerObjects)
		//{
		//	playerObjects->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//}
		

		// Drawing all obstacles
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 200, 800, 400);

		//for (auto& coinObjects : m_vector_enemies)
		//{
		//	coinObjects->draw_with_model(m_clock->GetDeltaTick());
		//}

		//// Drawing all obstacles
		//for (auto& coinObjects2 : m_vector_coins)
		//{
		//	coinObjects2->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//}

		//// Draw all walls
		///*	for (auto& obstacleObjects : m_vector_obstacle_walls)
		//{
		//	obstacleObjects->Draw(camera, "currentTime", currentTime, m_clock->GetDeltaTick());
		//}*/

		//// pyramid->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//sky_box->Draw(camera);
		m_tr_cube_map->Render(m_sh_cube_map, m_mesh_cube_map, camera);
		//
		////tank->draw_with_model(m_clock->GetDeltaTick());

		//m_bullet->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//
		//frameCounts += 1.0f * m_clock->GetDeltaTick() * 120.0f;

		//if (m_i_lives <= 0 || m_b_start == 0)
		//{
		//	//Menu->Draw(camera, "currentTime", currentTime, m_clock->GetDeltaTick());
		//	// player->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//	cube->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//	sphere->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_tr_cube_map, m_clock->GetDeltaTick());
		//	m_text_menu->Render();
		//	//	m_text_bg->Render();
		//}

		//if (m_text_score != nullptr)
		//{
		//	m_text_score->Render();
		//	m_text_lives->Render();
		//	m_text_level->Render();
		//	m_text_instruction->Render();
		//}

		//enable stencil and set stencil operation 
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //stPass, dpFail, bothPass 

		//** 1st pass ** //set current stencil value 
		glStencilFunc(GL_ALWAYS, // test function 
						1,// current value to set 
						0xFF);//mask value, 
		glStencilMask(0xFF);//enable writing to stencil buffer
		//--> render regular sized cube // fills stencil buffer 
		
		stencilCube->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());

		// ** 2nd pass ** 
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); 
		glStencilMask(0x00); //disable writing to stencil buffer
		//--> render scaled up cube 
		stencilCube2->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		// write to areas where value is not equal to 1
		// disable writing to stencil mask 
		glStencilMask(0x00); 
		glDisable(GL_STENCIL_TEST);

		glStencilMask(0xFF);//enable writing to stencil buffer

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		transparentCube->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());

		glDisable(GL_BLEND);
		
	//	glDisable(GL_SCISSOR_TEST);
	}
	else
	{
		this->Initialize();
	}
}

bool GameManager::IsStarted()
{
	return m_b_start;
}

bool GameManager::IsEnded()
{
	return m_b_ended;
}

void GameManager::StartGame()
{
	m_b_start = true;
}

CClock* GameManager::GetClock()
{
	return m_clock;
}

void GameManager::CreateCoins(int _number_coins, int _border)
{
	std::vector<Texture*> v_texture2 = { m_tr_slimes, m_tr_slimes };
	// Creates coin
	for (int i = 0; i < _number_coins; ++i)
	{
		int negate = rand() % 2;
		negate = (negate == 0 ? -1 : 1);
		const float random_x = static_cast<float>((rand() % _border) * negate);
		negate = rand() % 2;
		negate = (negate == 0 ? -1 : 1);

		const float random_z = static_cast<float>((rand() % _border) * negate);
		m_coin = new Enemy(m_sh_reflective, m_mesh_sphere, v_texture2, random_x, 0.0f, random_z);
		m_coin->Scale(5.0f);
		m_coin->RandomOn();
		m_vector_coins.push_back(m_coin);
	}
}


GameManager::~GameManager()
{
	delete m_bullet;
	m_bullet = nullptr;

	delete m_mdl_cat;
	m_mdl_cat = nullptr;

	delete tank;
	tank = nullptr;

	delete m_mdl_tank;
	m_mdl_tank = nullptr;

	delete m_tr_cube_map;
	m_tr_cube_map = nullptr;

	delete m_tr_stars;
	m_tr_stars = nullptr;
	delete m_tr_background;
	m_tr_background = nullptr;
	delete m_tr_slimes;
	m_tr_slimes = nullptr;
	delete m_tr_water;
	m_tr_water = nullptr;

	for (auto& text : m_v_text)
	{
		delete text;
		text = nullptr;
	}

	for (auto& backgroundObjects : m_vector_backgroundObjects)
	{
		delete backgroundObjects;
		backgroundObjects = nullptr;
	}

	for (auto& coinObjects : m_vector_enemies)
	{
		delete coinObjects;
		coinObjects = nullptr;
	}
	for (auto& coinObjects2 : m_vector_coins)
	{
		delete coinObjects2;
		coinObjects2 = nullptr;
	}
	for (auto& obstacleObjects : m_vector_obstacle_walls)
	{
		delete obstacleObjects;
		obstacleObjects = nullptr;
	}

	delete m_clock;
	m_clock = nullptr;

	for (auto& geometry : m_v_geometry)
	{
		delete geometry;
		geometry = nullptr;
	}

	for (auto& shader : m_v_sh)
	{
		delete shader;
		shader = nullptr;
	}

	for (auto& mesh : m_v_mesh)
	{
		delete mesh;
		mesh = nullptr;
	}
}