/****************************************************
  Bachelor of Software Engineering
  Media Design School
  Auckland
  New Zealand
 
  (c) 2018 Media Design School
 
  File Name		: "Terrain.h"
  Description	: contains declaration of the Terrain class
  Author		: Sakyawira
  Mail			: Sakyawira.Rus8080@mediadesign.school.nz
******************************************************/
#pragma once

#include <glew.h>
#include <freeglut.h>
#include "Camera.h"
#include "Mesh.h"
#include "TessModel.h"

struct TerrainAttribute
{
	glm::vec3 pos;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

class Terrain : public TessModel
{
public:
	struct InitInfo
	{
		std::string HeightmapFilename;

		float HeightScale;
		float HeightOffset;
		GLuint NumRows;
		GLuint NumCols;
		float CellSpacing;
	};

	Terrain(InitInfo _info, std::vector<Mesh*>& meshVector, GLuint program, Camera* camera);
	virtual ~Terrain();
	void render(glm::vec3 _position);

	float get_height(glm::vec3 _position)const;
	std::vector<TerrainAttribute>* get_vertices();

private:

	float width()const;
	float depth()const;

	void load_heightmap();
	void smooth();
	bool in_bounds(GLuint i, GLuint j);
	float average(GLuint i, GLuint j);

	void build_vb();
	void build_ib();

	InitInfo m_info;

	GLuint m_vertices_number_;
	GLuint m_faces_number_;

	std::vector<float> m_v_heightmap;

	std::vector<TerrainAttribute> vertices;
	std::vector<GLuint> indices;
};

