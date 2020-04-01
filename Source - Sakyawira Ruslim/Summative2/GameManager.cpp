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
	m_b_initialized_ = false;

	// Create Clock
	m_clock_ = new CClock();
	
	// Create Shader
	m_sh_fogBox = new Shader("Resources/Shaders/FogCubeMapVS.txt", "Resources/Shaders/FogCubeMapFS.txt", m_v_sh);
	m_sh_animate_ = new Shader("Resources/Shaders/AnimationVertex.txt", "Resources/Shaders/AnimationFragment.txt", m_v_sh);
	m_sh_fog_ = new Shader("Resources/Shaders/FogPhongVS.txt", "Resources/Shaders/FogPhongDiffuseFS.txt", m_v_sh);
	m_sh_phong_diffuse_ = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongDiffuse.fs", m_v_sh);
	m_sh_phong_specular_ = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongSpecular.fs", m_v_sh);
	m_sh_phong_rim_ = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongRim.fs", m_v_sh);
	m_sh_cube_map_ = new Shader("Resources/Shaders/CubeMapVS.txt", "Resources/Shaders/CubeMapFS.txt", m_v_sh);
	m_sh_reflective_ = new Shader("Resources/Shaders/FogReflectionVS.txt", "Resources/Shaders/FogReflectionFS.txt", m_v_sh);

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
	
	m_text_score_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_score_, "Resources/Fonts/arial.ttf", glm::vec2(-390.0f, 350.0f), m_v_text);
	m_text_lives_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_lives_, "Resources/Fonts/arial.ttf", glm::vec2(-390.0f, 300.0f), m_v_text);
	m_text_level_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_level_, "Resources/Fonts/arial.ttf", glm::vec2( 290.0f, 350.0f), m_v_text);
	m_text_menu_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_menu, "Resources/Fonts/arial.ttf", glm::vec2(-108, 250.0f), m_v_text);
	m_text_instruction_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_instruction, "Resources/Fonts/arial.ttf", glm::vec2(-108, -250.0f), m_v_text);
	m_string_bg_ = "L" + std::to_string(m_c_bg_);
	m_text_bg_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_bg_, "Resources/Fonts/waltographUI.ttf", glm::vec2(-1300.0f, -260.0f), m_v_text);

	// Texture
	m_tr_down = new Texture("Resources/Textures/down.png");
	m_tr_plain = new Texture("Resources/Textures/bullet.png");
	m_tr_up = new Texture("Resources/Textures/up.png");
	m_tr_water = new Texture("Resources/Textures/green.png");
	std::vector<const char*> textureDirs = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg"};
	m_tr_cube_map = new CubeMap(textureDirs);
	
	// Texture Vectors to be passed in
	std::vector<Texture*> v_water_texture = { m_tr_water, m_tr_water };
	std::vector<Texture*> v_texture2 = { m_tr_up, m_tr_up };
	std::vector<Texture*> v_down = { m_tr_down };
	std::vector<Texture*> plain_texture = { m_tr_plain, m_tr_plain };
	std::vector<Texture*> v_cubeMap = { m_tr_cube_map };
	std::vector<Texture*> v_up = {m_tr_up};

	// Stencil Cube
	stencilCube = new GameObject(m_sh_fog_, m_mesh_cube, plain_texture, 0.0f, 0.0f, 0.0f, m_v_geometry);
	stencilCube->Scale(5.0f);
	stencilCube->Rotate(30.0f);
	stencilCube2 = new GameObject(m_sh_fog_, m_mesh_cube, v_up, 0.0f, 0.0f, 0.0f, m_v_geometry);
	stencilCube2->Scale(5.5f);
	stencilCube2->Rotate(30.0f);
	transparentCube = new GameObject(m_sh_fog_, m_mesh_cube, v_water_texture, 0.0f, -6.0f, 0.0f, m_v_geometry);
	transparentCube->Scale(800.0f, 8.0f, 800.0f);

	// Sky-box / Cube-Map
	sky_box = new GameObject(m_sh_cube_map_, m_mesh_cube_map, v_cubeMap, 0.0f, 0.0f, 0.0f, m_v_geometry);
	sky_box->Scale(2000.0f);

	// Pyramid
	button_down = new GameObject(m_sh_fog_, m_mesh_cube, v_down, -10.0f, 0.0f, 0.0f, m_v_geometry);
	button_down->Scale(3.0f);

	// Cube
	button_up = new GameObject(m_sh_fog_, m_mesh_cube, v_up, 10.0f, 0.0f, 0.0f, m_v_geometry);
	button_up->Scale(3.0f);

	// Sphere
	sphere = new GameObject(m_sh_fog_, m_mesh_sphere, plain_texture, 32.0f, 0.0f, 0.0f, m_v_geometry);
	sphere->Scale(5.0f);

	// Tank
	tank = new GameObject(m_mdl_tank, 0.0f, 0.0f, 0.0f);

	srand(static_cast<unsigned>(std::random_device()()));
	int border = 75;

	create_spheres(10, border);
	
	this->initialize();
}


void GameManager::initialize()
{
	m_clock_->Initialise();
	m_clock_->Process();
	
	m_text_menu_->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_text_menu_->SetScale(0.5f);
	
	m_text_instruction_->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_text_instruction_->SetScale(0.39f);

	m_text_score_->SetScale(0.5f);
	m_text_score_->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	m_text_level_->SetScale(0.5f);
	m_text_level_->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	m_text_lives_->SetScale(0.5f);
	m_text_lives_->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	m_text_bg_->SetScale(130.0f);
	m_text_bg_->SetColor(glm::vec3(0.0f, 0.0f, 0.0f));
	
	//Menu->Scale(800.0f);

	// Reset Camera's Position
	camera.set_pos_x(0.0f);
	camera.set_pos_y(1.5f);
	camera.set_pos_z(26.0f);

	for (auto& coin : m_vector_coins)
	{
		coin->Enable();
	}
	
	m_b_initialized_ = true;
}

void GameManager::process_game(Audio& audio)
{
	if (m_b_initialized_ == 1)
	{
		float f_deltaT = m_clock_->GetDeltaTick();
		camera.update(m_b_start_, m_clock_->GetDeltaTick() * 1.0f, tank->GetLocation());

		if (update_mouse_picking(button_up))
		{
			m_text_instruction_->SetText("Collided!");
			if (m_is_clicked_)
			{
				stencilCube->Move(MOVE_FRONT, 10.0f * f_deltaT);
				stencilCube2->Move(MOVE_FRONT, 10.0f * f_deltaT);
			}
		}
		else if(update_mouse_picking(button_down))
		{
			m_text_instruction_->SetText("Collided!");
			if (m_is_clicked_)
			{
				stencilCube->Move(MOVE_BACK, 10.0f * f_deltaT);
				stencilCube2->Move(MOVE_BACK, 10.0f * f_deltaT);
			}
		}
		else
		{
			m_text_instruction_->SetText("Not Collided!");
		}
		

		if (m_b_start_)
		{
			// Update Texts
			m_string_score_ = "Press 'R' to reset";
			m_text_score_->SetText(m_string_score_);


			m_string_lives_ = "Lives = ";
			m_text_lives_->SetText(m_string_lives_);

			m_string_level_ = "Level = ";
			m_text_level_->SetText(m_string_level_);

			m_string_bg_ = "L"+ std::to_string(m_c_bg_);
			m_text_bg_->SetText(m_string_bg_);

			// m_text_instruction->SetText("Press 'Space' to shoot...");

			tank->sphere_collision_check(tank, button_up);

			current_time_ = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // Get current time.
			current_time_ = current_time_ * 0.001f;
		}
		else
		{
			m_string_score_ = "Press 'R' to use free moving camera!";
			m_text_score_->SetText(m_string_score_);
		}
	}
	
	else
	{
		this->initialize();
	}
	
	m_clock_->Process();
}

void GameManager::render()
{
	if (m_b_initialized_ == 1)
	{
		glEnable(GL_BLEND);
		// Drawing all obstacles
		glEnable(GL_SCISSOR_TEST);
		glScissor(0, 200, 800, 400);

		//
		m_tr_cube_map->Render(m_sh_fogBox, m_mesh_cube_map, camera);

		frame_counts_ += 1.0f * m_clock_->GetDeltaTick() * 120.0f;

		button_up->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		button_down->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		sphere->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());

		//// Drawing all obstacles
	for (auto& coinObjects2 : m_vector_coins)
	{
		coinObjects2->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
	}

		//tank->draw_with_model(m_clock->GetDeltaTick());
		
		//enable stencil and set stencil operation 
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE); //stPass, dpFail, bothPass 

		//** 1st pass ** //set current stencil value 
		glStencilFunc(GL_ALWAYS, // test function 
						1,// current value to set 
						0xFF);//mask value, 
		glStencilMask(0xFF);//enable writing to stencil buffer
		//--> render regular sized button_up // fills stencil buffer 
		
		stencilCube->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());

		// ** 2nd pass ** 
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF); 
		glStencilMask(0x00); //disable writing to stencil buffer
		//--> render scaled up button_up 
		stencilCube2->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		// write to areas where value is not equal to 1
		// disable writing to stencil mask 
		glStencilMask(0x00); 
		glDisable(GL_STENCIL_TEST);

		glStencilMask(0xFF);//enable writing to stencil buffer

		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		transparentCube->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());

		glDisable(GL_BLEND);
		
		glDisable(GL_SCISSOR_TEST);
		
		m_text_instruction_->Render();
		m_text_score_->Render();
	}
	else
	{
		this->initialize();
	}
}

bool GameManager::is_started()
{
	return m_b_start_;
}

void GameManager::start_game(bool _isStart)
{
	camera.set_pos_x(0.0f);
	camera.set_pos_y(1.5f);
	camera.set_pos_z(26.0f);
	camera.set_look_dir(glm::vec3(0.0f, 0.0f, 0.0f));
	stencilCube->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
	stencilCube2->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
	m_b_start_ = _isStart;
}

CClock* GameManager::get_clock()
{
	return m_clock_;
}

bool GameManager::update_mouse_picking(GameObject* _cube)
{
	//screen pos
	glm::vec2 normalizedScreenPos = m_mouse_pos_;
	//screenpos to Proj Space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);
	//Proj Space to eye space
	glm::mat4 invProjMat = glm::inverse(camera.get_projection());
	glm::vec4 eyeCoords = invProjMat * clipCoords; eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//eyespace to world space
	glm::mat4 invViewMat = glm::inverse(camera.get_view());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	m_ray_direction_ = glm::normalize(glm::vec3(rayWorld));
	
	//	add code to check
	//	intersection with other objects

	float radius = 115.0f;

	// Resize bounds
	//bounds.size = boundsSize;

	// Transform line segment from world space to local space
	// Matrix4x4 w2l = transform.worldToLocalMatrix;
	glm::vec3 start_p = camera.get_position();//w2l.MultiplyPoint(startT.position);
	glm::vec3 end_p = camera.get_position() + m_ray_direction_ * radius;//w2l.MultiplyPoint(endT.position);

	// Draw bounds
	//Gizmos.color = Color.yellow;
	//Gizmos.matrix = transform.localToWorldMatrix;
	//Gizmos.DrawWireCube(bounds.center, bounds.size);

	// Draw line segment
	//Gizmos.color = Color.cyan;
	///Gizmos.DrawCube(startP, Vector3.one * 0.3f);
	//Gizmos.DrawCube(endP, Vector3.one * 0.3f);
	//Gizmos.DrawLine(startP, endP);

	// Lowest point in center vector
	// center - extents
	// extents = size * 0.5f
	glm::vec3 min = _cube->GetMin();
	glm::vec3 max = _cube->GetMax();

	glm::vec3 dir = end_p - start_p;
	glm::vec3 one_over_dir = glm::vec3(1.0f / dir.x, 1.0f / dir.y, 1.0f / dir.z);

	// Slabs
	glm::vec3 min_slab = glm::vec3((min.x - start_p.x) * one_over_dir.x, (min.y - start_p.y) * one_over_dir.y, (min.z - start_p.z) * one_over_dir.z);
	glm::vec3 max_slab = glm::vec3((max.x - start_p.x) * one_over_dir.x, (max.y - start_p.y) * one_over_dir.y, (max.z - start_p.z) * one_over_dir.z);

	// Min/Max Slabs
	glm::vec3 i_min_slab = glm::vec3(glm::min(min_slab.x, max_slab.x), glm::min(min_slab.y, max_slab.y), glm::min(min_slab.z, max_slab.z));
	glm::vec3 i_max_slab = glm::vec3(glm::max(min_slab.x, max_slab.x), glm::max(min_slab.y, max_slab.y), glm::max(min_slab.z, max_slab.z));

	float final_min_slab = glm::max(i_min_slab.x, i_min_slab.y);
	final_min_slab = glm::max(final_min_slab, i_min_slab.z);
	float final_max_slab = glm::min(i_max_slab.x, i_max_slab.y);
	final_max_slab = glm::min(final_max_slab, i_max_slab.z);

	return final_max_slab >= 0.0f && final_max_slab >= final_min_slab && final_min_slab <= 1.0f;

	// Sphere Collision
	//glm::vec3 v = sphere->GetLocation() - camera.GetPosition();
	//float a = glm::dot(m_rayDirection, m_rayDirection);
	//float b = 2 * glm::dot(v, m_rayDirection);
	//float c = glm::dot(v, v) - radius * radius;
	//float d = b * b - 4 * a* c;

	//if (d > 0) 
	//{
	//	float x1 = (-b - sqrt(d)) / 2;
	//	float x2 = (-b + sqrt(d)) / 2;
	//	if (x1 >= 0 && x2 >= 0) return true; // intersects
	//	if (x1 < 0 && x2 >= 0) return true; // intersects
	//}
	//else if (d <= 0) 
	//{
	//	return false;// no intersection
	//}
}

void GameManager::set_mouse_pos(glm::vec2 mousePos_)
{
	m_mouse_pos_ = mousePos_;
}

void GameManager::set_click(bool newState)
{
	m_is_clicked_ = newState;
}

void GameManager::create_spheres(int _number_coins, int _border)
{
	std::vector<Texture*> plain_texture = { m_tr_plain};

	for (int i = 0; i < _number_coins; ++i)
	{
		float random_x;
		float random_z;
		do
		{
			int negate = rand() % 2;
			negate = (negate == 0 ? -1 : 1);
			random_x = (static_cast<float>((rand() % _border)+ 10.0f) * negate);
			negate = rand() % 2;
			negate = (negate == 0 ? -1 : 1);
			random_z = (static_cast<float>((rand() % _border) + 10.0f) * negate);
		} while (glm::vec3(random_x, 0.0f, random_z) == stencilCube->GetLocation() || glm::distance(glm::vec3(random_x, 0.0f, random_z), camera.get_position()) < 25.0f);
		
		m_coin = new GameObject(m_sh_fog_, m_mesh_sphere, plain_texture, random_x, 0.0f, random_z, m_v_geometry);
		m_coin->Scale(5.0f);
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

	delete m_tr_down;
	m_tr_down = nullptr;
	delete m_tr_plain;
	m_tr_plain = nullptr;
	delete m_tr_up;
	m_tr_up = nullptr;
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

	delete m_clock_;
	m_clock_ = nullptr;

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

	delete m_mesh_sphere;
	m_mesh_sphere = nullptr;
}