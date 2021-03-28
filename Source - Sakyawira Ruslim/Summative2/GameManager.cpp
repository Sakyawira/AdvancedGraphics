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
	skeletal = new Shader("Resources/Shaders/Animation.VS", "Resources/Shaders/Animation.FS", m_v_sh);
	phongDiffuse = new Shader("Resources/Shaders/PhongVS.txt", "Resources/Shaders/PhongDiffuse.fs", m_v_sh);
	cubeMap = new Shader("Resources/Shaders/CubeMapVS.txt", "Resources/Shaders/CubeMapFS.txt", m_v_sh);
	tessShadow = new Shader("Resources/Shaders/tess.VS", "Resources/Shaders/PhongDiffuse.FS", "Resources/Shaders/tessTriModel.tcs","Resources/Shaders/tessTriModel.tes", m_v_sh);
	particles = new Shader("Resources/Shaders/ParticleComputed.VS", "Resources/Shaders/ParticleComputed.FS", m_v_sh);
	compute = new Shader("Resources/Shaders/Particle.CS", m_v_sh);
	shadow = new Shader("Resources/Shaders/ShadowMap.VS", "Resources/Shaders/ShadowMap.FS",m_v_sh);
	animatedShadow = new Shader("Resources/Shaders/AnimationShadow.VS", "Resources/Shaders/ShadowMap.FS", m_v_sh);
	chromatical = new Shader("Resources/Shaders/Chromatical.VS", "Resources/Shaders/Chromatical.FS", m_v_sh);
	geometry = new Shader("Resources/Shaders/Geometry.VS", "Resources/Shaders/Geometry.FS", "Resources/Shaders/Quad.GS", m_v_sh);

	// Create Mesh
	animateMesh = new Mesh(animation_indices, animation_vertices, m_v_mesh);
	frameBufferMesh = new Mesh(animation_indices, static_vertices, m_v_mesh);
	cubeMesh = new Mesh(cube_indices, cube_vertices, m_v_mesh);
	cubeMapMesh = new Mesh(cube_map_indices, cube_map_vertices, m_v_mesh);
	Terrain::InitInfo tii;
	tii.HeightmapFilename = "Resources/Terrain/coastMountain513.raw";
	tii.HeightScale = 0.25f;
	tii.HeightOffset = -20.0f;
	tii.NumRows = 32;
	tii.NumCols = 32;
	tii.CellSpacing = 16.0f;

	// Model
	geomModel = new GeometryModel(geometry->GetProgram(), &camera);

	m_skModel = new ssAnimatedModel("Resources/Models/theDude.DAE", "Resources/Textures/theDude.png", &camera, skeletal->GetProgram(), animatedShadow->GetProgram());

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
	down = new Texture("Resources/Textures/down.png");
	plain = new Texture("Resources/Textures/grass snow.png");
	up = new Texture("Resources/Textures/up.png");
	water = new Texture("Resources/Textures/green.png");
	grass = new Texture("Resources/Textures/grass-texture-png-2.png");
	std::vector<const char*> textureDirs = {"right.jpg", "left.jpg", "top.jpg", "bottom.jpg", "back.jpg", "front.jpg"};
	cube_map = new CubeMap(textureDirs);
	terrainMesh = new Terrain(tii, m_v_mesh, tessShadow->GetProgram(), &camera, plain);
	
	// Texture Vectors to be passed in
	std::vector<Texture*> v_water_texture = { water, water };
	std::vector<Texture*> red_texture = { up, up };
	std::vector<Texture*> v_cubeMap = { cube_map };
	std::vector<Texture*> v_red = {up};
	std::vector<Texture*> v_blue = { plain };
	std::vector<Texture*> v_yellow = { plain, plain };

	// Stencil Cube
	stencilCube = new GameObject(phongDiffuse, cubeMesh, v_red, 0.0f, -130.0f, 0.0f, m_v_cubes);
	stencilCube->Scale(15.0f);

	// Sky-box / Cube-Map
	sky_box = new GameObject(cubeMap, cubeMapMesh, v_cubeMap, 0.0f, 0.0f, 0.0f, m_v_geometry);
	sky_box->Scale(2000.0f);

	// Particle System
	m_particles = new ParticleSystem(glm::vec3(10.0f, 2.0f, 0.0f), &camera, grass, particles, compute);

	srand(static_cast<unsigned>(std::random_device()()));
	int border = 75;

	//m_frameBuffer = new FrameBuffer(chromatical_, m_mesh_static);
	m_shadowMap = new ShadowMap(shadow);
	
	this->Initialise();
}


void GameManager::Initialise()
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
	
	m_b_initialized_ = true;
}

void GameManager::Update(Audio& audio)
{
	if (m_b_initialized_ == 1)
	{
		const float delta_t = m_clock_->GetDeltaTick();
		camera.update(m_b_start_, m_clock_->GetDeltaTick() * 1.0f, m_skModel->position);

		// all_mouse_pick(delta_t);

		if (this->IsStarted())
		{
			
			m_skModel->setPosition(camera.get_position() + camera.get_look_dir() * 30.0f);
			stencilCube->SetPos(m_skModel->position + camera.get_look_dir() * 30.0f);
		}

		CubeFollowTerrain();
		//m_skModel->setPosition(glm::vec3(stencilCube->GetLocation().x, stencilCube->GetLocation().y + 30, stencilCube->GetLocation().z));
		
		current_time_ = static_cast<float>(glutGet(GLUT_ELAPSED_TIME)); // Get current time.
		current_time_ = current_time_ * 0.001f;

		if (m_b_start_)
		{
			// Update Texts
			m_string_score_ = "Press 'R' to control character!";
			m_text_instruction_->SetText(m_string_score_);
		}
		else
		{
			m_string_score_ = "Press 'R' to use free moving camera! WASD to move Character.";
			m_text_instruction_->SetText(m_string_score_);
		}
		m_text_collision_->SetText("Hit 'Space' to change polygon mode.");
	}
	
	else
	{
		this->Initialise();
	}
	
	m_clock_->Process();
}

void GameManager::Render()
{
	if (m_b_initialized_ == 1)
	{
		frame_counts_ += 1.0f * m_clock_->GetDeltaTick() * 120.0f;

		glCullFace(GL_FRONT);
		m_shadowMap->start();
		m_skModel->ShadowPass(m_shadowMap, m_clock_->GetDeltaTick(), terrainMesh);
		stencilCube->DrawShadow(m_shadowMap, camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		terrainMesh->ShadowPass(glm::vec3(0.0f, -50.0f, 0.0f), m_shadowMap);
		m_shadowMap->end();
		glCullFace(GL_BACK);
	//	m_frameBuffer->PrepareRender();
		
		glEnable(GL_BLEND);
		if (m_b_wireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		cube_map->Render(cubeMap, cubeMapMesh, camera);
		terrainMesh->render(glm::vec3(0.0f, -50.0f, 0.0f), m_shadowMap);
		m_skModel->render(m_shadowMap,m_clock_->GetDeltaTick(), terrainMesh);
		stencilCube->Draw(m_shadowMap,camera, "currentTime", current_time_, "frameCounts", static_cast<int>(frame_counts_), m_clock_->GetDeltaTick());
		m_particles->render(m_clock_->GetDeltaTick(), glm::vec3(camera.get_position().x, camera.get_position().y + 25.0f, camera.get_position().z));
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//m_frameBuffer->Render("currentTime", current_time_);
		m_text_collision_->Render();
		m_text_instruction_->Render();
	}
	else
	{
		this->Initialise();
	}
}

bool GameManager::IsStarted() const
{
	return m_b_start_;
}

void GameManager::StartGame(bool _isStart)
{
	camera.set_pos_x(0.0f);
	camera.set_pos_y(1.5f);
	camera.set_pos_z(26.0f);
	camera.set_look_dir(glm::vec3(0.0f, 0.0f, 0.0f));
	m_b_start_ = _isStart;
}

CClock* GameManager::GetClock() const
{
	return m_clock_;
}

GameObject* GameManager::UpdateMousePicking()
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


	int mouse_pick_distance = INT_MAX;
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

	return return_object;
}

void GameManager::SetMousePos(glm::vec2 mousePos_)
{
	m_mouse_pos_ = mousePos_;
}

void GameManager::SetClick(bool newState)
{
	m_is_clicked_ = newState;
}

void GameManager::CubeFollowTerrain()
{
	
	// Get the height of the terrain and adjust the y position of the cube based on it
	float x = m_skModel->position.x;
	float y = terrainMesh->get_height(m_skModel->position) + (m_skModel->scale.y*30);
	float z = m_skModel->position.z;

	// If the Cube is not off the terrain
	if (!isnan(y) && y != -99999 + m_skModel->scale.y * 30)
	{
		m_cube_previous_y = y;
		m_skModel->setPosition(glm::vec3(x, y + 25.5f, z));
	}
	else
	{
		m_skModel->setPosition(glm::vec3(x, m_cube_previous_y, z));
	}
}

void GameManager::AllMousePick(float delta_t)
{
	m_text_collision_->SetText("Not Collided!");

	GameObject* picked_object = UpdateMousePicking();
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
	std::vector<Texture*> plain_texture = { plain};

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

	delete cube_map;
	cube_map = nullptr;

	delete down;
	down = nullptr;
	delete plain;
	plain = nullptr;
	delete up;
	up = nullptr;
	delete water;
	water = nullptr;
	delete grass;
	grass = nullptr;

	for (auto& text : m_v_text)
	{
		delete text;
		text = nullptr;
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
}