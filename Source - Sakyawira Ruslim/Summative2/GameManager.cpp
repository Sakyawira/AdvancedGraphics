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
	m_sh_fog = new Shader("Resources/Shaders/FogPhongVS.txt", "Resources/Shaders/FogPhongDiffuseFS.txt", m_v_sh);
	m_sh_phong_diffuse = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongDiffuse.fs", m_v_sh);
	m_sh_phong_specular = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongSpecular.fs", m_v_sh);
	m_sh_phong_rim = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongRim.fs", m_v_sh);
	m_sh_cube_map = new Shader("Resources/Shaders/CubeMapVS.txt", "Resources/Shaders/CubeMapFS.txt", m_v_sh);
	m_sh_reflective = new Shader("Resources/Shaders/FogReflectionVS.txt", "Resources/Shaders/FogReflectionFS.txt", m_v_sh);

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
	std::vector<const char*> textureDirs = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg"};
	m_tr_cube_map = new CubeMap(textureDirs);
	
	// Texture Vectors to be passed in
	std::vector<Texture*> v_water_texture = { m_tr_water, m_tr_water };
	std::vector<Texture*> v_texture2 = { m_tr_slimes, m_tr_slimes };
	std::vector<Texture*> v_texture = { m_tr_stars, m_tr_background };
	std::vector<Texture*> bg_texture = { m_tr_background, m_tr_background };
	std::vector<Texture*> v_cubeMap = { m_tr_cube_map };

	// Stencil Cube
	stencilCube = new GameObject(m_sh_fog, m_mesh_cube, v_texture, 0.0f, 0.0f, 0.0f, m_v_geometry);
	stencilCube->Scale(5.0f);
	stencilCube2 = new GameObject(m_sh_fog, m_mesh_cube, bg_texture, 0.0f, 0.0f, 0.0f, m_v_geometry);
	stencilCube2->Scale(6.0f);
	transparentCube = new GameObject(m_sh_fog, m_mesh_cube, v_water_texture, 0.0f, 0.0f, 0.0f, m_v_geometry);
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

	// Tank
	tank = new GameObject(m_mdl_tank, 0.0f, 0.0f, 0.0f);

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

	for (auto& coin : m_vector_coins)
	{
		coin->Enable();
	}
	
	m_b_initialized = true;
}

void GameManager::ProcessGame(Audio& audio)
{
	if (m_b_initialized == 1)
	{
		float f_deltaT = m_clock->GetDeltaTick() * 120.0f;
		camera.CameraUpdate(m_b_start, m_clock->GetDeltaTick() * 1.0f, tank->GetLocation());

		if (updateMousePicking())
		{
			m_text_instruction->SetText("Collided!");
		}
		else
		{
			m_text_instruction->SetText("Not Collided!");
		}

		if (m_b_start)
		{
			// Update Texts
			m_string_score = "Press 'R' to reset";
			m_text_score->SetText(m_string_score);


			m_string_lives = "Lives = ";
			m_text_lives->SetText(m_string_lives);

			m_string_level = "Level = ";
			m_text_level->SetText(m_string_level);

			m_string_bg = "L"+ std::to_string(m_c_bg);
			m_text_bg->SetText(m_string_bg);

			// m_text_instruction->SetText("Press 'Space' to shoot...");

			tank->sphere_collision_check(tank, cube);

			currentTime = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // Get current time.
			currentTime = currentTime * 0.001f;
			
			if (false)
			{
				m_string_menu = "You're Burnt Out!";
				m_string_instruction = "Press 'R' to play again...";
				m_text_menu->SetText(m_string_menu);
				m_text_instruction->SetText(m_string_instruction);
				tank->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
				camera.SetPosX(0.0f);
				camera.SetPosY(0.0f);
				m_b_start = false;
			}
		}
		else
		{
			m_string_score = "Press 'R' to use free moving camera!";
			m_text_score->SetText(m_string_score);
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
	
		// Drawing all obstacles
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 200, 800, 400);

		//// Drawing all obstacles
		//for (auto& coinObjects2 : m_vector_coins)
		//{
		//	coinObjects2->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//}

		//// pyramid->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//	sky_box->Draw(camera);
		m_tr_cube_map->Render(m_sh_cube_map, m_mesh_cube_map, camera);
		//
		////tank->draw_with_model(m_clock->GetDeltaTick());

		frameCounts += 1.0f * m_clock->GetDeltaTick() * 120.0f;

		//if (m_b_start == 0)
		{
		//	//Menu->Draw(camera, "currentTime", currentTime, m_clock->GetDeltaTick());
		//	// player->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		//	cube->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_clock->GetDeltaTick());
		sphere->Draw(camera, "currentTime", currentTime, "frameCounts", static_cast<int>(frameCounts), m_tr_cube_map, m_clock->GetDeltaTick());
		// m_text_menu->Render();
		//	//	m_text_bg->Render();
		}

		//if (m_text_score != nullptr)
		//{
		
		//	m_text_lives->Render();
		//	m_text_level->Render();
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
		
		glDisable(GL_SCISSOR_TEST);
		
		m_text_instruction->Render();
		m_text_score->Render();
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

void GameManager::StartGame(bool _isStart)
{
	camera.SetPosX(0.0f);
	camera.SetPosY(0.0f);
	camera.SetPosZ(26.0f);
	m_b_start = _isStart;
}

CClock* GameManager::GetClock()
{
	return m_clock;
}

bool GameManager::updateMousePicking()
{
	//screen pos
	glm::vec2 normalizedScreenPos = m_mousePos;
	//screenpos to Proj Space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);
	//Proj Space to eye space
	glm::mat4 invProjMat = glm::inverse(camera.get_projection());
	glm::vec4 eyeCoords = invProjMat * clipCoords; eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//eyespace to world space
	glm::mat4 invViewMat = glm::inverse(camera.get_view());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	m_rayDirection = glm::normalize(glm::vec3(rayWorld));
	
	//	add code to check
	//	intersection with other objects

	float radius = 5.0f;
	glm::vec3 v = sphere->GetLocation() - camera.GetPosition();
	float a = glm::dot(m_rayDirection, m_rayDirection);
	float b = 2 * glm::dot(v, m_rayDirection);
	float c = glm::dot(v, v) - radius * radius;
	float d = b * b - 4 * a* c;
	if (d > 0) 
	{
		float x1 = (-b - sqrt(d)) / 2;
		float x2 = (-b + sqrt(d)) / 2;
		if (x1 >= 0 && x2 >= 0) return true; // intersects
		if (x1 < 0 && x2 >= 0) return true; // intersects
	}
	else if (d <= 0) 
	{
		return false;// no intersection
	}
}

void GameManager::set_mouse_pos(glm::vec2 mousePos_)
{
	m_mousePos = mousePos_;
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

	for (auto& coinObjects2 : m_vector_coins)
	{
		delete coinObjects2;
		coinObjects2 = nullptr;
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