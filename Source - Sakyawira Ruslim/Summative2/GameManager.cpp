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
	m_sh_fog_ = new Shader("Resources/Shaders/FogPhongVS.txt", "Resources/Shaders/FogPhongDiffuseFS.txt", m_v_sh);
	m_sh_phong_diffuse_ = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongDiffuse.fs", m_v_sh);
	m_sh_phong_specular_ = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongSpecular.fs", m_v_sh);
	m_sh_phong_rim_ = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongRim.fs", m_v_sh);
	m_sh_cube_map_ = new Shader("Resources/Shaders/CubeMapVS.txt", "Resources/Shaders/CubeMapFS.txt", m_v_sh);
	m_sh_reflective_ = new Shader("Resources/Shaders/FogReflectionVS.txt", "Resources/Shaders/FogReflectionFS.txt", m_v_sh);
	m_sh_geometry_ = new Shader("Resources/Shaders/Geometry.VS", "Resources/Shaders/Geometry.FS", "Resources/Shaders/Quad.GS", m_v_sh);
	m_sh_star_geo_ = new Shader("Resources/Shaders/Geometry.VS", "Resources/Shaders/Geometry.FS", "Resources/Shaders/Star.GS", m_v_sh);
	m_sh_tess_ = new Shader("Resources/Shaders/tess.VS", "Resources/Shaders/tess.FS", "Resources/Shaders/tessQuadModel.tcs","Resources/Shaders/tessQuadModel.tes", m_v_sh);
	m_sh_lod_ = new Shader("Resources/Shaders/tess.VS", "Resources/Shaders/tess.FS", "Resources/Shaders/tessLODQuadModel.tcs", "Resources/Shaders/tessQuadModel.tes", m_v_sh);
	m_sh_chromatical = new Shader("Resources/Shaders/Chromatical.VS", "Resources/Shaders/Chromatical.FS", m_v_sh);

	// Create Mesh
	m_mesh_static = new Mesh(animation_indices, static_vertices, m_v_mesh);
	m_mesh_animate = new Mesh(animation_indices, animation_vertices, m_v_mesh);
	m_mesh_frameBuffer = new Mesh(animation_indices, static_vertices, m_v_mesh);
	m_mesh_scroll = new Mesh(animation_indices, animation_vertices, m_v_mesh);
	m_mesh_player = new Mesh(animation_indices, player_animation_vertices, m_v_mesh);
	m_mesh_pyramid = new Mesh(pyramid_indices, pyramid_vertices, m_v_mesh);
	m_mesh_cube = new Mesh(cube_indices, cube_vertices, m_v_mesh);
	m_mesh_sphere = new Sphere();
	m_mesh_cube_map = new Mesh(cube_map_indices, cube_map_vertices, m_v_mesh);

	// Model
	m_mdl_tank = new Model("Resources/Models/Tank/Tank.obj", &camera);
	m_mdl_cat = new Model("Resources/Models/pug/Dog 1.obj", &camera);
	geomModel = new GeometryModel(m_sh_geometry_->GetProgram(), &camera);
	starModel = new GeometryModel(m_sh_star_geo_->GetProgram(), &camera);
	tessModel = new TessModel(m_sh_tess_->GetProgram(), &camera);
	lod_tessModel = new TessModel(m_sh_lod_->GetProgram(), &camera);

	// Text
	std::string m_string_menu = "Sakyawira's Burnt Out";
	std::string m_string_instruction = "Press 'R' to start the game...";
	
	m_text_instruction_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_score_, "Resources/Fonts/arial.ttf", glm::vec2(-390.0f, 350.0f), m_v_text);
	m_text_lives_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_lives_, "Resources/Fonts/arial.ttf", glm::vec2(-390.0f, 300.0f), m_v_text);
	m_text_level_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_level_, "Resources/Fonts/arial.ttf", glm::vec2( 290.0f, 350.0f), m_v_text);
	m_text_menu_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_menu, "Resources/Fonts/arial.ttf", glm::vec2(-108, 250.0f), m_v_text);
	m_text_collision_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_instruction, "Resources/Fonts/arial.ttf", glm::vec2(-108, -250.0f), m_v_text);
	m_string_bg_ = "L" + std::to_string(m_c_bg_);
	m_text_bg_ = new TextLabel(WINDOW_WIDHT, WINDOW_HEIGHT, m_string_bg_, "Resources/Fonts/waltographUI.ttf", glm::vec2(-1300.0f, -260.0f), m_v_text);

	// Texture
	m_tr_down = new Texture("Resources/Textures/down.png");
	m_tr_plain = new Texture("Resources/Textures/grass snow.png");
	m_tr_up = new Texture("Resources/Textures/up.png");
	m_tr_water = new Texture("Resources/Textures/green.png");
	m_tr_grass = new Texture("Resources/Textures/grass-texture-png-2.png");
	std::vector<const char*> textureDirs = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg"};
	m_tr_cube_map = new CubeMap(textureDirs);
	
	// Texture Vectors to be passed in
	std::vector<Texture*> v_water_texture = { m_tr_water, m_tr_water };
	std::vector<Texture*> red_texture = { m_tr_up, m_tr_up };
	std::vector<Texture*> v_cubeMap = { m_tr_cube_map };
	std::vector<Texture*> v_red = {m_tr_up};
	std::vector<Texture*> v_blue = { m_tr_down };
	std::vector<Texture*> v_yellow = { m_tr_plain, m_tr_plain };

	// Stencil Cube
	stencilCube2 = new GameObject(m_sh_phong_diffuse_, m_mesh_cube, v_blue, 0.0f, 0.0f, 0.0f, m_v_cubes);
	stencilCube2->Scale(5.5f);
	stencilCube2->Rotate(0.0f);
	stencilCube = new GameObject(m_sh_phong_diffuse_, m_mesh_cube, v_red, 0.0f, 0.0f, 0.0f, m_v_cubes);
	stencilCube->Scale(5.0f);
	stencilCube->Rotate(0.0f);

	transparentCube = new GameObject(m_sh_phong_diffuse_, m_mesh_cube, v_water_texture, 0.0f, -6.0f, 0.0f, m_v_geometry);
	transparentCube->Scale(800.0f, 8.0f, 800.0f);

	// Sky-box / Cube-Map
	sky_box = new GameObject(m_sh_cube_map_, m_mesh_cube_map, v_cubeMap, 0.0f, 0.0f, 0.0f, m_v_geometry);
	sky_box->Scale(2000.0f);

	// Pyramid
	button_down = new GameObject(m_sh_phong_diffuse_, m_mesh_cube, v_blue, -10.0f, 0.0f, 0.0f, m_v_cubes);
	button_down->Scale(3.0f);

	// Cube
	button_up = new GameObject(m_sh_phong_diffuse_, m_mesh_cube, v_yellow, 10.0f, 0.0f, 0.0f, m_v_cubes);
	button_up->Scale(3.0f);

	// Sphere
	sphere = new GameObject(m_sh_phong_diffuse_, m_mesh_sphere, v_yellow, 32.0f, 0.0f, 0.0f, m_v_sphere);
	sphere->Scale(5.0f);

	// Tank
	tank = new GameObject(m_mdl_tank, 0.0f, 0.0f, 0.0f);

	srand(static_cast<unsigned>(std::random_device()()));
	int border = 75;

	create_spheres(10, border);

	Terrain::InitInfo tii;
	tii.HeightmapFilename = "Resources/Terrain/coastMountain513.raw";
	tii.HeightScale 	= 0.35f;
	tii.HeightOffset	= -20.0f;
	tii.NumRows 		= 513;
	tii.NumCols 		= 513;
	tii.CellSpacing 	= 1.0f;
	m_mesh_terrain = new Terrain(tii, m_v_mesh);
	terrain = new GameObject(m_sh_phong_diffuse_, m_mesh_terrain, v_yellow, 0.0f, 0.0f, 0.0f, m_v_geometry);
	terrain->SetPos(glm::vec3(0.0f, -20.0f, 0.0f));


	m_frameBuffer = new FrameBuffer(m_sh_chromatical, m_mesh_static);
	
	this->initialize();
}


void GameManager::initialize()
{
	m_clock_->Initialise();
	m_clock_->Process();
	
	m_text_menu_->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_text_menu_->SetScale(0.5f);
	
	m_text_collision_->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));
	m_text_collision_->SetScale(0.39f);

	m_text_instruction_->SetScale(0.5f);
	m_text_instruction_->SetColor(glm::vec3(1.0f, 1.0f, 1.0f));

	// Reset Camera's Position
	camera.set_pos_x(0.0f);
	camera.set_pos_y(0.0f);
	camera.set_pos_z(0.0f);

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
		const float delta_t = m_clock_->GetDeltaTick();
		camera.update(m_b_start_, m_clock_->GetDeltaTick() * 1.0f, tank->GetLocation());

		// all_mouse_pick(delta_t);

		if (this->is_started())
		{
			stencilCube->SetPos(camera.get_position() + camera.get_look_dir() * 30.0f);
		}

		cube_follow_terrain();
	
		
		current_time_ = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // Get current time.
		current_time_ = current_time_ * 0.001f;

		if (m_b_start_)
		{
			// Update Texts
			m_string_score_ = "Press 'R' to reset";
			m_text_instruction_->SetText(m_string_score_);
		}
		else
		{
			m_string_score_ = "Press 'R' to use free moving camera!";
			m_text_instruction_->SetText(m_string_score_);
		}
		/*if (m_b_wireframe)
		{*/
	
			m_text_collision_->SetText("Hit 'Space' to change polygon mode.");
		
		//}
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
		// Drawing all obstacles
		//glEnable(GL_SCISSOR_TEST);
		//glScissor(0, 200, 800, 400);

		frame_counts_ += 1.0f * m_clock_->GetDeltaTick() * 120.0f;
		m_frameBuffer->PrepareRender();
		
		glEnable(GL_BLEND);
		if (m_b_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		m_tr_cube_map->Render(m_sh_cube_map_, m_mesh_cube_map, camera);

		starModel->render(glm::vec3(-10.0f, 5.0f, 0.0f), m_tr_water);
		tessModel->render(glm::vec3(10.0f, 5.0f, 0.0f));
		lod_tessModel->render(glm::vec3(0.0f, 10.0f, 0.0f));

		button_up->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		button_down->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		terrain->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		

		int i = 0;
		for (auto point : *(m_mesh_terrain->get_vertices()))
		{
			i++;
			if (point.pos.y < 30 && point.pos.y > 10)
			{
				if (i % 30 == 0)
				{
					geomModel->render(glm::vec3(point.pos.x, point.pos.y - 18, point.pos.z), m_tr_grass);
				}
			}
			
		}

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
		stencilCube2->SetPos(stencilCube->GetLocation());
		stencilCube2->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		// write to areas where value is not equal to 1
		// disable writing to stencil mask 
		glStencilMask(0x00); 
		glDisable(GL_STENCIL_TEST);
		glStencilMask(0xFF);//enable writing to stencil buffer
	
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		transparentCube->Draw(camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
	
		glDisable(GL_BLEND);
	
		//glDisable(GL_SCISSOR_TEST);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		m_frameBuffer->Render("currentTime", current_time_);
		m_text_collision_->Render();
		m_text_instruction_->Render();

	}
	else
	{
		this->initialize();
	}
}

bool GameManager::is_started() const
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

CClock* GameManager::get_clock() const
{
	return m_clock_;
}

GameObject* GameManager::update_mouse_picking()
{
	//screen pos
	glm::vec2 normalizedScreenPos = m_mouse_pos_;
	//screenpos to Proj Space
	glm::vec4 clipCoords = glm::vec4(normalizedScreenPos.x, normalizedScreenPos.y, -1.0f, 1.0f);
	//Proj Space to eye space
	glm::mat4 invProjMat = glm::inverse(camera.get_projection());
	glm::vec4 eyeCoords = invProjMat * clipCoords; 
	eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);

	//eyespace to world space
	glm::mat4 invViewMat = glm::inverse(camera.get_view());
	glm::vec4 rayWorld = invViewMat * eyeCoords;
	m_ray_direction_ = glm::normalize(glm::vec3(rayWorld));
	
	//	add code to check
	//	intersection with other objects
	const float radius = 130.0f;
	glm::vec3 start_p = camera.get_position();
	glm::vec3 end_p = camera.get_position() + m_ray_direction_ * radius;


	float mouse_pick_distance = INT_MAX;
	GameObject* return_object = nullptr;
	
	for (auto& object : m_v_cubes)
	{
		if(object->ray_box_col(camera.get_position(), m_ray_direction_, 130.0f))
		{
			const float new_distance = glm::length(object->GetLocation() - camera.get_position());
			if (new_distance < mouse_pick_distance)
			{
				mouse_pick_distance = new_distance;
				return_object = object;
			}
		}
	}
	
	//for (auto& object : m_v_sphere)
	//{
		//if (sphere->ray_sphere_col(camera.get_position(), m_ray_direction_, 10.0f))
		//{
		//	const float new_distance = glm::length(sphere->GetLocation() - camera.get_position());
		//	if (new_distance < mouse_pick_distance)
		//	{
		//		mouse_pick_distance = new_distance;
		//		return_object = sphere;
		//	}
		//}
	//}
	return return_object;
}

void GameManager::set_mouse_pos(glm::vec2 mousePos_)
{
	m_mouse_pos_ = mousePos_;
}

void GameManager::set_click(bool newState)
{
	m_is_clicked_ = newState;
}

void GameManager::cube_follow_terrain()
{
	//Handle player y position when on terrain
	float x = stencilCube->GetLocation().x;
	float y = m_mesh_terrain->get_height(stencilCube->GetLocation()) + stencilCube->GetExtents().y;
	float z = stencilCube->GetLocation().z;

	//Checks if player is off of the terrain
	if (!isnan(y) && y != -99998)
	{
		m_cube_previous_y = y;
		stencilCube->SetPos(glm::vec3(x, y -20.0f, z));
	}
	else
	{
		stencilCube->SetPos(glm::vec3(x, m_cube_previous_y, z));
	}
}

void GameManager::all_mouse_pick(float delta_t)
{
	float mouse_pick_distance = INT_MAX;
	m_text_collision_->SetText("Not Collided!");

	GameObject* picked_object = update_mouse_picking();
	if (picked_object == button_up)
	{
		m_text_collision_->SetText("Collided with Red!");
		if (m_is_clicked_)
		{
			stencilCube->Move(MOVE_FRONT, 10.0f * delta_t);
			stencilCube2->Move(MOVE_FRONT, 10.0f * delta_t);
		}
	}
	else if (picked_object == button_down)
	{
		m_text_collision_->SetText("Collided with Blue!");
		if (m_is_clicked_)
		{
			stencilCube->Move(MOVE_BACK, 10.0f * delta_t);
			stencilCube2->Move(MOVE_BACK, 10.0f * delta_t);
		}
	}

	else if (picked_object == stencilCube2)
	{
		m_text_collision_->SetText("Collided with a stenciled cube!");
	}
	else if (picked_object == stencilCube)
	{
		m_text_collision_->SetText("Collided with a stenciled cube!");
	}
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
		
		m_coin = new GameObject(m_sh_fog_, m_mesh_sphere, plain_texture, random_x, 0.0f, random_z, m_v_sphere);
		m_coin->Scale(5.0f);
	}
}


GameManager::~GameManager()
{
	delete m_mdl_cat;
	m_mdl_cat = nullptr;
	delete m_mdl_tank;
	m_mdl_tank = nullptr;
	delete geomModel;
	geomModel = nullptr;
	delete starModel;
	starModel = nullptr;
	delete tessModel;
	tessModel = nullptr;
	delete lod_tessModel;
	lod_tessModel = nullptr;

	delete tank;
	tank = nullptr;

	delete m_frameBuffer;
	m_frameBuffer = nullptr;

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
	delete m_tr_grass;
	m_tr_grass = nullptr;

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

	for (auto& geometry : m_v_cubes)
	{
		delete geometry;
		geometry = nullptr;
	}

	for (auto& geometry : m_v_sphere)
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

	//delete m_mesh_terrain;
	//m_mesh_terrain = nullptr;

	delete m_mesh_sphere;
	m_mesh_sphere = nullptr;
}